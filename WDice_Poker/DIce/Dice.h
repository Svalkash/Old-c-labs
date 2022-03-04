#ifndef _DICE_H_
#define _DICE_H_
#include <iostream>
#include <ctime>
namespace Prog3{
	class Dice{
	private:
		static const int QUOTA = 10;// по столько памяти выделяется
		int SZ;//рвзмер выделенной памяти
		int size;// текущий размер
		int* ar;// массив с числами от 1 до 6, либо 0 (в той части памяти, которая между size и SZ)
	public:
		Dice();// создает массив размера QUOTA и случайно заполняет его числами от 1 до 6
		explicit Dice(int number);// создает массив размера number и заполняет его случайными числами от 1 до 6
		Dice(int number, const int* arr);//записывает из массива arr размера number числа(выдаст исключение при неправильных числах)
		Dice(const Dice&);//копирующий конструктор
		Dice(Dice&&);//перемещающий конструктор
		~Dice(){ delete[] ar; }//деструктор с очисткой памяти
		friend std::ostream & operator <<(std::ostream &, const Dice &);//вывод всегр массива в виде рисунков
		int GetSize()const{ return size; };//возвращает size
		int GetMaxSize()const{ return SZ; };//возвращает SZ
		void RollDice();//изменияет случайным образом значение чисел в массиве
		bool Coinsidence()const;//проверяет наличие совпадений
		int Sum() const;//возврачает сумму всех очков (чисел) массива
		Dice & operator +=(int);// добавляет в массив значение числа (выдаст исключение при неправильных числах)
		Dice & operator +=(const Dice &);//добавляет весь массив
		Dice & operator -=(int);//удаляет из массива все кости с данным значением 
		Dice & operator -=(const Dice &);//удаляет из массива все встречающиеся значения
		const int operator[](int) const;//возвращает сначение i-й кости
		Dice & operator =(const Dice &);//копирующее равно
		Dice & operator =(Dice&&);//перемещающее равно
	};
}

#endif