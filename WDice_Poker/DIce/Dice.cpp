#include "stdafx.h"
#include "Dice.h"

const char* IMAGE[] = {//массив картинок
	"-----\n|   |\n| * |\n|   |\n-----\n",//1
	"-----\n|*  |\n|   |\n|  *|\n-----\n",//2
	"-----\n|*  |\n| * |\n|  *|\n-----\n",//3
	"-----\n|* *|\n|   |\n|* *|\n-----\n",//4
	"-----\n|* *|\n| * |\n|* *|\n-----\n",//5
	"-----\n|* *|\n|* *|\n|* *|\n-----\n"//6
};
namespace Prog3{
	Dice::Dice(const Dice& d) : size(d.size), SZ(d.SZ)
	{
		ar = new int[SZ];
		for (int i = 0; i < SZ; ++i)
			ar[i] = d.ar[i];
	}
	Dice::Dice(Dice&&d) :size(d.size), SZ(d.SZ), ar(d.ar)
	{
		d.ar = nullptr;
	}
	Dice::Dice(int number) : size(number), SZ(QUOTA)
	{
		srand(time(nullptr));
		if (number < 0)
			throw std::exception("Invalid size");
		else
		{
			while (SZ < size)
				SZ += QUOTA;
			ar = new int[SZ];
			for (int i = 0; i < size;)
				ar[i++] = rand() % 6 + 1;
			for (int i = size; i < SZ;)
				ar[i++] = 0;
		}
	}
	Dice::Dice() :size(QUOTA), SZ(QUOTA), ar(new int[QUOTA])
	{
		srand(time(nullptr));
		for (int i = 0; i < SZ; ++i)
			ar[i] = rand() % 6 + 1;
	}
	Dice::Dice(int number, const int* arr) : size(number), SZ(QUOTA)
	{
		if (number < 0)
			throw std::exception("Invalid size");
		else
		{
			while (SZ < size)
				SZ += QUOTA;
			ar = new int[SZ];
			for (int i = 0; i < size; i++)
				if (arr[i] < 7 && arr[i] > 0)
					ar[i] = arr[i];
				else
					throw std::exception("Invalid points in array!");
			for (int i = size; i < SZ; i++)
				ar[i] = 0;
		}
	}
	std::ostream & operator <<(std::ostream &c, const Dice &d)
	{
		if (!d.size)
			c << "Empty array." << std::endl;
		else
		{
			c << "Dices show:" << std::endl;
			for (int i = 0; i < d.size;)
				c << "#" << ++i << ":\n\n" << IMAGE[d.ar[i - 1] - 1] << std::endl;
		}
		return c;
	}
	void Dice::RollDice()
	{
		srand(time(nullptr));
		for (int i = 0; i < size; i++)
			ar[i] = rand() % 6 + 1;
	}
	bool Dice::Coinsidence() const
	{
		for (int i = 0; i < size - 1; ++i)
			for (int j = i + 1; j < size; j++)
				if (ar[i] == ar[j])
					return true;
		return false;
	}
	int Dice::Sum() const
	{
		int res = 0;
		for (int i = 0; i < size;)
			res += ar[i++];
		return res;
	}
	Dice & Dice::operator+=(int points)
	{
		if (points < 1 || points>6)
			throw std::exception("Invalid number of points!");
		if (size >= SZ)
		{
			SZ += QUOTA;
			int *old = ar;
			ar = new int[SZ];
			for (int i = 0; i < size; ++i)
				ar[i] = old[i];
			delete[] old;
		}
		ar[size++] = points;
		for (int i = size; i < SZ; i++)
			ar[i] = 0;
		return *this;
	}
	Dice & Dice::operator+=(const Dice & d)
	{
		if (size + d.size > SZ)
		{
			do
				SZ += QUOTA;
			while (size + d.size > SZ);
			int *old = ar;
			ar = new int[SZ];
			for (int i = 0; i < size; ++i)
				ar[i] = old[i];
			delete[] old;
		}
		for (int i = 0; i < d.size;)
			ar[size++] = d.ar[i++];
		for (int i = size; i < SZ; i++)
			ar[i] = 0;
		return *this;
	}
	Dice & Dice::operator-=(int points)
	{
		if (points < 1 || points>6)
			throw std::exception("Invalid number of points!");
		for (int i = 0; i < size;)
			if (ar[i] == points)
			{
				ar[i] = ar[--size];
				ar[size] = 0;
			}
			else
				i++;
		if (SZ - QUOTA > size)
		{
			do
				SZ -= QUOTA;
			while (SZ - QUOTA > size);
			if (SZ < QUOTA)
				SZ = QUOTA;
			int *old = ar;
			ar = new int[SZ];
			for (int i = 0; i < SZ; ++i)
				ar[i] = old[i];
			delete[] old;
		}
		return *this;
	}
	Dice & Dice::operator-=(const Dice & d)
	{
		for (int i = 0; i < d.size; i++)
			for (int j = 0; j < size;)
				if (ar[j] == d.ar[i])
				{
					ar[j] = ar[--size];
					ar[size] = 0;
				}
				else
					j++;
		if (SZ - QUOTA > size)
		{
			do
				SZ -= QUOTA;
			while (SZ - QUOTA > size);
			if (SZ < QUOTA)
				SZ = QUOTA;
			int *old = ar;
			ar = new int[SZ];
			for (int i = 0; i < SZ; ++i)
				ar[i] = old[i];
			delete[] old;
		}
		return *this;
	}
	const int Dice::operator[](int i)const
	{
		if (i<0 || i >= size)
			throw std::exception("Illegal index");
		return ar[i];
	}
	Dice & Dice::operator =(const Dice &d)
	{
		if (this != &d){
			size = d.size;
			SZ = d.SZ;
			delete[] ar;
			ar = new int[SZ];
			for (int i = 0; i < SZ; ++i)
				ar[i] = d.ar[i];
		}
		return *this;
	}
	Dice & Dice::operator =(Dice&&d)
	{
		int tmp = size;
		size = d.size;
		d.size = tmp;
		tmp = SZ;
		SZ = d.SZ;
		d.SZ = tmp;
		int *ptr = ar;
		ar = d.ar;
		d.ar = ptr;
		return *this;
	}
}