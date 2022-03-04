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
#include "myVector.h"
#include "mySet.h"

//œ¿Ã¿√»“» — œ≈–≈Ã≈Ÿ¿ﬁŸ»Ã  ŒÕ—“–” “Œ–ŒÃ » Œœ≈–¿“Œ–ŒÃ
//REMEMBER TO CHANGE TURN..killEnemy IN TEMPLATE!! WE MUSTN'T ALWAYS CHANGE THE ITERATOR!

namespace P4v25_T
{
	enum Colors {
		Black, Blue, Green, Cyan, Red, Magenta, Brown, LightGray,
		DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
	};

	template<Colors txt = LightGray, Colors bg = Black>
	std::ostream& color(std::ostream &text) {
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | txt));
		return text;
	};

	class Level
	{
	private:
		myVector<CStats> configCS;
		myVector<TStats> configTS;
		double wallMaxHP; //wall config
		int wallBuildCost; //wall config
		double wallRepairCost; //wall config, gold/1 HP
		double timeG, timeQ;
		int gold;
		int sizeX, sizeY;
		Tile **Map; //map[x] [y]... sizeX columns of sizeY tiles
		mySet<Enemy*> Enemies;
		//more memory, faster operations
		B_Castle *Castle; //maybe should open this?
		mySet<Building*> Towers, Spawns; //maybe should open this?
		int gamestate; //1 - WIN, 0 - game, -1 - LOSE
	public:
		//Level() : configCS(), configTS(), timeG(0), timeQ(0), gold(0), sizeX(0), sizeY(0), Map(nullptr), set<Enemy*>(nullptr), Castle(nullptr), Towers(), Spawns(), sizeE(0), maxSZE(0) {} //MUST NOT BE USED!!
		Level(const std::string &, const std::string &, const std::string &); //for map, config, save. standard save MUST BE IN THE DIRECTORY. fail = stop. INit: CHECK PATHFIND!!!!!!11!1!
		~Level();

		Level(const Level &);
		Level(Level &&);
		Level &operator =(const Level &);
		Level &operator =(Level &&);
		
		bool save(const std::string &fsave);
		//
		double getTimeG() const { return timeG; }
		double getTimeQ() const { return timeQ; }
		double calcTimeQ();
		int  turn(); //returns -1 if gameover, 1 if WIN, otherwise - 0. CHANGES IN STL VERSION
		bool pathfind(); //returns 0 if there's no way from spawn to castle
		void pf_visit(const Pos &, const Pos &);
		/*
		build rc:
		0 not existing position
		-1 wrong place
		-2 not enough gold
		-3 blocked path
		-4 enemies at position
		*/
		int build(const Pos &, const int &, const std::string &); //position, type (tower or wall). RETURNS ERROR CODE!
		/*
		upgrade rc:
		0 not existing position
		-1 not upgradable/wrong place
		-2 not enough gold
		-3 maximum level
		*/
		int upgrade(const Pos &);
		/*
		Too high values is not a problem: wall will be healed to its maxHP.
		repair rc:
		0 not existing position
		-1 not wall
		-2 not enough gold
		-3 negative heal, are you kidding?
		*/
		int repair(const Pos &, const double &);
		friend std::ostream &operator <<(std::ostream &, Level &); //can throw badalloc!
		const TStats getTS(const int &);
		const CStats getCS(const int &);
		const int size() { return Enemies.size(); }
		//for Map
		Tile* &operator [](const int &x) { return Map[x]; } //UNSAFE: well, [y] anyway may work improperly. So I don't want to mess with X
		Tile &operator[](const Pos &p); //SAFE
		//for set<Enemy*>
		void addEnemy(Enemy *ne) { Enemies.insert(ne); } //if element is already here, then we just don't insert it. Nu a hule.
		int removeEnemy(Enemy *me); //returns 1 if removed, 0 if not found
		typedef mySet<Enemy*>::iterator iterator;
		iterator begin() { return Enemies.begin(); }
		iterator end() { return Enemies.end(); }
		//destroyers
		void destroyBuilding(Tile &); //returns nothing. May change gamestate.
		void killEnemy(Enemy *); //returns nothing. May change gamestate.
		//auxiliary
		bool checkPos(const Pos &);
		bool allSpawnsEmpty();
		iterator nextReady();
	};
}