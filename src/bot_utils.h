#pragma once
#include <string>
#include <filesystem>

namespace BotUtils {
    struct TelegramConfig {
        std::string bot_token;
        std::string chat_id;
    };

    TelegramConfig LoadTelegramConfig(const std::filesystem::path& configPath);
    bool ZipFolder(const std::filesystem::path& folderPath, const std::filesystem::path& zipPath);
    bool SendZipToTelegram(const TelegramConfig& config, const std::filesystem::path& zipPath);
}
