#pragma once

#include "B_Castle.h"
#include "B_Spawn.h"
#include "B_Tower.h"
#include "B_Wall.h"
#include "E_Aviation.h"
#include "E_InfHeavy.h"
#include "E_InfLight.h"
#include "H_Aviation.h"
#include "H_InfHeavy.h"
#include "H_InfLight.h"

//! Main namespace of my library
namespace P4v25_SFML
{
	/*!
	\brief Main game class

	Contains all map data, configs, textures, other info about game level.
	*/
	class Level
	{
	private:
		//! Contains all possible Castle levels (from 0). Do not change during the game.
		std::vector<CStats> configCS;


		//! Contains all possible Tower levels (from 0). Do not change during the game.
		std::vector<TStats> configTS;


		//! Maximum HP for walls; defined for all walls in level
		double wallMaxHP;


		//! Build cost (gold) for walls
		int wallBuildCost;


		//! Wall repair cost (gold/1 HP)
		double wallRepairCost;


		//! Calculated time quant (for a turn)
		double timeQ;


		//! GOLD. SURPRISE, BL
		int gold;


		//! Size of the map (in tiles)
		int sizeX, sizeY;


		//! Matrix containing all lmap tiles, [sizeX x sizeY]
		Tile **Map; //map[x] [y]... sizeX columns of sizeY tiles


		//! Contains pointers to all enemies on the level (not in spawns)
		std::set<Enemy*> Enemies;


		//! Pointer to the Castle (to optimize operations)
		B_Castle *Castle;


		//! Pointers for Towers and Spawns (to optimize iterating)
		std::set<Building*> Towers, Spawns;


		/*!
		\brief Current game state

		- 0 = Normal
		- -1 = Castle destroyed, LOSE
		- -1 = All enemies killed & all spawns empty, WIN
		*/
		int gamestate;
		/*!
		\brief Textures (pointers)

		Key - texture code (defined in Common.h)
		Value - pointer to texture
		*/
		std::map<int, sf::Texture*> tileset;


		//! Tile width in pixels
		int tileWidth;
	public:
		/*!
		\brief Level constructor

		Constructs level from three files. Loads all map data, tileset, textures, configs and save.
		If loaded, checks all critical situations, launches pathfinder and other functions.
		If everthing is right, level is ready for the game.

		\param fmap The path to map file (Tiled map editor, .tmx)
		\param fconfig The path to config file (Text file, .cfg)
		\param fconfig The path to save file (Text file, .sav)
		*/
		Level(const std::string &fmap, const std::string &fconfig, const std::string &fsave);


		//! Destructor; clears Map, Enemies and tileset.
		~Level();


		//! Copy constructor
		Level(const Level &);


		//! Move constructor
		Level(Level &&);


		//! Copy operator =
		Level &operator =(const Level &);


		//! Move operator =
		Level &operator =(Level &&);


		/*!
		\brief Saves level

		Saves game info to text file, overwriting it.

		\param fsave Save file path (.sav)
		*/
		void save(const std::string &fsave);


		//! Returns time quant
		double getTimeQ() const { return timeQ; }


		/*!
		\brief Calculates time quant for this level, saves and returns it.

		Time quant is minimum time until the next change of the level:
		- Releasing enemies from any Spawn
		- Full reload of the Tower.
		- Sending gold from the Castle.
		- Reaching the next tile by any enemy.
		If maximum time quant is provided (usually frame period), timeQ is limited by maxTQ.

		\param maxTQ Maximum time quant
		\return Time quant
		*/
		double calcTimeQ(const double &maxTQ = 0);


		/*!
		\brief Main turn function

		Makes one turn until the next change of the level:
		- Releasing enemies from any Spawn
		- Full reload of the Tower.
		- Sending gold from the Castle.
		- Reaching the next tile by any enemy.
		If maximum time quant is provided (usually frame period), time quant is limited by maxTQ.

		Turn order:
		1. Castle regenerates.
		2. Castle sends gold (if ready).
		3. Spawn releases enemies (if ready).
		4. Enemies regenerate.
		5. Enemies move or attack their targets.
		6. Towers shoot at enemies (if ready).

		\param maxTQ Maximum time quant
		\return New game state.
		*/
		int turn(const double &maxTQ = 0);


		/*!
		\brief Makes several turns until external time quant.

		Makes several turns until the sum of their time quants reaches external time quant OR until gamestate changes to WIN or LOSE.
		\param extTQ External time quant
		\return New game state.
		*/
		int turnUntil(const double &extTQ);


		/*!
		\brief Pathfind algorithm.

		Finds paths from all tiles to the castle, uses BFS graph algorithm.
		For each map tile finds the next tile and distance to the castle for all main enemy types.
		If cannot find path from the tile to the castle, distance = -1, and next tile is current tile.

		Checks if there are path for light infantry from Spawns to Castle.

		\return 1 Castle is available from all Spawns
		\return 0 Can't find path from any Spawn.
		*/
		bool pathfind();


		/*!
		\brief Pathfind visit function

		\param curP Current tile position
		\param prevP Previous (from the castle) tile position
		*/
		void pf_visit(const Pos &curP, const Pos &prevP);


		/*!
		\brief Builds something at the position.

		Checks if position is available and if selected tile is empty from buildings (plains) and enemies.
		Checks if gold is enough for the selected building type.
		Checks if building doesn't completely block path from spawns to the Castle for light infantry.

		\param p Position for the building
		\param type Building type (11 for Wall, 12 for Tower).
		\param name Building name

		\return 1 If everything is good.
		\return 0 If position doesn't exist (doesn't fit in level size, or <0).
		\return -1 If tile type is not Plains.
		\return -2 If gold is not enough.
		\return -3 If the building blocks path for light infantry.
		\return -4 If enemies are at the building position
		*/
		int build(const Pos &p, const int &type, const std::string &name);


		/*!
		\brief Upgrades the building at the position.

		Checks if position exists.
		Checks if building exists and can be upgraded by player.
		Checks if gold is enough.
		Checks if maximum level is not reached yet.

		\param p Position of the building

		\return 1 If everything is good.
		\return 0 If position doesn't exist (doesn't fit in level size, or <0).
		\return -1 If building doesn't exist or can't be upgraded by player.
		\return -2 If gold is not enough.
		\return -3 If maximum level is already reached.
		*/
		int upgrade(const Pos &p);


		/*!
		\brief Repairs selected wall.

		Repairs wall at the position.
		Too high heal values is not a problem: wall will be healed to its maximum HP.
		If heal value <= 0 - same.

		\param p Position of the wall

		\return 1 If everything is good.
		\return 0 If position doesn't exist (doesn't fit in level size, or <0).
		\return -1 If wall doesn't exist at this position.
		\return -2 If gold is not enough.
		*/
		int repair(const Pos &p, const double &healHP = 0);


		/*!
		\brief Returns Tower stats for the selected level (from 0).

		Checks if the level exists.

		\param lvl Level
		\throw exception("Level:getTS:illegal_level") If the selected level doesn't exist.
		\return constant Link to the stats.
		*/
		const TStats &getTS(const int &lvl) const;


		/*!
		\brief Returns Castle stats for the selected level (from 0).

		Checks if the level exists.

		\param lvl Level
		\throw exception("Level:getCS:illegal_level") If the selected level doesn't exist.
		\return constant Link to the stats.
		*/
		const CStats &getCS(const int &lvl) const;


		//! Returns tile width (in pixels).
		int getTileWidth() const { return tileWidth; }


		//! Returns the number of the enemies on the level.
		int size() { return Enemies.size(); }


		//! Returns link to (x) tile column. UNSAFE: Doesn't check if the tile exists.
		Tile* &operator [](const int &x) { return Map[x]; }


		/*!
		\brief Returns link to (x, y) tile. SAFE: Checks if the tile exists.
		\throw exception("Level:operator[]:illegal_position") if the tile doesn't exist.
		*/
		Tile &operator[](const Pos &p); //SAFE


		/*!
		\brief Adds enemy to the level.

		Doesn't copy Enemy, just inserts its pointer to the list.
		Used to transfer Enemy pointers from Spawns to Map.
		*/
		void addEnemy(Enemy *ne) { Enemies.insert(ne); }


		//int removeEnemy(Enemy *me); //returns 1 if removed, 0 if not found


		//! Iterator for Enemies list
		typedef std::set<Enemy*>::iterator iterator;


		//! Const iterator for Enemies list
		typedef std::set<Enemy*>::const_iterator const_iterator;


		//! Iterator begin() function for Enemies list
		iterator begin() { return Enemies.begin(); }


		//! Iterator end() function for Enemies list
		iterator end() { return Enemies.end(); }


		//! Const iterator begin() function for Enemies list
		const_iterator begin() const { return Enemies.begin(); }


		//! Const iterator end() function for Enemies list
		const_iterator end() const { return Enemies.end(); }


		/*!
		\brief Destroys building at the tile.

		Destroys building at the selected tile, changes its type to Plains, launches pathfind.
		May change game state if Castle is destroyed.

		\param t Tile of the building

		\throw exception("Level:destroyBuilding:illegal_type") If there is no building or if it can't be destroyed.
		*/
		void destroyBuilding(Tile &t); //returns nothing. May change gamestate.


		//void killEnemy(Enemy *); //returns nothing. May change gamestate.


		/*!
		\brief Kills the enemy pointed by the iterator.

		Kills enemy pointed by the iterator. If hero was killed, launches auraCheck() for each enemy.
		May change gamestate if no enemies are left & all Spawns are empty.

		\param it Iterator to the enemy being killed.
		\return Iterator to the next enemy in the list.
		*/
		iterator killEnemy(iterator it); //returns iterator. May change gamestate.

		/*!
		\brief Removes the enemy pointed by the iterator from the list.

		Deletes info about enemy and erases its pointer from the list.

		\param it Iterator to the enemy being removed.
		\return Iterator to the next enemy in the list.
		*/
		iterator removeEnemy(iterator it);


		/*!
		\brief Checks if the given position is within the level.

		\return true If position is within the level.
		\return false If position is outside the level.
		*/
		bool checkPos(const Pos &p);


		/*!
		\brief Checks if all enemy Spawns are empty.

		\return true If all enemy Spawns are empty.
		\return false If there are some enemies left in any of spawns.
		*/
		bool allSpawnsEmpty();


		//ePos correct(const ePos &p) const;


		/*!
		\brief Returns texture defined by ID.

		Texture IDs are defined in Common.h

		\throw std::out_of_range If there's no texture with given ID in current tileset.
		\return Link to the texture.
		*/
		sf::Texture &getTexture(const int &id = 0) { return *tileset.at(id); }


		//! Returns level size.
		Pos getSize() const { return Pos(sizeX, sizeY); }
		

		//! Returns part of Castle HP (current/maximum).
		double getCastleHPpart() const { return Castle->getHPpart(); }


		//! Returns player's gold number.
		int getGold() const { return gold; }


		/*!
		\brief Draws all level objects in the window.

		Drawing order:
		1. Tiles, buildings, HPbars.
		2. Enemies, HPbars.
		3. Tower ranges.
		4. Hero aura circles.
		*/
		virtual void draw(sf::RenderWindow &window);

		/*!
		\brief Checks if given position (in pixels, in window coordinates) is inside any Enemy's sprite.

		\warning Level top-left coordinates must be (0,0)! Otherwise doesn't work correctly.
		If position is within several sprites, chooses the last of them in the list.
		\return Pointer to selected enemy
		\return nullptr if there's no enemies at the given position
		*/
		Enemy *clickEnemy(const int &x, const int &y) const;


		/*!
		\brief Checks if given position (in pixels, in window coordinates) is inside any tile.

		\warning Level top-left coordinates must be (0,0)! Otherwise doesn't work correctly.
		\return Position of the selected tile
		\return (-1, -1) if the position is outside the level.
		*/
		Pos clickTile(const int &x, const int &y) const;

		//! Returns Tower build cost (for level 0)
		int getTowerBuildCost() const { return configTS[0].upgradeCost; }


		//! Returns Wall build cost
		int getWallBuildCost() const { return wallBuildCost; }


		//! Returns Wall repair cost (for 1 HP)
		double getWallRepairCost() const { return wallRepairCost; }


		/*!
		\brief Returns Tower upgrade cost (from the given level)

		Levels are measured from 0.

		\param curLvl Current Tower level.
		\return Cost of the upgrade to the next level.
		\return -1 if maximum level is reached already.
		*/
		int getTowerUpgradeCost(const int &curLvl) const;


		/*!
		\brief Returns Castle upgrade cost (from the given level)

		Levels are measured from 0.

		\param curLvl Current Castle level.
		\return Cost of the upgrade to the next level.
		\return -1 if maximum level is reached already.
		*/
		int getCastleUpgradeCost(const int &curLvl) const;


		/*!
		\brief Returns pointer to the stats of the next Tower level.

		Levels are measured from 0.

		\param curLvl Current Tower level.
		\return Pointer to the next level stats.
		\return nullptr if maximum level is reached already.
		*/
		const TStats *getNextTowerLevel(const int &curLvl) const;


		/*!
		\brief Returns pointer to the stats of the next Castle level.

		Levels are measured from 0.

		\param curLvl Current Castle level.
		\return Pointer to the next level stats.
		\return nullptr if maximum level is reached already.
		*/
		const CStats *getNextCastleLevel(const int &curLvl) const;


		//! Returns information string about Castle HP.
		std::string getInfoHP() const { return Castle->getInfoHP(); }

		//! Returns current game state.
		int getGamestate() const { return gamestate; }
	};
}