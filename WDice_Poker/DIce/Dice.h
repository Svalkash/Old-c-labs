#ifndef _DICE_H_
#define _DICE_H_
#include <iostream>
#include <ctime>
namespace Prog3{
	class Dice{
	private:
		static const int QUOTA = 10;// �� ������� ������ ����������
		int SZ;//������ ���������� ������
		int size;// ������� ������
		int* ar;// ������ � ������� �� 1 �� 6, ���� 0 (� ��� ����� ������, ������� ����� size � SZ)
	public:
		Dice();// ������� ������ ������� QUOTA � �������� ��������� ��� ������� �� 1 �� 6
		explicit Dice(int number);// ������� ������ ������� number � ��������� ��� ���������� ������� �� 1 �� 6
		Dice(int number, const int* arr);//���������� �� ������� arr ������� number �����(������ ���������� ��� ������������ ������)
		Dice(const Dice&);//���������� �����������
		Dice(Dice&&);//������������ �����������
		~Dice(){ delete[] ar; }//���������� � �������� ������
		friend std::ostream & operator <<(std::ostream &, const Dice &);//����� ����� ������� � ���� ��������
		int GetSize()const{ return size; };//���������� size
		int GetMaxSize()const{ return SZ; };//���������� SZ
		void RollDice();//��������� ��������� ������� �������� ����� � �������
		bool Coinsidence()const;//��������� ������� ����������
		int Sum() const;//���������� ����� ���� ����� (�����) �������
		Dice & operator +=(int);// ��������� � ������ �������� ����� (������ ���������� ��� ������������ ������)
		Dice & operator +=(const Dice &);//��������� ���� ������
		Dice & operator -=(int);//������� �� ������� ��� ����� � ������ ��������� 
		Dice & operator -=(const Dice &);//������� �� ������� ��� ������������� ��������
		const int operator[](int) const;//���������� �������� i-� �����
		Dice & operator =(const Dice &);//���������� �����
		Dice & operator =(Dice&&);//������������ �����
	};
}

#endif