#pragma once

#define QUOTA 5

#define MAP_FOLDER "maps"
#define CONFIG_FOLDER "configs"
#define SAVE_FOLDER "saves"
#define DEFAULT_MAP_FILE "test_map.tmx"
#define DEFAULT_CONFIG_FILE "config.cfg"
#define DEFAULT_SAVE_FILE "newgame.sav"
#define CRASH_SAVE_FILE "crashsave.sav"
//#define AUTO_SAVE_FILE "autosave.sav"

namespace P4v25
{
	struct Pos
	{
		int x, y; //rar?
		Pos(int a = 0, int b = 0) :x(a), y(b) {}
		double dist(const Pos &p) const { return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y)); }
		const bool operator ==(const Pos &p) const { return ((x == p.x) && (y == p.y)); }
		const bool operator !=(const Pos &p) const { return !((x == p.x) && (y == p.y)); }
	};
}