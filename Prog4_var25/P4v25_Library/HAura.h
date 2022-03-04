#pragma once


namespace P4v25
{
	class Enemy;

	class HAura
	{
	private:
		double maxHP;
		double regHP;
		double damC;
		int movS;
		double rad;
	public:
		HAura() : maxHP(0), regHP(0), damC(0), movS(0) {}
		HAura(const double &, const double &, const double &, const int &, const double &);
		virtual void buff(Enemy &) const;
		friend std::ostream &operator <<(std::ostream &, const HAura &);
		double getRad() const { return rad; }
		virtual ~HAura() {}
	};
}