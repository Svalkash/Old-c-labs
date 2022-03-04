#include "stdafx.h"
#include "Level.h"

using namespace std;

namespace P4v25_SFML
{
	Level::Level(const std::string &fmap, const std::string &fconfig, const std::string &fsave) :Towers(), Spawns(), Enemies(), configCS(), configTS()
	{
		//READING MAP
		string foldermap = getfolder(fmap);
		pugi::xml_document doc;
		if (!doc.load_file(fmap.c_str()))
			throw exception("Level:cannot_open_map");
		pugi::xml_node map = doc.child("map");
		tileWidth = map.attribute("tilewidth").as_int();
		int tileHeight = map.attribute("tileheight").as_int();
		if (tileHeight != tileWidth)
			throw exception("Level:tile_height_is_not_equal_to_tile_width");
		sizeX = map.attribute("width").as_int();
		sizeY = map.attribute("height").as_int();
		//loading tileset
		pugi::xml_node maptileset = map.child("tileset");
		string source = maptileset.attribute("source").as_string();
		string ftileset = foldermap + source;
		string folderts = getfolder(ftileset);
		pugi::xml_document tsdoc;
		if (!tsdoc.load_file(ftileset.c_str()))
			throw exception("Level:cannot_open_tileset");
		pugi::xml_node tstileset = tsdoc.child("tileset");
		int tstileWidth = tstileset.attribute("tilewidth").as_int();
		int tstileHeight = tstileset.attribute("tileheight").as_int();
		if ((tstileWidth != tileWidth) || (tstileHeight != tileWidth))
			throw exception("Level:different_tileWidth/tileHeight_values");
		int tilecount = tstileset.attribute("tilecount").as_int();
		pugi::xml_node tile = tstileset.child("tile");
		while (tile)
		{
			int id = tile.attribute("id").as_int();
			pugi::xml_node image = tile.child("image");
			int width = image.attribute("width").as_int();
			int height = image.attribute("height").as_int();
			if ((width != tileWidth) || (height != tileWidth))
				throw exception("Level:wrong_image_resolution");
			string imgsource = folderts + image.attribute("source").as_string();
			sf::Texture *tempTexture = new sf::Texture;
			if (!(*tempTexture).loadFromFile(imgsource))
				throw exception("Level:cannot_load_texture");
			tileset.insert(make_pair(id, tempTexture));
			tile = tile.next_sibling("tile");
		}
		//check if nothing is missed
		try
		{
			for (int i = 0; i < tilesetIDnumber; ++i)
				tileset.at(tilesetIDs[i]);
		}
		catch (std::out_of_range our)
		{
			throw exception("Level:some_textures_is_missing"); //LOL
		}
		//set repeated window and panel textures
		(*tileset[1001]).setRepeated(true);
		(*tileset[1002]).setRepeated(true);
		//loading map
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
					tempType -= 1; //because Tiled works too weird
					if ((tempType != 01) && (tempType != 02) && (tempType != 03) && (tempType != 11) && (tempType != 12) && (tempType != 13) && (tempType != 21))
						throw exception("Level:illegal_tile_type");
					if ((tempType == 11) || (tempType == 12) || (tempType == 13) || (tempType == 21)) //buildings always on plains
						Map[layerX][layerY].type = 01;
					else
						Map[layerX][layerY].type = tempType;
					Map[layerX][layerY].sprite.setTexture(*tileset[Map[layerX][layerY].type]); //setting texture
					Map[layerX][layerY].sprite.setPosition(layerX * tileWidth, layerY * tileWidth); //setting position
					//origin = 0,0.
					tempStrNum.clear();
					layerX++;
				}
				if (layerX == sizeX){
					layerX = 0;
					layerY++;
				}
			}
			//the last element!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			tempType = atoi(tempStrNum.c_str());
			tempType -= 1; //because Tiled works too weird
			if ((tempType != 01) && (tempType != 02) && (tempType != 03) && (tempType != 11) && (tempType != 12) && (tempType != 13) && (tempType != 21))
				throw exception("Level:illegal_tile_type");
			if ((tempType == 11) || (tempType == 12) || (tempType == 13) || (tempType == 21)) //buildings always on plains
				Map[layerX][layerY].type = 01;
			else
				Map[layerX][layerY].type = tempType;
			Map[layerX][layerY].sprite.setTexture(*tileset[Map[layerX][layerY].type]); //setting texture
			Map[layerX][layerY].sprite.setPosition(layerX * tileWidth, layerY * tileWidth); //setting position
			//origin = 0,0.
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
					double gs;
					int pr;
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
					int rad;
					double s;
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
				save_in >> gold;
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
					B_Wall *ptrW = new B_Wall(x, y, wallMaxHP, c, name); //any other exceptions are inside the constructor.
					Map[x][y].b = ptrW;
					Map[x][y].sprite.setTexture(*tileset[Map[x][y].type]); //setting texture. POSITION IS ALREADY SET
					//HPbar
					ptrW->HPbarBase.setPosition(x * tileWidth, y * tileWidth + (1 - BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
					ptrW->HPbarBase.setFillColor(BASE_BAR_COLOR);
					ptrW->HPbarBase.setSize(sf::Vector2f(tileWidth, BAR_HEIGHT_COEFF * tileWidth));
					ptrW->HPbar.setPosition(x * tileWidth, y * tileWidth + (1 - BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
					ptrW->HPbar.setFillColor(HP_BAR_COLOR);
					ptrW->setHPBarSize();
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
					B_Tower *ptrT = new B_Tower(x, y, l, configTS[l], name, tw); //any other exceptions are inside the constructor.
					Map[x][y].b = ptrT;
					Towers.insert(Map[x][y].b); //adding to list
					Map[x][y].sprite.setTexture(*tileset[Map[x][y].type]); //setting texture
					//adding "top" sprite
					const int towerTopType = 201; //for... search? Don't know. Let it be there.
					ptrT->topSprite.setTexture(*tileset[towerTopType]);
					ptrT->topSprite.setPosition(x * tileWidth + tileWidth / 2, y * tileWidth + tileWidth / 2);
					ptrT->topSprite.setOrigin(tileWidth / 2, tileWidth / 2);
					//RELOADbar
					ptrT->RELOADbarBase.setPosition(x * tileWidth, y * tileWidth + (1 - BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
					ptrT->RELOADbarBase.setFillColor(BASE_BAR_COLOR);
					ptrT->RELOADbarBase.setSize(sf::Vector2f(tileWidth, BAR_HEIGHT_COEFF * tileWidth));
					ptrT->RELOADbar.setPosition(x * tileWidth, y * tileWidth + (1 - BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
					ptrT->RELOADbar.setFillColor(RELOAD_BAR_COLOR);
					ptrT->setRELOADbarSize();
					//range circle
					ptrT->rangeCircle.setPosition(x * tileWidth + tileWidth / 2, y * tileWidth + tileWidth / 2);
					ptrT->rangeCircle.setFillColor(RANGE_COLOR);
					ptrT->rangeCircle.setOutlineThickness(RANGE_OUTLINE_THICKNESS);
					ptrT->rangeCircle.setOutlineColor(RANGE_OUTLINE_COLOR);
					ptrT->setRangeCircleSize(*this); //origin is set here too
					//shootline
					ptrT->shootLine.setPosition(x * tileWidth + tileWidth / 2, y * tileWidth + tileWidth / 2);
					ptrT->shootLine.setOrigin(0, SHOOTLINE_THICKNESS / 2);
					ptrT->shootLine.setFillColor(SHOOTLINE_COLOR);
					ptrT->shootLine.setSize(sf::Vector2f(0, SHOOTLINE_THICKNESS));
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
				Map[cx][cy].sprite.setTexture(*tileset[Map[cx][cy].type]); //setting texture
				//HPbar
				Castle->HPbarBase.setPosition(cx * tileWidth, cy * tileWidth + (1 - BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
				Castle->HPbarBase.setFillColor(BASE_BAR_COLOR);
				Castle->HPbarBase.setSize(sf::Vector2f(tileWidth, BAR_HEIGHT_COEFF * tileWidth));
				Castle->HPbar.setPosition(cx * tileWidth, cy * tileWidth + (1 - BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
				Castle->HPbar.setFillColor(HP_BAR_COLOR);
				Castle->setHPbarSize();
				//GOLDbar
				Castle->GOLDbarBase.setPosition(cx * tileWidth, cy * tileWidth + (1 - 2 * BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
				Castle->GOLDbarBase.setFillColor(BASE_BAR_COLOR);
				Castle->GOLDbarBase.setSize(sf::Vector2f(tileWidth, BAR_HEIGHT_COEFF * tileWidth));
				Castle->GOLDbar.setPosition(cx * tileWidth, cy * tileWidth + (1 - 2 * BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
				Castle->GOLDbar.setFillColor(GOLD_BAR_COLOR);
				Castle->setGOLDbarSize();
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
					Map[sx][sy].sprite.setTexture(*tileset[Map[sx][sy].type]); //setting texture
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
						double bs;
						double x, y;
						save_in >> etype >> name >> x >> y >> bm >> bc >> br >> bcoeff >> bs;
						//if ((x < 0) || (x >= sizeX) || (y < 0) || (y >= sizeY))
						//	throw exception("Level:illegal_enemy_position");
						Enemy *ptrE;
						double am, ar, acoeff, arad;
						double as;
						switch (etype)
						{
						case 0:
							ptrE = new E_InfLight(bm, bc, br, bcoeff, bs, name, x, y);
							break;
						case 2:
							ptrE = new E_InfHeavy(bm, bc, br, bcoeff, bs, name, x, y);
							break;
						case 4:
							ptrE = new E_Aviation(bm, bc, br, bcoeff, bs, name, x, y);
							break;
						case 1:
							save_in >> am >> ar >> acoeff >> as >> arad;
							ptrE = new H_InfLight(bm, bc, br, bcoeff, bs, name, x, y, am, ar, acoeff, as, arad);
							break;
						case 3:
							save_in >> am >> ar >> acoeff >> as >> arad;
							ptrE = new H_InfHeavy(bm, bc, br, bcoeff, bs, name, x, y, am, ar, acoeff, as, arad);
							break;
						case 5:
							save_in >> am >> ar >> acoeff >> as >> arad;
							ptrE = new H_Aviation(bm, bc, br, bcoeff, bs, name, x, y, am, ar, acoeff, as, arad);
							break;
						default:
							throw exception("Level:illegal_enemy_type");
							break;
						}
						//type is OK
						ptrE->sprite.setOrigin(tileWidth / 2, tileWidth / 2); //assuming that all tileset have the same size
						ptrE->sprite.setPosition(x * tileWidth + tileWidth / 2, y * tileWidth + tileWidth / 2);
					//HPbar
						ptrE->HPbarBase.setFillColor(BASE_BAR_COLOR);
						ptrE->HPbarBase.setSize(sf::Vector2f(tileWidth, BAR_HEIGHT_COEFF * tileWidth));
						ptrE->HPbar.setFillColor(HP_BAR_COLOR);
						ptrE->setHPbarPosition(*this); //works with BOTH bars: base and normal
						ptrE->setHPbarSize();
						//auraCircle
						Hero *ptrH = dynamic_cast<Hero*>(ptrE);
						if (ptrH)
						{
							ptrH->auraCircle.setRadius(ptrH->getAura().getRad() * tileWidth);
							ptrH->auraCircle.setOrigin(ptrH->getAura().getRad() * tileWidth, ptrH->getAura().getRad() * tileWidth);
							ptrH->auraCircle.setFillColor(AURA_COLOR);
							ptrH->auraCircle.setOutlineThickness(AURA_OUTLINE_THICKNESS);
							ptrH->auraCircle.setOutlineColor(AURA_OUTLINE_COLOR);
							ptrH->setAuraCirclePosition();
						}
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
					double bs;
					double x, y;
					save_in >> etype >> name >> x >> y >> bm >> bc >> br >> bcoeff >> bs;
					if ((x < 0) || (x >= sizeX) || (y < 0) || (y >= sizeY))
						throw exception("Level:illegal_enemy_position");
					Enemy *ptrE;
					double am, ar, acoeff, arad;
					double as;
					switch (etype)
					{
					case 0:
						ptrE = new E_InfLight(bm, bc, br, bcoeff, bs, name, x, y);
						break;
					case 2:
						ptrE = new E_InfHeavy(bm, bc, br, bcoeff, bs, name, x, y);
						break;
					case 4:
						ptrE = new E_Aviation(bm, bc, br, bcoeff, bs, name, x, y);
						break;
					case 1:
						save_in >> am >> ar >> acoeff >> as >> arad;
						ptrE = new H_InfLight(bm, bc, br, bcoeff, bs, name, x, y, am, ar, acoeff, as, arad);
						break;
					case 3:
						save_in >> am >> ar >> acoeff >> as >> arad;
						ptrE = new H_InfHeavy(bm, bc, br, bcoeff, bs, name, x, y, am, ar, acoeff, as, arad);
						break;
					case 5:
						save_in >> am >> ar >> acoeff >> as >> arad;
						ptrE = new H_Aviation(bm, bc, br, bcoeff, bs, name, x, y, am, ar, acoeff, as, arad);
						break;
					default:
						throw exception("Level:illegal_enemy_type");
						break;
					}
					//positioning. Texture will be added in findGoal
					ptrE->sprite.setOrigin(tileWidth / 2, tileWidth / 2); //assuming that all tileset have the same size
					ptrE->sprite.setPosition(x * tileWidth + tileWidth / 2, y * tileWidth + tileWidth / 2);
					//HPbar
					ptrE->HPbarBase.setFillColor(BASE_BAR_COLOR);
					ptrE->HPbarBase.setSize(sf::Vector2f(tileWidth, BAR_HEIGHT_COEFF * tileWidth));
					ptrE->HPbar.setFillColor(HP_BAR_COLOR);
					ptrE->setHPbarPosition(*this); //works with BOTH bars: base and normal
					ptrE->setHPbarSize();
					//auraCircle
					Hero *ptrH = dynamic_cast<Hero*>(ptrE);
					if (ptrH)
					{
						ptrH->auraCircle.setRadius(ptrH->getAura().getRad() * tileWidth);
						ptrH->auraCircle.setOrigin(ptrH->getAura().getRad() * tileWidth, ptrH->getAura().getRad() * tileWidth);
						ptrH->auraCircle.setFillColor(AURA_COLOR);
						ptrH->auraCircle.setOutlineThickness(AURA_OUTLINE_THICKNESS);
						ptrH->auraCircle.setOutlineColor(AURA_OUTLINE_COLOR);
						ptrH->setAuraCirclePosition();
					}
					//insert
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
				(**it).findGoal(*this); //finding goals for everyone, setting tileset
			for (iterator it = begin(); it != end(); ++it)
				(**it).auraCheck(*this); //checking auras for everyone
			if (Castle->getHP() == 0)
				gamestate = -1; //impossible: it's checked in constructor.
			else if (!Enemies.size() && allSpawnsEmpty())
				gamestate = 1; //possible
			else gamestate = 0;
		}
		catch (exception &ex) //badalloc here too? CHECK IT!!!!1!1!!!
		{
			for (int i = 0; i < mc; ++i)
				delete[] Map[i]; //need to delete allocated memory
			delete[] Map;
			for (set<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
				delete (*it);
			for (std::map<int, sf::Texture*>::iterator it = tileset.begin(); it != tileset.end(); ++it)
				delete (*it).second;
			//Will other containersdestruct themselves without my help?
			throw ex; //throwit "further")) TODO: it works?
		}
	}

	Level::~Level()
	{
		//configCS, configTS - local destructors.
		//set<Building*> - stores pointers, no need to mess with it
		//Buildings - inside tiles, tiles MUST delete them
		//Map - matrix. Need to help.
		if (Map)
			for (int i = 0; i < sizeX; ++i)
				delete[] Map[i];
		delete[] Map;
		//set<Enemy*> - can't delete pointers, ducking STL/template. Let's help it.
		for (set<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
			delete (*it);
		for (std::map<int, sf::Texture*>::iterator it = tileset.begin(); it != tileset.end(); ++it)
			delete (*it).second;
		//list of ptrs will be deleted by destructor
	}

	Level::Level(const Level &b) :configCS(b.configCS), configTS(b.configTS), wallMaxHP(b.wallMaxHP), wallBuildCost(b.wallBuildCost), wallRepairCost(b.wallRepairCost), timeQ(b.timeQ), gold(b.gold), sizeX(b.sizeX), sizeY(b.sizeY), Enemies(), Towers(), Spawns(), gamestate(b.gamestate), tileset()
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
			for (set<Enemy*>::iterator it = b.Enemies.begin(); it != b.Enemies.end(); ++it)
				Enemies.insert((*it)->clone());
			for (std::map<int, sf::Texture*>::const_iterator it = b.tileset.begin(); it != b.tileset.end(); ++it)
				tileset.insert(make_pair((*it).first, cloneTex((*it).second)));
			//creating NEW links for the buildings
			/*
			1. Getting position of the building
			2. Finding this position in new map
			3. Extracting the pointer
			*/
			Castle = static_cast<B_Castle*>(Map[(b.Castle->getPos()).x][(b.Castle->getPos()).y].b);
			for (set<Building*>::iterator it = b.Towers.begin(); it != b.Towers.end(); ++it)
				Towers.insert(Map[((*it)->getPos()).x][((*it)->getPos()).y].b);
			for (set<Building*>::iterator it = b.Spawns.begin(); it != b.Spawns.end(); ++it)
				Spawns.insert(Map[((*it)->getPos()).x][((*it)->getPos()).y].b);
		}
		catch (bad_alloc &ba)
		{
			for (int i = 0; i < mc; ++i)
				delete[] Map[i]; //need to delete allocated memory
			delete[] Map;
			for (set<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
				delete (*it);
			for (std::map<int, sf::Texture*>::iterator it = tileset.begin(); it != tileset.end(); ++it)
				delete (*it).second;
			//Will other destruct themselves?
			throw ba; //throwit "further")) TODO: it works?
		}
	}

	Level::Level(Level &&b) :configCS(b.configCS), configTS(b.configTS), wallMaxHP(b.wallMaxHP), wallBuildCost(b.wallBuildCost), wallRepairCost(b.wallRepairCost), timeQ(b.timeQ), gold(b.gold), sizeX(b.sizeX), sizeY(b.sizeY), Enemies(b.Enemies), Castle(b.Castle), Towers(b.Towers), Spawns(b.Spawns), gamestate(b.gamestate)
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
		set<Enemy*> ne;
		set<Building*> ns, nt;
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
			for (set<Enemy*>::iterator it = b.Enemies.begin(); it != b.Enemies.end(); ++it)
				ne.insert((*it)->clone());
			for (std::map<int, sf::Texture*>::const_iterator it = b.tileset.begin(); it != b.tileset.end(); ++it)
				tileset.insert(make_pair((*it).first, cloneTex((*it).second)));
			//Buildings
			nc = static_cast<B_Castle*>(nm[(b.Castle->getPos()).x][(b.Castle->getPos()).y].b);
			for (set<Building*>::iterator it = b.Towers.begin(); it != b.Towers.end(); ++it)
				nt.insert(nm[((*it)->getPos()).x][((*it)->getPos()).y].b);
			for (set<Building*>::iterator it = b.Spawns.begin(); it != b.Spawns.end(); ++it)
				ns.insert(nm[((*it)->getPos()).x][((*it)->getPos()).y].b);
		}
		catch (bad_alloc &ba)
		{
			for (std::map<int, sf::Texture*>::iterator it = tileset.begin(); it != tileset.end(); ++it)
				delete (*it).second;
			for (int i = 0; i < mc; ++i)
				delete[] nm[i]; //need to delete allocated memory
			delete[] nm;
			for (set<Enemy*>::iterator it = ne.begin(); it != ne.end(); ++it)
				delete (*it);
			throw ba; //throwit "further")) TODO: it works?
		}
		//everything created successfully. Delete old
		for (int i = 0; i < sizeX; i++)
			delete[] Map[i];
		delete[]Map;
		for (set<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
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
		for (set<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
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

	void Level::save(const std::string &fsave)
	{
		ofstream save_out(fsave, ostream::trunc);
		if (!save_out.is_open())
			throw exception("Level:save:cannot_open/create_save_file");
		try
		{
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
			for (set<Building*>::iterator it = Towers.begin(); it != Towers.end(); ++it)
				save_out << (*static_cast<B_Tower*>(*it)) << endl;
			//writing castle
			//castle
			save_out << (*Castle) << endl;
			//writing spawns
			save_out << Spawns.size() << endl; //spawnN
			for (set<Building*>::iterator it = Spawns.begin(); it != Spawns.end(); ++it)
				save_out << (*static_cast<B_Spawn*>(*it)) << endl;
			//writing "living" enemies
			save_out << Enemies.size() << endl;
			for (set<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it)
				save_out << (**it) << endl;
			//well done!
		}
		//closing file ANYWAY
		catch (exception &ex)
		{
			save_out.close();
			throw ex;
		}
		save_out.close();
	}
	
	double Level::calcTimeQ(const double &maxTQ)
	{
		//castle
		double tQ = (*Castle).remainingTime();
		double ntQ;
		//towers
		for (set<Building*>::iterator it = Towers.begin(); it != Towers.end(); ++it)
		{
			B_Tower *ptr = static_cast<B_Tower*>(*it);
			ntQ = (*ptr).remainingTime();
			if ((ntQ) && (ntQ < tQ)) //check for ntQ != 0 (0 if building is disabled/ready)
				tQ = ntQ;
		}
		//spawns
		for (set<Building*>::iterator it = Spawns.begin(); it != Spawns.end(); ++it)
		{
			B_Spawn *ptr = static_cast<B_Spawn*>(*it);
			ntQ = (*ptr).remainingTime();
			if ((ntQ) && (ntQ < tQ)) //check for ntQ != 0 (0 if building is disabled/ready)
				tQ = ntQ;
		}
		//enemies
		for (iterator it = begin(); it != end(); ++it)
		{
			ntQ = (**it).remainingTime(*this);
			if ((ntQ) && (ntQ < tQ)) //check for ntQ != 0 (0 if building is disabled/ready)
				tQ = ntQ;
		}
		if (maxTQ)
			timeQ = fmin(maxTQ, tQ);
		else
			timeQ = tQ;
		return timeQ;
	}
	
	int Level::turnUntil(const double &extTQ)
	{
		double timeS = 0;
		int rc = 0;
		while ((!rc) && (timeS < extTQ))
		{
			rc = turn(extTQ - timeS);
			timeS += timeQ;
		}
		return rc;
	}

	int Level::turn(const double &maxTQ)
	{
		//I want it to be here.
		if (gamestate)
			return gamestate;
		//for casts
		B_Spawn *ptrS;
		B_Tower *ptrT;
		//calculating time quant
		calcTimeQ(maxTQ);
		//Castle regens
		Castle->regen(timeQ);
		//Castle tries to give gold
		Castle->addTW(timeQ);
		if (Castle->isReady())
			gold += Castle->giveMoney();
		//releasing enemies from spawns (if any ready)
		for (set<Building*>::iterator it = Spawns.begin(); it != Spawns.end(); ++it)
		{
			ptrS = static_cast<B_Spawn*>(*it);
			ptrS->addTW(timeQ);
			//release already CONTAINS isReady (otherwise it'll be just a waste of time)
			while (ptrS->release(*this)) //release returns 0 if there's no ready enemies. Or returns 1 and releases. So...)
				; //need many iterations because of weird multimap<const double, Enemy*>-erase mechanics. Can change in STL version.
		}
		//regen enemies
		for (iterator it = begin(); it != end(); ++it)
			(*it)->regen(timeQ);
		//enemies
		for (iterator it = begin(); it != end();) //TEST, BLYAT. If doesn't work, use ANOTHER erase() form.
		{
			if ((*it)->move(*this)) //Enemy moved. Returned 1 if the enemy dies
				it = killEnemy(it);
			else
				++it;
			//But what if it was the last enemy OR/and it has destroyed the castle?
			if (gamestate)
				return gamestate; //no need to continue.
		}
		//now towers
		for (set<Building*>::iterator it = Towers.begin(); it != Towers.end(); ++it)
		{
			ptrT = static_cast<B_Tower*>(*it);
			ptrT->addTW(timeQ);
			int aimRC = ptrT->aim(*this);
			if ((ptrT->isReady()) && (aimRC))
			{
				ptrT->shoot(*this);
				//what if it has killed the last enemy?
				if (gamestate)
					return gamestate; //no need to continue.
			}
		}
		return 0; //if gamestate!=0, MUST return smth before.
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
				Map[x][y].nextA = Pos(x, y); //uhm.. works?
				Map[x][y].nextL = Pos(x, y);
				Map[x][y].nextH = Pos(x, y);
			}
		//start
		Pos start = Castle->getPos();
		//set castle dist to 0
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
		for (set<Building*>::iterator it = Spawns.begin(); it != Spawns.end(); ++it)
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
			if (Pos((**it).getPos()) == p)
				return -4;
		//choosing correct type;
		if (type == 11)
		{
			//is gold enough?
			if (gold < wallBuildCost)
				return -2;
			//placing.. Can LInf pass to the castle?
			Tile backup(t); //backup
			B_Wall *ptrW = new B_Wall(p, wallMaxHP, wallMaxHP, name); //is that right? creates new building CAN THROW BADALLOC
			t.b = ptrW;
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
			//setting texture
			t.sprite.setTexture(*tileset[t.type]);
			//HPbar
			ptrW->HPbarBase.setPosition(p.x * tileWidth, p.y * tileWidth + (1 - BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
			ptrW->HPbarBase.setFillColor(BASE_BAR_COLOR);
			ptrW->HPbarBase.setSize(sf::Vector2f(tileWidth, BAR_HEIGHT_COEFF * tileWidth));
			ptrW->HPbar.setPosition(p.x * tileWidth, p.y * tileWidth + (1 - BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
			ptrW->HPbar.setFillColor(HP_BAR_COLOR);
			ptrW->setHPBarSize();
			return 1;
		}
		if (type == 12)
		{
			//is gold enough?
			if (gold < configTS[0].upgradeCost)
				return -2;
			//placing.. Can LInf pass to the castle?
			Tile backup(t); //backup
			B_Tower *ptrT = new B_Tower(p, 0, configTS[0], name, 1.0 / configTS[0].shootS); //is that right? creates new building CAN THROW BADALLOC
			t.b = ptrT;
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
			//setting texture
			t.sprite.setTexture(*tileset[t.type]);
			//adding "top" sprite
			const int towerTopType = 201; //for... search? Don't know. Let it be there.
			ptrT->topSprite.setTexture(*tileset[towerTopType]);
			ptrT->topSprite.setPosition(p.x * tileWidth + tileWidth / 2, p.y * tileWidth + tileWidth / 2);
			ptrT->topSprite.setOrigin(tileWidth / 2, tileWidth / 2);
			//RELOADbar
			ptrT->RELOADbarBase.setPosition(p.x * tileWidth, p.y * tileWidth + (1 - BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
			ptrT->RELOADbarBase.setFillColor(BASE_BAR_COLOR);
			ptrT->RELOADbarBase.setSize(sf::Vector2f(tileWidth, BAR_HEIGHT_COEFF * tileWidth));
			ptrT->RELOADbar.setPosition(p.x * tileWidth, p.y * tileWidth + (1 - BAR_HEIGHT_COEFF) * tileWidth); //position + 1 - bar_height before down side.
			ptrT->RELOADbar.setFillColor(RELOAD_BAR_COLOR);
			ptrT->setRELOADbarSize();
			//range circle
			ptrT->rangeCircle.setPosition(p.x * tileWidth + tileWidth / 2, p.y * tileWidth + tileWidth / 2);
			ptrT->rangeCircle.setFillColor(RANGE_COLOR);
			ptrT->rangeCircle.setOutlineThickness(RANGE_OUTLINE_THICKNESS);
			ptrT->rangeCircle.setOutlineColor(RANGE_OUTLINE_COLOR);
			ptrT->setRangeCircleSize(*this); //origin is set here too
			//shootline
			ptrT->shootLine.setPosition(p.x * tileWidth + tileWidth / 2, p.y * tileWidth + tileWidth / 2);
			ptrT->shootLine.setOrigin(0, SHOOTLINE_THICKNESS / 2);
			ptrT->shootLine.setFillColor(SHOOTLINE_COLOR);
			ptrT->shootLine.setSize(sf::Vector2f(0, SHOOTLINE_THICKNESS));
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
			//well, set new stats and lvl
			gold -= configTS[curLvl + 1].upgradeCost;
			bptr->setStats(configTS[curLvl + 1], *this);
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
			//well, set new stats and lvl
			gold -= configCS[curLvl + 1].upgradeCost;
			bptr->setStats(configCS[curLvl + 1]);
			bptr->setLvl(curLvl + 1);
			return 1;
		}
		return -1;
	}

	int Level::repair(const Pos &p, const double &healHP)
	{
		//is position in the range?
		if (!checkPos(p))
			return 0;
		Tile &t = (*this)[p];
		//is there any wall?
		if (t.type != 11)
			return -1;
		//is gold enough?
		B_Wall *bptr = static_cast<B_Wall*>(t.b);
		//if <=0, heal to full
		double healValue = (healHP <= 0) ? (wallMaxHP - bptr->getHP()) : fmin(healHP, wallMaxHP - bptr->getHP());
		int goldNeeded = ceil(healValue * wallRepairCost);
		if (gold < goldNeeded)
			return -2;
		gold -= goldNeeded;
		bptr->heal(healValue);
		return 1;
	}

	const TStats &Level::getTS(const int &lvl) const
	{
		if ((lvl < 0) || (lvl >= configTS.size()))
			throw exception("Level:getTS:illegal_level");
		return configTS[lvl];
	}

	const CStats &Level::getCS(const int &lvl) const
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
			//cout << "castle" << static_cast<B_Castle*>(t.b)->getName() << " is destroyed" << endl;
			gamestate = -1;
			return;
		}
		if (t.type != 11)
			throw exception("Level:destroyBuilding:illegal_type");
		//cout << "wall" << static_cast<B_Wall*>(t.b)->getName() << " is destroyed" << endl;
		delete t.b;
		t.b = nullptr;
		t.type = 01; //buildings could be only on plains
		//IF NOT CASTLE: layout changed => pathfind
		pathfind();
		//setting texture
		t.sprite.setTexture(*tileset[t.type]); //setting texture
	}

	/*
	void Level::killEnemy(Enemy *e)
	{
		//cout << e->getName() << " dies" << endl;
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

	*/

	Level::iterator Level::killEnemy(Level::iterator it)
	{
		Enemy *e = (*it);
		//cout << e->getName() << " dies" << endl;
		bool f = 0;
		if (dynamic_cast<Hero*>(e))
			f = 1;
		iterator retIt = removeEnemy(it);
		if (f)
			for (iterator it = begin(); it != end(); ++it)
				(**it).auraCheck(*this); //hero can change everything
		if ((Enemies.size() == 0) && allSpawnsEmpty())
			if (gamestate == 0)
				gamestate = 1; //for situations, when BOTH enemy and castle die.
		return retIt;
	}

	/*
	int Level::removeEnemy(Enemy *me)
	{
		delete me;
		return Enemies.erase(me);
	}
	*/

	Level::iterator Level::removeEnemy(Level::iterator it)
	{
		delete (*it);
		return Enemies.erase(it);
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
		for (set<Building*>::iterator it = Spawns.begin(); it != Spawns.end(); ++it)
			if (static_cast<B_Spawn*>(*it)->size() != 0)
				return 0;
		return 1;
	}

	void Level::draw(sf::RenderWindow &window)
	{
		//tiles, buildings
		for (int x = 0; x < sizeX; ++x)
			for (int y = 0; y < sizeY; ++y)
				Map[x][y].draw(window);
		//enemies
		for (iterator it = begin(); it != end(); ++it)
			(**it).draw(window);
		//enemy auras
		for (iterator it = begin(); it != end(); ++it)
			(**it).drawAura(window);
		//tower ranges
		for (int x = 0; x < sizeX; ++x)
			for (int y = 0; y < sizeY; ++y)
				Map[x][y].drawRange(window);
	}

	int Level::getTowerUpgradeCost(const int &curLvl) const
	{
		if (curLvl == configTS.size() - 1)
			return -1;
		else
			return configTS[curLvl + 1].upgradeCost;
	}

	int Level::getCastleUpgradeCost(const int &curLvl) const
	{
		if (curLvl == configCS.size() - 1)
			return -1;
		else
			return configCS[curLvl + 1].upgradeCost;
	}

	const TStats *Level::getNextTowerLevel(const int &curLvl) const
	{
		if (curLvl == configTS.size() - 1)
			return nullptr;
		else
			return &configTS[curLvl + 1];
	}

	const CStats *Level::getNextCastleLevel(const int &curLvl) const
	{
		if (curLvl == configCS.size() - 1)
			return nullptr;
		else
			return &configCS[curLvl + 1];
	}

	Enemy *Level::clickEnemy(const int &x, const int &y) const
	{
		Enemy *res = nullptr;
		for (const_iterator it = begin(); it != end(); ++it)
			if ((**it).sprite.getGlobalBounds().contains(x, y))
				res = (*it);
		return res;
	}
	
	Pos Level::clickTile(const int &x, const int &y) const
	{
		if (x < 0)
			return Pos(-1, -1);
		if (y < 0)
			return Pos(-1, -1);
		if (x >= sizeX * tileWidth)
			return Pos(-1, -1);
		if (x >= sizeY * tileWidth)
			return Pos(-1, -1);
		return Pos(x / tileWidth, y / tileWidth);
	}

	/*
	ePos Level::correct(const ePos &p) const
	{
		return ePos(p.x + tileWidth / 2, p.y + tileWidth / 2);
	}
	*/
}