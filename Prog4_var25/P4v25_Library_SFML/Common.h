#pragma once

#define QUOTA 5

#define BAR_HEIGHT_COEFF 0.125 //so it will take 1/8 of the texture
#define HP_BAR_COLOR sf::Color(255, 0, 0)
#define GOLD_BAR_COLOR sf::Color(255, 255, 0)
#define RELOAD_BAR_COLOR sf::Color(0, 0, 255)
#define BASE_BAR_COLOR sf::Color(63, 63, 63)

#define AURA_POINTCOUNT 200
#define AURA_OUTLINE_THICKNESS -2.0 //now thickness will extrude TOWARDS the center
#define AURA_OUTLINE_COLOR sf::Color(255, 0, 0)
#define AURA_COLOR sf::Color(255, 0, 0, 32)

#define RANGE_POINTCOUNT 200
#define RANGE_OUTLINE_THICKNESS -2.0 //now thickness will extrude TOWARDS the center
#define RANGE_OUTLINE_COLOR sf::Color(0, 0, 255)
#define RANGE_COLOR sf::Color(0, 0, 255, 32)

#define SHOOTLINE_THICKNESS 4.0
#define SHOOTLINE_COLOR sf::Color(255, 127, 0)
#define SHOOTLINE_DELAY 0.05

#define PRINT_PRECISION 2

//#define AUTO_SAVE_FILE "autosave.sav"

/*
texture IDs:
Transparent:
0000 empty
Map:
0001 plains
0002 water
0003 hills
0011 wall
0012 tower
0013 castle
0021 spawn
Enemies:
0100-103 li
0110-113 liH
0120-123 hi
0130-133 hiH
0140-143 a
0150-153 aH
Other entities:
0201 tower top (crossbow or smth)
tileset for the program:
1001 window
1002 panel
Icons:
1101 play
1102 pause
1103 save (WITHOUT EXIT.. I hope)
1111 select buildings
1112 select enemies
1121 select buildings (PRESSED)
1122 select enemies (PRESSED)
1131 action build (TOWER)
1132 action build (WALL)
1133 action upgrade
1134 action repair
1141 yes
1142 no
*/

/*
Directions:
up 0
left 1
down 2
right 3
*/

#define DEFAULT_DIRECTION 2 //if someone doesn't move

namespace P4v25_SFML
{
	const int tilesetIDnumber = 48;
	const int tilesetIDs[tilesetIDnumber] = { 0, 01, 02, 03, 11, 12, 13, 21, 100, 101, 102, 103, 110, 111, 112, 113, 120, 121, 122, 123, 130, 131, 132, 133, 140, 141, 142, 143, 150, 151, 152, 153, 201, 1001, 1002, 1101, 1102, 1103, 1111, 1112, 1121, 1122, 1131, 1132, 1133, 1134, 1141, 1142 };
	struct ePos;
	struct Pos
	{
		int x, y; //rar?
		Pos() :x(0), y(0) {}
		Pos(int a, int b) :x(a), y(b) {}
		explicit Pos(const ePos &ep);
		double dist(const Pos &p) const { return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y)); }
		double dist(const ePos &p) const;
		const bool operator ==(const Pos &p) const { return ((x == p.x) && (y == p.y)); }
		const bool operator !=(const Pos &p) const { return !((x == p.x) && (y == p.y)); }
	};

	struct ePos //enemy position; contains doubles
	{
		double x, y; //rar?
		ePos() :x(0), y(0) {}
		ePos(double a, double b) :x(a), y(b) {}
		ePos(const Pos &p);
		double dist(const Pos &p) const;
		double dist(const ePos &p) const { return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y)); }
		const bool operator ==(const ePos &ep) const { return ((x == ep.x) && (y == ep.y)); }
		const bool operator !=(const ePos &ep) const { return !((x == ep.x) && (y == ep.y)); }
		Pos round() const { return Pos(std::round(x), std::round(y)); } //Don't need this; can use explicit (higher)
	};

	int sign(const float &f);

	std::string getfolder(const std::string &address);

	sf::Texture *cloneTex(const sf::Texture *tex);
}