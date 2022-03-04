#pragma once

#define FONT_FILE "fonts/PressStart2P.ttf"

#define GAME_TITLE "Taver defins"

#define DIALOG_FONT_SIZE 16
#define DIALOG_STR_MAX_LEN 40
#define DIALOG_INPUT_FONT_SIZE 12
#define DIALOG_INPUT_MAX_LEN 40
#define DIALOG_GAP 8
#define DIALOG_FONT_COLOR sf::Color(255, 0, 0)
#define DIALOG_INPUT_FONT_COLOR sf::Color(255, 255, 255)

#define CONFIRM_FONT_SIZE 16
#define CONFIRM_STR_MAX_LEN 40
#define CONFIRM_GAP 8
#define CONFIRM_FONT_COLOR sf::Color(255, 255, 0)

#define PANEL_GAP_SIZE 4

#define PANEL_FONT_SIZE 16
#define PANEL_FONT_COLOR sf::Color(255, 255, 255)
#define PANEL_HP_BAR_BASE_COLOUR sf::Color(127, 0, 0)
#define PANEL_HP_BAR_COLOUR sf::Color(0, 255, 0)
#define PANEL_HP_BAR_BASE_LENGTH 128
#define PANEL_HP_BAR_HEIGHT 20
#define GOLD_FONT_SIZE PANEL_FONT_SIZE
#define GOLD_STR_MAXLEN 6
#define GOLD_FONT_COLOR sf::Color(255, 215, 0)

#define INFO_MAX_LEN 40
#define INFO_MAX_LINES 15
#define INFO_FONT_SIZE 12
#define INFO_COLOR sf::Color::Black
#define INFO_FONT_COLOR sf::Color(0, 255, 0)

#define MAP_FOLDER "maps/"
#define CONFIG_FOLDER "configs/"
#define SAVE_FOLDER "saves/"

#define DEFAULT_MAP_NAME "test_map"
#define CONFIG_NAME "config"
#define DEFAULT_SAVE_NAME "newgame"
#define CRASH_SAVE_NAME "crashsave"

#define MAP_FORMAT ".tmx"
#define SAVE_FORMAT ".sav"
#define CONFIG_FORMAT ".cfg"

#define CAN_SAVE_TO_DEFAULT 0
#define CAN_SAVE_TO_CRASH 0

std::string enterString(const std::string &windowName, const std::string &windowTitle, const sf::Font &font);
bool confirm(const std::string &windowName, const std::string &windowTitle, const sf::Font &font, const sf::Texture &yesTexture, const sf::Texture &noTexture);