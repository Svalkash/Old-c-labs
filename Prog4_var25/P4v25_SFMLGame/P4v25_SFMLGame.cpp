// P4v25_SFMLGame.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include "../P4v25_Library_SFML/Level.h"
#include <SFML/Graphics.hpp>
#include "P4v25_SFMLGame.h"

using namespace std;
using namespace P4v25_SFML;

std::string enterString(const std::string &windowName, const std::string &windowTitle, const sf::Font &font)
{
	//font is ready
	string str;
	//title
	sf::Text titleText;
	titleText.setFont(font);
	titleText.setString(windowTitle);
	titleText.setCharacterSize(DIALOG_FONT_SIZE);
	titleText.setFillColor(DIALOG_FONT_COLOR);
	titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
	titleText.setPosition(DIALOG_GAP, DIALOG_GAP);
	//input
	sf::Text inputText;
	inputText.setFont(font);
	inputText.setString("");
	inputText.setCharacterSize(DIALOG_INPUT_FONT_SIZE);
	inputText.setFillColor(DIALOG_INPUT_FONT_COLOR);
	inputText.setStyle(sf::Text::Italic);
	inputText.setPosition(DIALOG_GAP, DIALOG_GAP * 2 + DIALOG_FONT_SIZE * 2 + DIALOG_GAP);
	//
	sf::RenderWindow inputWindow(sf::VideoMode(fmax(DIALOG_STR_MAX_LEN * DIALOG_FONT_SIZE, DIALOG_INPUT_MAX_LEN * DIALOG_INPUT_FONT_SIZE) + DIALOG_GAP * 2, DIALOG_FONT_SIZE * 2 + DIALOG_INPUT_FONT_SIZE + DIALOG_GAP * 4), windowName);
	//framerate limit
	inputWindow.setVerticalSyncEnabled(true); // call it once, after creating the window
	while (inputWindow.isOpen())
	{
		sf::Event event;
		while (inputWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				str.clear();
				inputWindow.close();
			}
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 8)
					if (str.length() > 0)
					{
						str.pop_back();
						inputText.setString(str);
					}
				if ((event.text.unicode > 31) && (event.text.unicode < 127))
					str += static_cast<char>(event.text.unicode);
				inputText.setString(str);
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Enter)
					inputWindow.close();
				else if (event.key.code == sf::Keyboard::Escape)
				{
					str.clear();
					inputWindow.close();
				}
			}
		}
		inputWindow.clear();
		inputWindow.draw(titleText);
		inputWindow.draw(inputText);
		inputWindow.display();
	}
	return str;
}

bool confirm(const std::string &windowName, const std::string &windowTitle, const sf::Font &font, const sf::Texture &yesTexture, const sf::Texture &noTexture)
{
	bool f;
	int buttonSize = yesTexture.getSize().x; //assuming that it is tileWidth
	//font is ready
	string str;
	//title
	sf::Text titleText;
	titleText.setFont(font);
	titleText.setString(windowTitle);
	titleText.setCharacterSize(CONFIRM_FONT_SIZE);
	titleText.setFillColor(CONFIRM_FONT_COLOR);
	titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
	titleText.setPosition(CONFIRM_GAP, CONFIRM_GAP);
	//
	sf::Sprite yesSprite;
	yesSprite.setTexture(yesTexture);
	yesSprite.setPosition(CONFIRM_GAP, CONFIRM_GAP + CONFIRM_FONT_SIZE + CONFIRM_GAP);
	sf::Sprite noSprite;
	noSprite.setTexture(noTexture);
	noSprite.setPosition(CONFIRM_GAP + buttonSize + CONFIRM_GAP, CONFIRM_GAP + CONFIRM_FONT_SIZE + CONFIRM_GAP);
	//
	sf::RenderWindow confirmWindow(sf::VideoMode(fmax(CONFIRM_FONT_SIZE * CONFIRM_STR_MAX_LEN, 2 * buttonSize + CONFIRM_GAP) + CONFIRM_GAP, CONFIRM_FONT_SIZE + buttonSize + 3 * CONFIRM_GAP), windowName);
	//framerate limit
	confirmWindow.setVerticalSyncEnabled(true); // call it once, after creating the window
	while (confirmWindow.isOpen())
	{
		sf::Event event;
		while (confirmWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				f = 0;
				confirmWindow.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (yesSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
				{
					f = 1;
					confirmWindow.close();
				}
				else if (noSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
				{
					f = 0;
					confirmWindow.close();
				}
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Enter)
				{
					f = 1;
					confirmWindow.close();
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					f = 0;
					confirmWindow.close();
				}
			}
		}
		confirmWindow.clear();
		confirmWindow.draw(titleText);
		confirmWindow.draw(yesSprite);
		confirmWindow.draw(noSprite);
		confirmWindow.display();
	}
	return f;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool gameLoaded = 0;
	try
	{
		//FONT_LOADING
		cout << "font loading" << endl;
		sf::Font font;
		if (!font.loadFromFile(FONT_FILE))
			throw exception("cannot_load_font");
		//LOADING
		cout << "game loading" << endl;
		string saveName_load, mapName;
		saveName_load = enterString("Enter save name:", "Enter save name,\nenter nothing to start a new game", font);
		if (saveName_load.length() == 0)
			saveName_load = DEFAULT_SAVE_NAME;
		const string mapFile = string(MAP_FOLDER) + string(DEFAULT_MAP_NAME) + string(MAP_FORMAT);
		const string configFile = string(CONFIG_FOLDER) + string(CONFIG_NAME) + string(CONFIG_FORMAT);
		const string saveFile_load = string(SAVE_FOLDER) + saveName_load + string(SAVE_FORMAT);
		const string saveFile_crash = string(SAVE_FOLDER) + string(CRASH_SAVE_NAME) + string(SAVE_FORMAT);
		Level game(mapFile, configFile, saveFile_load);
		//critical situations
		if (game.getGamestate() == 1)
		{
			confirm("WIN", "All enemies killed!", font, game.getTexture(1141), game.getTexture(1141));
			return 0;
		}
		if (game.getGamestate() == -1)
		{
			confirm("DEFEAT", "Castle destroyed!", font, game.getTexture(1142), game.getTexture(1142));
			return 0;
		}
		gameLoaded = 1;
		cout << "loaded" << endl;
		//WINDOW ELEMENTS SIZE
		int tileW = game.getTileWidth();
		sf::Vector2f panelPos(game.getSize().x * tileW, 0);
		sf::Vector2f gameTextPos(panelPos.x + PANEL_GAP_SIZE, panelPos.y + PANEL_GAP_SIZE);
		string gameTextStr("Game");
		sf::Vector2f gameButton1Pos(gameTextPos.x, gameTextPos.y + PANEL_FONT_SIZE + PANEL_GAP_SIZE);
		sf::Vector2f gameButton2Pos(gameButton1Pos.x + tileW + PANEL_GAP_SIZE, gameButton1Pos.y);
		sf::Vector2f selectTextPos(gameButton1Pos.x, gameButton1Pos.y + tileW + PANEL_GAP_SIZE);
		string selectTextStr("Select");
		sf::Vector2f selectButton1Pos(selectTextPos.x, selectTextPos.y + PANEL_FONT_SIZE + PANEL_GAP_SIZE);
		sf::Vector2f selectButton2Pos(selectButton1Pos.x + tileW + PANEL_GAP_SIZE, selectButton1Pos.y);
		string actionsTextStr("Actions");
		sf::Vector2f actionsTextPos(selectButton1Pos.x, selectButton1Pos.y + tileW + PANEL_GAP_SIZE);
		sf::Vector2f actionsButton1Pos(actionsTextPos.x, actionsTextPos.y + PANEL_FONT_SIZE + PANEL_GAP_SIZE);
		sf::Vector2f actionsButton2Pos(actionsButton1Pos.x + tileW + PANEL_GAP_SIZE, actionsButton1Pos.y);
		sf::Vector2f castleHPTextPos(actionsButton1Pos.x, actionsButton1Pos.y + tileW + PANEL_GAP_SIZE);
		string castleHPTextStr("Castle HP");
		sf::Vector2f castleHPBarPos(castleHPTextPos.x, castleHPTextPos.y + PANEL_FONT_SIZE + PANEL_GAP_SIZE);
		sf::Vector2f goldTextPos(castleHPBarPos.x, castleHPBarPos.y + PANEL_HP_BAR_HEIGHT + PANEL_GAP_SIZE);
		string goldTextStr("Gold");
		sf::Vector2f goldNumberPos(goldTextPos.x, goldTextPos.y + PANEL_FONT_SIZE + PANEL_GAP_SIZE);
		sf::Vector2f panelSize(
			max((int)gameTextStr.length() * PANEL_FONT_SIZE,
			max((int)selectTextStr.length() * PANEL_FONT_SIZE,
			max((int)actionsTextStr.length() * PANEL_FONT_SIZE,
			max((int)castleHPTextStr.length() * PANEL_FONT_SIZE,
			max((int)goldTextStr.length() * PANEL_FONT_SIZE,
			max(tileW * 2 + PANEL_GAP_SIZE,
			max(PANEL_HP_BAR_BASE_LENGTH,
			GOLD_STR_MAXLEN * GOLD_FONT_SIZE))))))) + PANEL_GAP_SIZE,
			PANEL_FONT_SIZE * 5 + tileW * 3 + PANEL_HP_BAR_HEIGHT + GOLD_FONT_SIZE + PANEL_GAP_SIZE * 11);
		sf::Vector2f infoPos(0, fmax(game.getSize().y * tileW, panelSize.y));
		sf::Vector2f infoSize(INFO_MAX_LEN * INFO_FONT_SIZE, INFO_MAX_LINES * INFO_FONT_SIZE);
		sf::Vector2f windowSize(fmax(panelPos.x + panelSize.x, infoSize.x), infoPos.y + infoSize.y);
		//expands...
		panelSize.x = fmax(panelSize.x, windowSize.x - panelPos.x);
		panelSize.y = fmax(panelSize.y, game.getSize().y * tileW);
		infoSize.x = fmax(panelSize.x, windowSize.x);
		//WINDOW ELEMENTS
		//windowBack
		sf::Sprite windowBackground;
		windowBackground.setTexture(game.getTexture(1001));
		windowBackground.setTextureRect(sf::IntRect(0, 0, windowSize.x, windowSize.y));
		windowBackground.setPosition(0, 0);
		//panelBack
		sf::Sprite panelBackground;
		panelBackground.setTexture(game.getTexture(1002));
		panelBackground.setTextureRect(sf::IntRect(0, 0, panelSize.x, panelSize.y));
		panelBackground.setPosition(panelPos);
		//texts
		sf::Text gameText;
		gameText.setFont(font);
		gameText.setString(gameTextStr);
		gameText.setPosition(gameTextPos);
		gameText.setCharacterSize(PANEL_FONT_SIZE);
		gameText.setFillColor(PANEL_FONT_COLOR);
		sf::Text selectText;
		selectText.setFont(font);
		selectText.setPosition(selectTextPos);
		selectText.setString(selectTextStr);
		selectText.setCharacterSize(PANEL_FONT_SIZE);
		selectText.setFillColor(PANEL_FONT_COLOR);
		sf::Text actionsText;
		actionsText.setFont(font);
		actionsText.setPosition(actionsTextPos);
		actionsText.setString(actionsTextStr);
		actionsText.setCharacterSize(PANEL_FONT_SIZE);
		actionsText.setFillColor(PANEL_FONT_COLOR);
		sf::Text castleHPText;
		castleHPText.setFont(font);
		castleHPText.setPosition(castleHPTextPos);
		castleHPText.setString(castleHPTextStr);
		castleHPText.setCharacterSize(PANEL_FONT_SIZE);
		castleHPText.setFillColor(PANEL_FONT_COLOR);
		sf::Text goldText;
		goldText.setFont(font);
		goldText.setPosition(goldTextPos);
		goldText.setString(goldTextStr);
		goldText.setCharacterSize(PANEL_FONT_SIZE);
		goldText.setFillColor(PANEL_FONT_COLOR);
		//buttons
		sf::Sprite gameButton1; //play|pause, default - play
		gameButton1.setTexture(game.getTexture(1101));
		gameButton1.setPosition(gameButton1Pos);
		sf::Sprite gameButton2; //save
		gameButton2.setTexture(game.getTexture(1103));
		gameButton2.setPosition(gameButton2Pos);
		sf::Sprite selectButton1; //buildings(active)
		selectButton1.setTexture(game.getTexture(1121));
		selectButton1.setPosition(selectButton1Pos);
		sf::Sprite selectButton2; //enemies(active)
		selectButton2.setTexture(game.getTexture(1122));
		selectButton2.setPosition(selectButton2Pos);
		sf::Sprite actionsButton1; //build tower|repair|upgrade, default - empty
		actionsButton1.setTexture(game.getTexture(0000));
		actionsButton1.setPosition(actionsButton1Pos);
		sf::Sprite actionsButton2; //build wall, default - empty
		actionsButton2.setTexture(game.getTexture(0000));
		actionsButton2.setPosition(actionsButton2Pos);
		//castleHPbars
		sf::RectangleShape castleHPBarBase;
		castleHPBarBase.setSize(sf::Vector2f(PANEL_HP_BAR_BASE_LENGTH, PANEL_HP_BAR_HEIGHT));
		castleHPBarBase.setFillColor(PANEL_HP_BAR_BASE_COLOUR);
		castleHPBarBase.setPosition(castleHPBarPos);
		sf::RectangleShape castleHPBar;
		castleHPBar.setSize(sf::Vector2f(game.getCastleHPpart() * PANEL_HP_BAR_BASE_LENGTH, PANEL_HP_BAR_HEIGHT));
		castleHPBar.setPosition(castleHPBarPos);
		castleHPBar.setFillColor(PANEL_HP_BAR_COLOUR);
		//gold
		sf::Text goldNumber;
		goldNumber.setFont(font);
		goldNumber.setPosition(goldNumberPos);
		goldNumber.setString(to_string(game.getGold()));
		goldNumber.setCharacterSize(GOLD_FONT_SIZE);
		goldNumber.setFillColor(GOLD_FONT_COLOR);
		//info
		sf::RectangleShape infoRect;
		infoRect.setPosition(infoPos);
		infoRect.setSize(infoSize);
		infoRect.setFillColor(INFO_COLOR);
		sf::Text infoText;
		infoText.setFont(font);
		infoText.setPosition(infoPos);
		infoText.setString("Loaded!");
		infoText.setCharacterSize(INFO_FONT_SIZE);
		infoText.setFillColor(INFO_FONT_COLOR);
		//WINDOW
		sf::RenderWindow gameWindow(sf::VideoMode(windowSize.x, windowSize.y), GAME_TITLE);
		gameWindow.setVerticalSyncEnabled(true); //to stabilize FPS
		//GAME FLAGS
		bool paused = true;
		/*
		0 - nothing
		1 - tile
		2 - enemy
		3 - HP
		4 - output
		*/
		int selected = 0;
		bool selectBuildings = 1;
		bool selectEnemies = 1;
		Pos selectedP(-1, -1);
		Enemy *selectedE = nullptr;
		//CLOCK START
		sf::Clock clock;
		//GAME
		while (gameWindow.isOpen())
		{
			try
			{
				sf::Event event;
				sf::Time elapsed = clock.restart();
				//EVENTS
				while (gameWindow.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						if (confirm("Are you sure?", "Close game?", font, game.getTexture(1141), game.getTexture(1142)))
							gameWindow.close();
						else
							clock.restart(); //time was spent on "confirming"
					if (event.type == sf::Event::KeyPressed)
						if ((event.key.code == sf::Keyboard::Space) || (event.key.code == sf::Keyboard::Pause))
						{
							if (paused)
							{
								paused = 0;
								gameButton1.setTexture(game.getTexture(1102));
							}
							else
							{
								paused = 1;
								gameButton1.setTexture(game.getTexture(1101));
							}
						}
					if (event.type == sf::Event::MouseButtonPressed)
					{
						//PANEL
						//play/pause
						if (gameButton1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						{
							if (paused)
							{
								paused = 0;
								gameButton1.setTexture(game.getTexture(1102));
							}
							else
							{
								paused = 1;
								gameButton1.setTexture(game.getTexture(1101));
							}
						}
						//save
						else if (gameButton2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						{
							string saveName_save = enterString("Save", "Enter save name:", font);
							if (!saveName_save.length())
							{
								//"info output"
								selected = 4;
								selectedP = Pos(-1, -1);
								actionsButton1.setTexture(game.getTexture(0000));
								actionsButton2.setTexture(game.getTexture(0000));
								selectedE = nullptr;
								infoText.setString("Cannot save:\nEmpty save name.");
							}
							else if ((!CAN_SAVE_TO_DEFAULT) && (saveName_save == string(DEFAULT_SAVE_NAME)))
							{
								//"info output"
								selected = 4;
								selectedE = nullptr;
								selectedP = Pos(-1, -1);
								actionsButton1.setTexture(game.getTexture(0000));
								actionsButton2.setTexture(game.getTexture(0000));
								infoText.setString("Cannot save:\nSave name equals to new game save name.");
							}
							else if ((!CAN_SAVE_TO_CRASH) && (saveName_save == string(CRASH_SAVE_NAME)))
							{
								//"info output"
								selected = 4;
								selectedE = nullptr;
								selectedP = Pos(-1, -1);
								actionsButton1.setTexture(game.getTexture(0000));
								actionsButton2.setTexture(game.getTexture(0000));
								infoText.setString("Cannot save:\nSave name equals to crash save name.");
							}
							else
							{
								string saveFile_save = string(SAVE_FOLDER) + saveName_save + string(SAVE_FORMAT);
								try
								{
									game.save(saveFile_save);
									//"info output"
									selected = 4;
									selectedE = nullptr;
									selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString(string("Saved to: ") + saveName_save);
								}
								catch (exception &ex)
								{
									//"info output"
									selected = 4;
									selectedE = nullptr;
									selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString(string("Cannot save:\n") + ex.what());
								}
							}
						}
						//selectBuildings
						else if (selectButton1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						{
							if (selectBuildings)
							{
								selectBuildings = 0;
								selectButton1.setTexture(game.getTexture(1111)); //sB-inactive
							}
							else
							{
								selectBuildings = 1;
								selectButton1.setTexture(game.getTexture(1121)); //sB-active
							}
						}
						//selectEnemies
						else if (selectButton2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						{
							if (selectEnemies)
							{
								selectEnemies = 0;
								selectButton2.setTexture(game.getTexture(1112)); //sE-inactive
							}
							else
							{
								selectEnemies = 1;
								selectButton2.setTexture(game.getTexture(1122)); //sE-active
							}
						}
						//build tower/repair/upgrade - IF SELECTED A TILE
						else if ((selected == 1) && (actionsButton1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)))
						{
							int rc = 1;
							B_Wall *ptrW = nullptr;
							B_Tower *ptrT = nullptr;
							B_Castle *ptrC = nullptr;
							switch (game[selectedP].type)
							{
							case 11:
								//repair wall
								if (game[selectedP].type == 11)
								{
									rc = game.repair(selectedP);
									switch (rc)
									{
									case -2:
										//"info output"
										selected = 4;
										selectedE = nullptr;
										selectedP = Pos(-1, -1);
										actionsButton1.setTexture(game.getTexture(0000));
										actionsButton2.setTexture(game.getTexture(0000));
										ptrW = dynamic_cast<B_Wall*>(game[selectedP].b);
										infoText.setString(string("Not enough gold, needed: ") + to_string(static_cast<int>(ceil(game.getWallRepairCost() * (ptrW->getMaxHP() - ptrW->getHP())))));
										break;
									case 1:
										//"info output"
										selected = 4;
										selectedE = nullptr;
										selectedP = Pos(-1, -1);
										actionsButton1.setTexture(game.getTexture(0000));
										actionsButton2.setTexture(game.getTexture(0000));
										infoText.setString("Wall repaired!");
										break;
									default:
										throw exception("Game:unexpected_repair()_return_code");
										break;
									}
								}
								break;
							case 12:
								//upgrade tower
								if (game[selectedP].type == 12)
								{
									rc = game.upgrade(selectedP);
									switch (rc)
									{
									case -2:
										//"info output"
										selected = 4;
										selectedE = nullptr;
										selectedP = Pos(-1, -1);
										actionsButton1.setTexture(game.getTexture(0000));
										actionsButton2.setTexture(game.getTexture(0000));
										ptrT = dynamic_cast<B_Tower*>(game[selectedP].b);
										infoText.setString(string("Not enough gold, needed: ") + to_string(game.getTowerUpgradeCost(ptrT->getLvl())));
										break;
									case -3:
										//"info output"
										selected = 4;
										selectedE = nullptr;
										selectedP = Pos(-1, -1);
										actionsButton1.setTexture(game.getTexture(0000));
										actionsButton2.setTexture(game.getTexture(0000));
										infoText.setString("Maximum level!");
										break;
									case 1:
										//"info output"
										selected = 4;
										selectedE = nullptr;
										selectedP = Pos(-1, -1);
										actionsButton1.setTexture(game.getTexture(0000));
										actionsButton2.setTexture(game.getTexture(0000));
										infoText.setString("Tower upgraded!");
										break;
									default:
										throw exception("Game:unexpected_upgrade()_return_code");
										break;
									}
								}
								break;
							case 13:
								//upgrade castle
								if (game[selectedP].type == 13)
								{
									rc = game.upgrade(selectedP);
									switch (rc)
									{
									case -2:
										//"info output"
										selected = 4;
										selectedE = nullptr;
										selectedP = Pos(-1, -1);
										actionsButton1.setTexture(game.getTexture(0000));
										actionsButton2.setTexture(game.getTexture(0000));
										ptrC = dynamic_cast<B_Castle*>(game[selectedP].b);
										infoText.setString(string("Not enough gold, needed: ") + to_string(game.getCastleUpgradeCost(ptrC->getLvl())));
										break;
									case -3:
										//"info output"
										selected = 4;
										selectedE = nullptr;
										selectedP = Pos(-1, -1);
										actionsButton1.setTexture(game.getTexture(0000));
										actionsButton2.setTexture(game.getTexture(0000));
										infoText.setString("Maximum level!");
										break;
									case 1:
										//"info output"
										selected = 4;
										selectedE = nullptr;
										selectedP = Pos(-1, -1);
										actionsButton1.setTexture(game.getTexture(0000));
										actionsButton2.setTexture(game.getTexture(0000));
										infoText.setString("Castle upgraded!");
										break;
									default:
										throw exception("Game:unexpected_upgrade()_return_code");
										break;
									}
								}
								break;
							case 01:
								//build tower
								string towerName = enterString("Tower name:", "Enter tower name:", font);
								if (!towerName.length())
									break;
								rc = game.build(selectedP, 12, towerName);
								switch (rc)
								{
								case -2:
									//"info output"
									selected = 4;
									selectedE = nullptr;
									selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString(string("Not enough gold, needed: ") + to_string(game.getTowerBuildCost()));
									break;
								case -3:
									//"info output"
									selected = 4;
									selectedE = nullptr;
									selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString("Cannot build there:\nYou cannot block path for enemies.");
									break;
								case -4:
									//"info output"
									selected = 4;
									selectedE = nullptr;
									selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString("Cannot build there:\nEnemies are at the position.");
									break;
								case 1:
									//"info output"
									selected = 4;
									selectedE = nullptr;
									selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString("Tower built!");
									break;
								default:
									throw exception("Game:unexpected_build()_return_code");
									break;
								}
								break;
							}
						}
						//build wall
						else if ((selected == 1) && (actionsButton2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)))
						{
							if (game[selectedP].type == 01)
							{
								//build wall
								string wallName = enterString("Wall name:", "Enter wall name:", font);
								if (!wallName.length())
									break;
								int rc = game.build(selectedP, 11, wallName);
								switch (rc)
								{
								case -2:
									//"info output"
									selected = 4;
									selectedE = nullptr;
									selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString(string("Not enough gold, needed: ") + to_string(game.getWallBuildCost()));
									break;
								case -3:
									//"info output"
									selected = 4;
									selectedE = nullptr;
									selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString("Cannot build there:\nYou cannot block path for enemies.");
									break;
								case -4:
									//"info output"
									selected = 4;
									selectedE = nullptr;
									selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString("Cannot build there:\nEnemies are at the position.");
									break;
								case 1:
									//"info output"
									selected = 4;
									selectedE = nullptr;
									selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString("Wall built!");
									break;
								default:
									throw exception("Game:unexpected_build()_return_code");
									break;
								}
							}
						}
						//SELECTION - castleHP
						else if (castleHPBarBase.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						{
							selected = 3;
							selectedE = nullptr;
							selectedP = Pos(-1, -1);
							actionsButton1.setTexture(game.getTexture(0000));
							actionsButton2.setTexture(game.getTexture(0000));
							infoText.setString(game.getInfoHP());
						}
						//SELECTION - something in the level
						else
						{
							//enemy?
							if (selectEnemies)
								selectedE = game.clickEnemy(event.mouseButton.x, event.mouseButton.y);
							else
								selectedE = nullptr;
							if (selectedE)
							{
								selected = 2;
								selectedP = Pos(-1, -1);
								actionsButton1.setTexture(game.getTexture(0000));
								actionsButton2.setTexture(game.getTexture(0000));
								infoText.setString(selectedE->getInfo());
							}
							else
							{
								if (selectBuildings)
									selectedP = game.clickTile(event.mouseButton.x, event.mouseButton.y);
								else
									selectedP = Pos(-1, -1);
								if (game.checkPos(selectedP)) //that means it is >0.. and another check is not so bad)
								{
									selected = 1;
									selectedE = nullptr;
									//plains, build-build
									switch (game[selectedP].type)
									{
									case 01:
										//buildTower
										actionsButton1.setTexture(game.getTexture(1131));
										//buildWall
										actionsButton2.setTexture(game.getTexture(1132));
										break;
									case 11:
										//repair
										actionsButton1.setTexture(game.getTexture(1134));
										//no second action
										actionsButton2.setTexture(game.getTexture(0000));
										break;
									case 12:
										if (game.getTowerUpgradeCost(dynamic_cast<B_Tower*>(game[selectedP].b)->getLvl()) == -1) //maxlvl
											actionsButton1.setTexture(game.getTexture(0000));
										else //canbeupgraded
											actionsButton1.setTexture(game.getTexture(1133));
										//no second action
										actionsButton2.setTexture(game.getTexture(0000));
										break;
									case 13:
										if (game.getCastleUpgradeCost(dynamic_cast<B_Castle*>(game[selectedP].b)->getLvl()) == -1) //maxlvl
											actionsButton1.setTexture(game.getTexture(0000));
										else //canbeupgraded
											actionsButton1.setTexture(game.getTexture(1133));
										//no second action
										actionsButton2.setTexture(game.getTexture(0000));
										break;
									default:
										actionsButton1.setTexture(game.getTexture(0000));
										actionsButton2.setTexture(game.getTexture(0000));
										break;
									}
									infoText.setString(game[selectedP].getInfo(game));
								}
								else //else you're clicking in the void)
								{
									selected = 0;
									//selectedE = nullptr;
									//selectedP = Pos(-1, -1);
									actionsButton1.setTexture(game.getTexture(0000));
									actionsButton2.setTexture(game.getTexture(0000));
									infoText.setString("");
								}
							}
						}
					}
					//restart timer anyway
					clock.restart();
				}
				//TURN
				if (!paused)
					game.turnUntil(elapsed.asSeconds());
				if (game.getGamestate() == 1)
				{
					confirm("WIN", "All enemies killed!", font, game.getTexture(1141), game.getTexture(1141));
					gameWindow.close();
					continue;
				}
				if (game.getGamestate() == -1)
				{
					confirm("DEFEAT", "Castle destroyed!", font, game.getTexture(1142), game.getTexture(1142));
					gameWindow.close();
					continue;
				}
				//update gold and HP values... YES IT'S STUPID
				castleHPBar.setSize(sf::Vector2f(game.getCastleHPpart() * PANEL_HP_BAR_BASE_LENGTH, PANEL_HP_BAR_HEIGHT));
				goldNumber.setString(to_string(game.getGold()));
				//eh.. that's all?
				//CLEARING
				gameWindow.clear();
				//DRAWING
				//background
				gameWindow.draw(windowBackground);
				//level
				game.draw(gameWindow);
				//panel
				gameWindow.draw(panelBackground);
				gameWindow.draw(gameText);
				gameWindow.draw(gameButton1);
				gameWindow.draw(gameButton2);
				gameWindow.draw(selectText);
				gameWindow.draw(selectButton1);
				gameWindow.draw(selectButton2);
				gameWindow.draw(actionsText);
				gameWindow.draw(actionsButton1);
				gameWindow.draw(actionsButton2);
				gameWindow.draw(castleHPText);
				gameWindow.draw(castleHPBarBase);
				gameWindow.draw(castleHPBar);
				gameWindow.draw(goldText);
				gameWindow.draw(goldNumber);
				//info
				gameWindow.draw(infoRect);
				gameWindow.draw(infoText);
				//DISPLAY
				gameWindow.display();
			}
			catch (exception &ex)
			{
				gameWindow.close();
				cout << "Game crashed: " << ex.what() << endl;
				try
				{
					game.save(saveFile_crash);
					cout << "Game state was saved to " << CRASH_SAVE_NAME << endl;
					system("pause");
					return 2;
				}
				catch (exception &csex)
				{
					cout << "Cannot save game: " << csex.what() << endl;
					system("pause");
					return 3;
				}
			}
		}
	}
	catch (exception &ex)
	{
		if (!gameLoaded)
			cout << "Game was not loaded: " << ex.what() << endl;
		//game was loaded
		system("pause");
		return 1;
	}
	return 0;
}