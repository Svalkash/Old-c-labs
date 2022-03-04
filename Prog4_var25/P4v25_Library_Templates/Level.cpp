#include "stdafx.h"
#include "Level.h"

using namespace std;

namespace P4v25_T
{
	Level::Level(const std::string &fmap, const std::string &fconfig, const std::string &fsave) :Towers(), Spawns(), Enemies(), configCS(), configTS()
	{
		//READING MAP
		pugi::xml_document doc;
		if (!doc.load_file(fmap.c_str()))
			throw exception("Level:cannot_open_map_file");
		pugi::xml_node map = doc.child("map");
		int tileWidth = map.attribute("tilewidth").as_int();
		int tileHeight = map.attribute("tileheight").as_int();
		sizeX = map.attribute("width").as_int();
		sizeY = map.attribute("height").as_int();
		//creating map
		Map = new Tile*[sizeX]; //new map. If badalloc.. so be it.
		int mc = 0; //memory counter, counts SUCCESSFUL mallocs. 
		try
		{
			for (int i = 0; i < sizeX; ++i)
			{
				Map[i] = new Tile[sizeY];
				++mc;
			}
			pugi::xml_node layer = map.child("layer");
			pugi::xml_node data = layer.child("data");
			string TempLayerString = data.text().as_string();
			string tempStrNum;
			int layerX = 0;
			int layerY = 0;
			int tempType;
			for (char i : TempLayerString) {
				if (i != ',') {
					tempStrNum += i;
				}
				else {
					tempType = atoi(tempStrNum.c_str());
					if ((tempType != 01) && (tempType != 02) && (tempType != 03) && (tempType != 11) && (tempType != 12) && (tempType != 13) && (tempType != 21))
						throw exception("Level:illegal_tile_type");
					if ((tempType == 11) && (tempType == 12) && (tempType == 13) && (tempType == 21)) //buildings always on plains
						Map[layerX][layerY].type = 01;
					Map[layerX][layerY].type = tempType;
					tempStrNum.clear();
					layerX++;
				}
				if (layerX == sizeX){
					layerX = 0;
					layerY++;
				}
			}
			//the last element
			tempType = atoi(tempStrNum.c_str());
			if ((tempType != 01) && (tempType != 02) && (tempType != 03) && (tempType != 11) && (tempType != 12) && (tempType != 13) && (tempType != 21))
				throw exception("Level:illegal_tile_type");
			Map[layerX][layerY].type = tempType;
			//is map full?
			if ((layerX != sizeX - 1) && (layerY != sizeY - 1))
				throw exception("Level:not_enough_map_data");
			//READING CONFIG
			ifstream config_in(fconfig);
			if (!config_in.is_open())
				throw exception("Level:cannot_open_config_file");
			try
			{
				//wall config
				config_in >> wallMaxHP >> wallBuildCost >> wallRepairCost;
				if (wallMaxHP <= 0)
					throw exception("Level:illegal_maximum_wall_HP");
				if (wallBuildCost <= 0)
					throw exception("Level:illegal_maximum_build_cost");
				if (wallRepairCost <= 0)
					throw exception("Level:illegal_maximum_repair_cost");
				//CS
				int csN;
				config_in >> csN;
				if (csN < 1)
					throw exception("Level:illegal_castle_stats_number");
				for (int i = 0; i < csN; ++i)
				{
					double m, r;
					int gs, pr;
					int uc;
					config_in >> gs >> pr >> m >> r >> uc;
					configCS.push_back(CStats(gs, pr, m, r, uc)); //exceptions should be here
				}
				//TS
				int tsN;
				config_in >> tsN;
				if (tsN < 1)
					throw exception("Level:illegal_tower_stats_number");
				for (int i = 0; i < tsN; ++i)
				{
					double dmg;
					int rad, s;
					int uc;
					config_in >> rad >> dmg >> s >> uc;
					configTS.push_back(TStats(rad, dmg, s, uc)); //exceptions should be here
				}
			}
			//closing file ANYWAY
			catch (exception &ex)
			{
				config_in.close();
				throw ex;
			}
			config_in.close();
			//READING SAVE
			ifstream save_in(fsave);
			if (!save_in.is_open())
				throw exception("Level:cannot_open_save_file");
			try
			{
				save_in >> timeG >> gold;
				if (timeG < 0)
					throw exception("Level:illegal_global_time");
				if (gold < 0)
					throw exception("Level:illegal_gold");
				//buildings
				//walls
				int wallN;
				save_in >> wallN;
				if (wallN < 0)
					throw exception("Level:illegal_wall_number");
				for (int i = 0; i < wallN; ++i)
				{
					int x, y;
					double c;
					string name;
					save_in >> name >> x >> y >> c;
					if ((x < 0) || (x >= sizeX) || (y < 0) || (y >= sizeY))
						throw exception("Level:illegal_building_position");
					if (Map[x][y].type != 01)
						throw exception("Level:cannot_build_here");
					Map[x][y].type = 11;
					Map[x][y].b = new B_Wall(x, y, wallMaxHP, c, name); //any other exceptions are inside the constructor.
					//bad_alloc must be catched later
				}
				//towers
				int towerN;
				save_in >> towerN;
				if (towerN < 0)
					throw exception("Level:illegal_tower_number");
				for (int i = 0; i < towerN; ++i)
				{
					int x, y, l;
					double tw;
					string name;
					save_in >> name >> x >> y >> l >> tw;
					if ((x < 0) || (x >= sizeX) || (y < 0) || (y >= sizeY))
						throw exception("Level:illegal_building_position");
					if (Map[x][y].type != 01)
						throw exception("Level:cannot_build_here");
					Map[x][y].type = 12;
					if ((l < 0) || (l >= configTS.size()))
						throw exception("Level:illegal_tower_level"); //need it there because we use it in configTS[l]
					Map[x][y].b = new B_Tower(x, y, l, configTS[l], name, tw); //any other exceptions are inside the constructor.
					Towers.insert(Map[x][y].b); //adding to list
				}
				//castle
				int cx, cy, cl;
				double cHP, ctw;
				string cname;
				save_in >> cname >> cx >> cy >> cl >> cHP >> ctw;
				if ((cx < 0) || (cx >= sizeX) || (cy < 0) || (cy >= sizeY))
					throw exception("Level:illegal_building_position");
				if (Map[cx][cy].type != 01)
					throw exception("Level:cannot_build_here");
				Map[cx][cy].type = 13;
				if ((cl < 0) || (cl >= configCS.size()))
					throw exception("Level:illegal_castle_level"); //need it there because we use it in configTS[l]
				Castle = new B_Castle(cx, cy, cl, configCS[cl], cname, cHP, ctw); //any other exceptions are inside the constructor.
				Map[cx][cy].b = Castle;
				//spawns
				int spawnN;
				save_in >> spawnN;
				if (spawnN < 0)
					throw exception("Level:illegal_spawn_number");
				for (int i = 0; i < spawnN; ++i)
				{
					int sx, sy;
					double stw;
					save_in >> sx >> sy >> stw;
					if ((sx < 0) || (sx >= sizeX) || (sy < 0) || (sy >= sizeY))
						throw exception("Level:illegal_building_position");
					if (Map[sx][sy].type != 01)
						throw exception("Level:cannot_build_here");
					Map[sx][sy].type = 21;
					B_Spawn *ptrS = new B_Spawn(sx, sy, stw); //any other exceptions are inside the constructor.
					Map[sx][sy].b = ptrS;
					Spawns.insert(ptrS); //adding to list
					//adding enemies
					int seitemN;
					save_in >> seitemN;
					if (seitemN < 0)
						throw exception("Level:illegal_spawn_item_number");
					for (int j = 0; j < seitemN; ++j)
					{
						/*
						IL-0
						HIL-1
						IH-2
						HIH-3
						A-4
						HA-5
						*/
						int etype;
						string name;
						double bm, bc, br, bcoeff; //loading without auras: auras can be restored
						int bs;
						int x, y;
						double tw;
						save_in >> etype >> name >> x >> y >> bm >> bc >> br >> bcoeff >> bs >> tw;
						//if ((x < 0) || (x >= sizeX) || (y < 0) || (y >= sizeY))
						//	throw exception("Level:illegal_enemy_position");
						x = sx; //yes, i know that's stupid.
						y = sy;
						tw = 1.0 / bs;
						Enemy *ptrE;
						double am, ar, acoeff, arad;
						int as;
						switch (etype)
						{
						case 0:
							ptrE = new E_InfLight(bm, bc, br, bcoeff, bs, name, x, y, tw);
							break;
						case 2:
							ptrE = new E_InfHeavy(bm, bc, br, bcoeff, bs, name, x, y, tw);
							break;
						case 4:
							ptrE = new E_Aviation(bm, bc, br, bcoeff, bs, name, x, y, tw);
							break;
						case 1:
							save_in >> am >> ar >> acoeff >> as >> arad;
							ptrE = new H_InfLight(bm, bc, br, bcoeff, bs, name, x, y, tw, am, ar, acoeff, as, arad);
							break;
						case 3:
							save_in >> am >> ar >> acoeff >> as >> arad;
							ptrE = new H_InfHeavy(bm, bc, br, bcoeff, bs, name, x, y, tw, am, ar, acoeff, as, arad);
							break;
						case 5:
							save_in >> am >> ar >> acoeff >> as >> arad;
							ptrE = new H_Aviation(bm, bc, br, bcoeff, bs, name, x, y, tw, am, ar, acoeff, as, arad);
							break;
						default:
							throw exception("Level:illegal_enemy_type");
							break;
						}
						//type is OK
						double et; //exit time
						save_in >> et;
						ptrS->addEnemy(ptrE, et); //et<0 checked in constructor
						//No need in clone: we create new and send it.
					}
				}
				//adding "living" enemies
				int enemyN;
				save_in >> enemyN;
				if (enemyN < 0)
					throw exception("Level:illegal_enemy_number");
				for (int i = 0; i < enemyN; ++i)
				{
					/*
					IL-0
					HIL-1
					IH-2
					HIH-3
					A-4
					HA-5
					*/
					int etype;
					string name;
					double bm, bc, br, bcoeff; //loading without auras: auras can be restored
					int bs;
					int x, y;
					double tw;
					save_in >> etype >> name >> x >> y >> bm >> bc >> br >> bcoeff >> bs >> tw;
					if ((x < 0) || (x >= sizeX) || (y < 0) || (y >= sizeY))
						throw exception("Level:illegal_enemy_position");
					Enemy *ptrE;
					double am, ar, acoeff, arad;
					int as;
					switch (etype)
					{
					case 0:
						ptrE = new E_InfLight(bm, bc, br, bcoeff, bs, name, x, y, tw);
						break;
					case 2:
						ptrE = new E_InfHeavy(bm, bc, br, bcoeff, bs, name, x, y, tw);
						break;
					case 4:
						ptrE = new E_Aviation(bm, bc, br, bcoeff, bs, name, x, y, tw);
						break;
					case 1:
						save_in >> am >> ar >> acoeff >> as >> arad;
						ptrE = new H_InfLight(bm, bc, br, bcoeff, bs, name, x, y, tw, am, ar, acoeff, as, arad);
						break;
					case 3:
						save_in >> am >> ar >> acoeff >> as >> arad;
						ptrE = new H_InfHeavy(bm, bc, br, bcoeff, bs, name, x, y, tw, am, ar, acoeff, as, arad);
						break;
					case 5:
						save_in >> am >> ar >> acoeff >> as >> arad;
						ptrE = new H_Aviation(bm, bc, br, bcoeff, bs, name, x, y, tw, am, ar, acoeff, as, arad);
						break;
					default:
						throw exception("Level:illegal_enemy_type");
						break;
					}
					Enemies.insert(ptrE);
				}
			}
			//closing file ANYWAY
			catch (exception &ex)
			{
				save_in.close();
				throw ex;
			}
			save_in.close();
			//everything added. Doing basic algorithms, checking critical situations...
			if (!pathfind())
				throw exception("Level:illegal_layout,pathfind_is_impossible");
			for (iterator it = begin(); it != end(); ++it)
				(**it).auraCheck(*this); //checking auras for everyone
			if (Castle->getHP() == 0)
				gamestate = -1; //impossible: it's checked in constructor.
			else if (Enemies.size() && allSpawnsEmpty())
				gamestate = 1; //possible
			else gamestate = 0;
		}
		catch (exception &ex) //badalloc here too? CHECK IT!!!!1!1!!!
		{
			for (int i = 0; i < mc; ++i)
				delete[] Map[i]; //need to delete allocated memory
			delete[] Map;
			for (mySet<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
				delete (*it);
			//Will other containersdestruct themselves without my help?
			throw ex; //throwit "further")) TODO: it works?
		}
	}

	Level::~Level()
	{
		//configCS, configTS - local destructors.
		//mySet<Building*> - stores pointers, no need to mess with it
		//Buildings - inside tiles, tiles MUST delete them
		//Map - matrix. Need to help.
		if (Map)
			for (int i = 0; i < sizeX; ++i)
				delete[] Map[i];
		delete[] Map;
		//mySet<Enemy*> - can't delete pointers, ducking STL/template. Let's help it.
		for (mySet<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
			delete (*it);
		//list of ptrs will be deleted by destructor
	}

	Level::Level(const Level &b) :configCS(b.configCS), configTS(b.configTS), wallMaxHP(b.wallMaxHP), wallBuildCost(b.wallBuildCost), wallRepairCost(b.wallRepairCost), timeG(b.timeG), timeQ(b.timeQ), gold(b.gold), sizeX(b.sizeX), sizeY(b.sizeY), Enemies(), Towers(), Spawns(), gamestate(b.gamestate)
	{
		//only map is left
		Map = new Tile*[b.sizeX]; //new map. If badalloc.. so be it.
		int mc = 0, ec = 0; //memory counter, counts SUCCESSFUL mallocs. 
		try
		{
			//copying map
			for (int i = 0; i < b.sizeX; ++i)
			{
				Map[i] = new Tile[b.sizeY];
				++mc;
			}
			for (int x = 0; x < sizeX; ++x)
				for (int y = 0; y < sizeY; ++y)
					Map[x][y] = b.Map[x][y]; //necessary because I need to get .b later
			//cloning enemies
			for (mySet<Enemy*>::const_iterator it = b.Enemies.begin(); it != b.Enemies.end(); ++it)
				Enemies.insert((*it)->clone());
			//creating NEW links for the buildings
			/*
			1. Getting position of the building
			2. Finding this position in new map
			3. Extracting the pointer
			*/
			Castle = static_cast<B_Castle*>(Map[(b.Castle->getPos()).x][(b.Castle->getPos()).y].b);
			for (mySet<Building*>::const_iterator it = b.Towers.begin(); it != b.Towers.end(); ++it)
				Towers.insert(Map[((*it)->getPos()).x][((*it)->getPos()).y].b);
			for (mySet<Building*>::const_iterator it = b.Spawns.begin(); it != b.Spawns.end(); ++it)
				Spawns.insert(Map[((*it)->getPos()).x][((*it)->getPos()).y].b);
		}
		catch (bad_alloc &ba)
		{
			for (int i = 0; i < mc; ++i)
				delete[] Map[i]; //need to delete allocated memory
			delete[] Map;
			for (mySet<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
				delete (*it);
			//Will other destruct themselves?
			throw ba; //throwit "further")) TODO: it works?
		}
	}

	Level::Level(Level &&b) :configCS(b.configCS), configTS(b.configTS), wallMaxHP(b.wallMaxHP), wallBuildCost(b.wallBuildCost), wallRepairCost(b.wallRepairCost), timeG(b.timeG), timeQ(b.timeQ), gold(b.gold), sizeX(b.sizeX), sizeY(b.sizeY), Enemies(b.Enemies), Castle(b.Castle), Towers(b.Towers), Spawns(b.Spawns), gamestate(b.gamestate)
	{
		//only map is left
		Map = b.Map;
		b.Map = nullptr;
		//Not sure if enemies are working right.
		//DOES MOVING CONSTRUCTOR CALL MOVING CONSTRUCTORS OF THE MEMBERS?
	}

	Level &Level::operator =(const Level &b)
	{
		//dynamic first: we must avoid class corruption
		Tile **nm = new Tile*[b.sizeX]; //new map. If badalloc.. so be it.
		mySet<Enemy*> ne;
		mySet<Building*> ns, nt;
		B_Castle *nc;
		int mc = 0, ec = 0; //memory counter, counts SUCCESSFUL mallocs. 
		try
		{
			//Map
			for (int i = 0; i < b.sizeX; ++i)
			{
				nm[i] = new Tile[b.sizeY];
				++mc;
			}
			for (int x = 0; x < sizeX; ++x)
				for (int y = 0; y < sizeY; ++y)
					nm[x][y] = b.Map[x][y];
			//Enemies
			for (mySet<Enemy*>::const_iterator it = b.Enemies.begin(); it != b.Enemies.end(); ++it)
				ne.insert((*it)->clone());
			//Buildings
			nc = static_cast<B_Castle*>(nm[(b.Castle->getPos()).x][(b.Castle->getPos()).y].b);
			for (mySet<Building*>::const_iterator it = b.Towers.begin(); it != b.Towers.end(); ++it)
				nt.insert(nm[((*it)->getPos()).x][((*it)->getPos()).y].b);
			for (mySet<Building*>::const_iterator it = b.Spawns.begin(); it != b.Spawns.end(); ++it)
				ns.insert(nm[((*it)->getPos()).x][((*it)->getPos()).y].b);
		}
		catch (bad_alloc &ba)
		{
			for (int i = 0; i < mc; ++i)
				delete[] nm[i]; //need to delete allocated memory
			delete[] nm;
			for (mySet<Enemy*>::iterator it = ne.begin(); it != ne.end(); ++it)
				delete (*it);
			throw ba; //throwit "further")) TODO: it works?
		}
		//everything created successfully. Delete old
		for (int i = 0; i < sizeX; i++)
			delete[] Map[i];
		delete[]Map;
		for (mySet<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
			delete (*it);
		//Copy new
		Map = nm;
		Enemies = ne;
		Castle = nc;
		Towers = nt;
		Spawns = ns;
		//others
		configCS = b.configCS;
		configTS = b.configTS;
		wallMaxHP = b.wallMaxHP;
		wallBuildCost = b.wallBuildCost;
		wallRepairCost = b.wallRepairCost;
		timeG = b.timeG;
		timeQ = b.timeQ;
		gold = b.gold;
		sizeX = b.sizeX;
		sizeY = b.sizeY;
		gamestate = b.gamestate;
		return *this;
	}

	Level &Level::operator =(Level &&b)
	{
		//delete current
		for (int i = 0; i < sizeX; i++)
			delete[] Map[i];
		delete[]Map;
		for (mySet<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
			delete (*it);
		//replace
		Map = b.Map;
		b.Map = nullptr;
		//others
		configCS = b.configCS;
		configTS = b.configTS;
		wallMaxHP = b.wallMaxHP;
		wallBuildCost = b.wallBuildCost;
		wallRepairCost = b.wallRepairCost;
		timeG = b.timeG;
		timeQ = b.timeQ;
		gold = b.gold;
		sizeX = b.sizeX;
		sizeY = b.sizeY;
		Enemies = b.Enemies;
		Castle = b.Castle;
		Towers = b.Towers;
		Spawns = b.Spawns;
		gamestate = b.gamestate;
		//IS EVERYTHING ALRIGHT?
		return *this;
	}

	bool Level::save(const std::string &fsave)
	{
		ofstream save_out(fsave, ostream::trunc);
		if (!save_out.is_open())
			return 0;
		try
		{
			save_out << timeG << endl;
			save_out << gold << endl;
			//buildings
			//walls
			//count walls number, cause we know nothing about them
			int wallN = 0;
			for (int px = 0; px < sizeX; ++px)
				for (int py = 0; py < sizeX; ++py)
					if (Map[px][py].type == 11)
						++wallN;
			save_out << wallN << endl;
			//writing walls
			for (int px = 0; px < sizeX; ++px)
				for (int py = 0; py < sizeX; ++py)
					if (Map[px][py].type == 11)
						save_out << (*static_cast<B_Wall*>(Map[px][py].b)) << endl;
			//writing towers
			save_out << Towers.size() << endl; //towerN
			for (mySet<Building*>::iterator it = Towers.begin(); it != Towers.end(); ++it)
				save_out << (*static_cast<B_Tower*>(*it)) << endl;
			//writing castle
			//castle
			save_out << (*Castle) << endl;
			//writing spawns
			save_out << Spawns.size() << endl; //spawnN
			for (mySet<Building*>::iterator it = Spawns.begin(); it != Spawns.end(); ++it)
				save_out << (*static_cast<B_Spawn*>(*it)) << endl;
			//writing "living" enemies
			save_out << Enemies.size() << endl;
			for (mySet<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
				save_out << (**it) << endl;
			//well done!
		}
		//closing file ANYWAY
		catch (exception &ex)
		{
			cout << "Level:game_cannot_be_saved:" << ex.what() << endl;
			save_out.close();
			return 0;
		}
		save_out.close();
		return 0;
	}
	
	double Level::calcTimeQ()
	{
		//castle
		double tQ = (*Castle).remainingTime();
		double ntQ;
		//towers
		for (mySet<Building*>::iterator it = Towers.begin(); it != Towers.end(); ++it)
		{
			B_Tower *ptr = static_cast<B_Tower*>(*it);
			ntQ = (*ptr).remainingTime();
			if ((ntQ) && (ntQ < tQ)) //check for ntQ != 0 (0 if building is disabled/ready)
				tQ = ntQ;
		}
		//spawns
		for (mySet<Building*>::iterator it = Spawns.begin(); it != Spawns.end(); ++it)
		{
			B_Spawn *ptr = static_cast<B_Spawn*>(*it);
			ntQ = (*ptr).remainingTime();
			if ((ntQ) && (ntQ < tQ)) //check for ntQ != 0 (0 if building is disabled/ready)
				tQ = ntQ;
		}
		//enemies
		for (iterator it = begin(); it != end(); ++it)
		{
			ntQ = (**it).remainingTime();
			if ((ntQ) && (ntQ < tQ)) //check for ntQ != 0 (0 if building is disabled/ready)
				tQ = ntQ;
		}
		timeQ = tQ;
		return tQ;
	}
	
	int Level::turn()
	{
		try
		{
			//I want it to be here.
			if (gamestate)
				return gamestate;
			//for casts
			B_Spawn *ptrS;
			B_Tower *ptrT;
			//calculating time quant
			calcTimeQ();
			//adding it to current time
			timeG += timeQ;
			//Castle regens
			Castle->regen(timeQ);
			//Castle tries to give gold
			Castle->addTW(timeQ);
			if (Castle->isReady())
				gold += Castle->giveMoney();
			//releasing enemies from spawns (if any ready)
			for (mySet<Building*>::iterator it = Spawns.begin(); it != Spawns.end(); ++it)
			{
				ptrS = static_cast<B_Spawn*>(*it);
				ptrS->addTW(timeQ);
				//release already CONTAINS isReady (otherwise it'll be just a waste of time)
				while (ptrS->release(*this)) //release returns 0 if there's no ready enemies. Or returns 1 and releases. So...)
					; //need many iterations because of weird multimap<double, Enemy*>-erase mechanics. Can change in STL version.
			}
			//let the enemies move! adding timeQ
			for (iterator it = begin(); it != end(); ++it)
				(*it)->addTW(timeQ);
			//regen
			for (iterator it = begin(); it != end(); ++it)
				(*it)->regen(timeQ);
			iterator movingEn = nextReady();
			while (movingEn != end())
			{
				if ((*movingEn)->move(*this)) //Enemy moved. Returned 1 if the enemy dies
					killEnemy(*movingEn);
				//But what if it was the last enemy OR/and it has destroyed the castle?
				if (gamestate)
					return gamestate; //no need to continue.
				//next iteration
				movingEn = nextReady();
			}
			//now towers
			for (mySet<Building*>::iterator it = Towers.begin(); it != Towers.end(); ++it)
			{
				ptrT = static_cast<B_Tower*>(*it);
				ptrT->addTW(timeQ);
				if (ptrT->isReady())
				{
					ptrT->shoot(*this);
					//what if it has killed the last enemy?
					if (gamestate)
						return gamestate; //no need to continue.
				}
			}
			return 0; //if gamestate!=0, MUST return smth before.
		}
		catch (exception &ex)
		{
			string crashSaveFname = CRASH_SAVE_FILE; //saves game before crash
			save(crashSaveFname);
			throw ex;
		}
	}

	Level::iterator Level::nextReady()
	{
		for (iterator it = begin(); it != end(); ++it)
			if ((*it)->isReady())
				return it;
		return end();
	}

	bool Level::pathfind()
	{
		//init
		for (int x = 0; x < sizeX; ++x)
			for (int y = 0; y < sizeY; ++y)
			{
				Map[x][y].distL = -1;
				Map[x][y].distH = -1;
				Map[x][y].distA = -1;
				Map[x][y].nextA = (x, y); //uhm.. works?
				Map[x][y].nextL = (x, y);
				Map[x][y].nextH = (x, y);
			}
		//start
		Pos start = Castle->getPos();
		//mySet castle dist to 0
		(*this)[start].distL = 0;
		(*this)[start].distH = 0;
		(*this)[start].distA = 0;
		if (start.x > 0)
			pf_visit(Pos(start.x - 1, start.y), start);
		if (start.x < sizeX - 1)
			pf_visit(Pos(start.x + 1, start.y), start);
		if (start.y > 0)
			pf_visit(Pos(start.x, start.y - 1), start);
		if (start.y < sizeY - 1)
			pf_visit(Pos(start.x, start.y + 1), start);
		//check
		for (mySet<Building*>::iterator it = Spawns.begin(); it != Spawns.end(); ++it)
			if ((*this)[(**it).getPos()].distL == -1) //hmm.. but what if I remove (*this)?
				return 0;
		//Enemies still can be trapped in narrow corridors using walls. It's not bug, it's a feature!
		return 1;
	}
	
	void Level::pf_visit(const Pos &curP, const Pos &prevP)
	{
		//EVEN FOR CASTLE increase distance
		/*
		If prev was unreachable, its dist == -1. Else we can find a path through it.
		So, we need:
		a. prev was REACHABLE
		b1. cur wasn't reached BEFORE.
		b2. Distance was LONGER
		*/
		Tile &cur = (*this)[curP];
		Tile &prev = (*this)[prevP];
		//changing distances
		bool changeF = 0; //change flag
		if ((prev.distL != -1) && ((cur.type == 01) || (cur.type == 21)) && ((cur.distL == -1) || (cur.distL > prev.distL + 1))) //light - only plains and spawns
		{
			cur.distL = prev.distL + 1;
			cur.nextL = prevP;
			changeF = 1;
		}
		if ((prev.distH != -1) && ((cur.type == 01) || (cur.type == 11) || (cur.type == 21)) && ((cur.distH == -1) || (cur.distH > prev.distH + 1))) //heavy - can destroy walls and spawns
		{
			cur.distH = prev.distH + 1;
			cur.nextH = prevP;
			changeF = 1;
		}
		if ((prev.distA != -1) && ((cur.type == 01) || (cur.type == 02) || (cur.type == 11) || (cur.type == 12) || (cur.type == 21)) && ((cur.distA == -1) || (cur.distA > prev.distA + 1))) //avia - plain, hill, wall, tower, spawn
		{
			cur.distA = prev.distA + 1;
			cur.nextA = prevP;
			changeF = 1;
		}
		if (!changeF)
			return; //if nothing changed, stop branching.
		//Further.
		if ((curP.x > 0) && (curP.x - 1 != prevP.x))
			pf_visit(Pos(curP.x - 1, curP.y), curP);
		if ((curP.x < sizeX - 1) && (curP.x + 1 != prevP.x))
			pf_visit(Pos(curP.x + 1, curP.y), curP);
		if ((curP.y > 0) && (curP.y - 1 != prevP.y))
			pf_visit(Pos(curP.x, curP.y - 1), curP);
		if ((curP.y < sizeY - 1) && (curP.y + 1 != prevP.y))
			pf_visit(Pos(curP.x, curP.y + 1), curP);
	}
	
	int Level::build(const Pos &p, const int &type, const string &name)
	{
		//is position in the range?
		if (!checkPos(p))
			return 0;
		Tile &t = (*this)[p];
		//is position empty?
		if (t.type != 01)
			return -1;
		//is pos. empty FROM ENEMIES?!
		for (iterator it = begin(); it != end(); ++it)
			if ((**it).getPos() == p)
				return -4;
		//choosing correct type;
		if (type == 11)
		{
			//is gold enough?
			if (gold < wallBuildCost)
				return -2;
			//placing.. Can LInf pass to the castle?
			Tile backup(t); //backup
			t.b = new B_Wall(p, wallMaxHP, wallMaxHP, name); //is that right? creates new building CAN THROW BADALLOC
			t.type = type; //change type
			//there's no "wall list"
			if (!pathfind()) //test if path is available
			{
				delete t.b;
				t.b = nullptr;
				t = backup;
				pathfind(); //change back pathfind values
				return -3;
			}
			gold -= wallBuildCost;
			return 1;
		}
		if (type == 12)
		{
			//is gold enough?
			if (gold < configTS[0].upgradeCost)
				return -2;
			//placing.. Can LInf pass to the castle?
			Tile backup(t); //backup
			t.b = new B_Tower(p, 0, configTS[0], name, 1.0 / configTS[0].shootS); //is that right? creates new building CAN THROW BADALLOC
			t.type = type; //change type
			//there's no "wall list"
			if (!pathfind()) //test if path is available
			{
				delete t.b;
				t.b = nullptr;
				t = backup;
				pathfind(); //change back pathfind values
				return -3;
			}
			Towers.insert(t.b);
			gold -= configTS[0].upgradeCost;
			return 1;
		}
		throw exception("Level:build:illegal_type");
		return 0;
	}

	int Level::upgrade(const Pos &p)
	{
		//is position in the range?
		if (!checkPos(p))
			return 0;
		Tile &t = (*this)[p];
		//is there any upgradable building?
		if (t.type == 12)
		{
			B_Tower *bptr = static_cast<B_Tower*>(t.b);
			int curLvl = bptr->getLvl();
			//is there any UPGRADE?
			if (configTS.size() == curLvl + 1) //size == maxlevel + 1, (0,1,2,3) size == 4, so if size==lvl+1, then we ALREADY reached maxLevel
				return -3;
			//is gold enough?
			if (gold < configTS[curLvl + 1].upgradeCost)
				return -2;
			//well, mySet new stats and lvl
			gold -= configTS[curLvl + 1].upgradeCost;
			bptr->setStats(configTS[curLvl + 1]);
			bptr->setLvl(curLvl + 1);
			return 1;
		}
		if (t.type == 13)
		{
			B_Castle *bptr = static_cast<B_Castle*>(t.b);
			int curLvl = bptr->getLvl();
			//is there any UPGRADE?
			if (configCS.size() == curLvl + 1) //size == maxlevel + 1, (0,1,2,3) size == 4, so if size==lvl+1, then we ALREADY reached maxLevel
				return -3;
			//is gold enough?
			if (gold < configCS[curLvl + 1].upgradeCost)
				return -2;
			//well, mySet new stats and lvl
			gold -= configTS[curLvl + 1].upgradeCost;
			bptr->setStats(configCS[curLvl + 1]);
			bptr->setLvl(curLvl + 1);
			return 1;
		}
		return -1;
	}

	int Level::repair(const Pos &p, const double &healHP)
	{
		//is heal positive?)
		if (healHP < 0)
			return -3;
		//is position in the range?
		if (!checkPos(p))
			return 0;
		Tile &t = (*this)[p];
		//is there any wall?
		if (t.type != 11)
			return -1;
		//is gold enough?
		B_Wall *bptr = static_cast<B_Wall*>(t.b);
		double healValue = fmin(healHP, wallMaxHP - bptr->getHP());
		int goldNeeded = ceil(healValue * wallRepairCost);
		if (gold < goldNeeded)
			return -2;
		gold -= goldNeeded;
		bptr->heal(healValue);
		return 1;
	}

	ostream &operator <<(ostream &c, Level &L)
	{
		int a, l, h; //0 - no, 1 - normal, 2 - hero
		for (int i = 0; i < L.sizeY; ++i)
			c << "_";
		c << endl;
		cout << " |";
		for (int x = 0; x < L.sizeX; ++x)
			cout << x % 10;
		cout << endl;
		for (int y = 0; y < L.sizeY; ++y)
		{
			cout << y % 10 << '|';
			for (int x = 0; x < L.sizeX; ++x)
			{
				a = 0;
				l = 0;
				h = 0;
				for (Level::iterator it = L.begin(); it != L.end(); ++it)
				{
					if (((**it).getPos().x == x) && ((**it).getPos().y == y))
					{
						if (dynamic_cast<E_Aviation*>(*it))
							if (dynamic_cast<H_Aviation*>(*it))
								a = 2;
							else
								a = 1;
						if (dynamic_cast<E_InfLight*>(*it))
							if (dynamic_cast<H_InfLight*>(*it))
								l = 2;
							else
								l = 1;
						if (dynamic_cast<E_InfHeavy*>(*it))
							if (dynamic_cast<H_InfHeavy*>(*it))
								h = 2;
							else
								h = 1;
					}
				}
				//output priority: A>H>L>a>h>l>any type
				if (a == 2)
					c << color<LightRed, Black> << "A" << color;
				else if (h == 2)
					c << color<LightRed, Black> << "H" << color;
				else if (l == 2)
					c << color<LightRed, Black> << "L" << color;
				else if (a == 1)
					c << color<LightRed, Black> << "a" << color;
				else if (h == 1)
					c << color<LightRed, Black> << "h" << color;
				else if (l == 1)
					c << color<LightRed, Black> << "l" << color;
				else switch (L.Map[x][y].type)
				{
				case 01:
					c << color<LightGreen, Black> << "." << color;
					break;
				case 02:
					c << color<LightBlue, Black> << "~" << color;
					break;
				case 03:
					c << color<Brown, Black> << "^" << color;
					break;
				case 11:
					c << color<DarkGray, Black> << "#" << color;
					break;
				case 12:
					c << color<White, Black> << static_cast<B_Tower*>(L.Map[x][y].b)->getLvl() << color;
					break;
				case 13:
					c << color<LightMagenta, Black> << "@" << color;
					break;
				case 21:
					c << color<Yellow, Black> << "*" << color;
					break;
				default:
					throw exception("Level:print:wtf?");
					break;
				}
			}
			c << endl;
		}
		for (int i = 0; i < L.sizeY; ++i)
			c << "_";
		c << endl;
		c << "Gold: " << L.gold << endl;
		for (int i = 0; i < L.sizeY; ++i)
			c << "_";
		c << endl;
		return c;
	}

	const TStats Level::getTS(const int &lvl)
	{
		if ((lvl < 0) || (lvl >= configTS.size()))
			throw exception("Level:getTS:illegal_level");
		return configTS[lvl];
	}

	const CStats Level::getCS(const int &lvl)
	{
		if ((lvl < 0) || (lvl >= configCS.size()))
			throw exception("Level:getCS:illegal_level");
		return configCS[lvl];
	}

	Tile &Level::operator[](const Pos &p)
	{
		if ((p.x < 0) || (p.x >= sizeX) || (p.y < 0) || (p.y >= sizeY))
			throw exception("Level:operator[]:illegal_position");
		return Map[p.x][p.y];
	}

	void Level::destroyBuilding(Tile &t)
	{
		if (t.type == 13)
		{
			cout << "castle" << static_cast<B_Castle*>(t.b)->getName() << " is destroyed" << endl;
			gamestate = -1;
			return;
		}
		if (t.type != 11)
			throw exception("Level:destroyBuilding:illegal_type");
		cout << "wall" << static_cast<B_Wall*>(t.b)->getName() << " is destroyed" << endl;
		delete t.b;
		t.b = nullptr;
		t.type = 01; //buildings could be only on plains
		//IF NOT CASTLE: layout changed => pathfind
		pathfind();
	}

	void Level::killEnemy(Enemy *e)
	{
		cout << e->getName() << " dies" << endl;
		bool f = 0;
		if (dynamic_cast<Hero*>(e))
			f = 1;
		removeEnemy(e);
		if (f)
			for (iterator it = begin(); it != end(); ++it)
				(**it).auraCheck(*this); //hero can change everything
		if ((Enemies.size() == 0) && allSpawnsEmpty())
			if (gamestate == 0)
				gamestate = 1; //for situations, when BOTH enemy and castle die.
	}

	int Level::removeEnemy(Enemy *me)
	{
		delete me;
		return Enemies.erase(me);
	}

	bool Level::checkPos(const Pos &p)
	{
		if (p.x < 0)
			return 0;
		if (p.x >= sizeX)
			return 0;
		if (p.y < 0)
			return 0;
		if (p.y >= sizeY)
			return 0;
		return 1;
	}

	bool Level::allSpawnsEmpty()
	{
		for (mySet<Building*>::iterator it = Spawns.begin(); it != Spawns.end(); ++it)
			if (static_cast<B_Spawn*>(*it)->size() != 0)
				return 0;
		return 1;
	}
}