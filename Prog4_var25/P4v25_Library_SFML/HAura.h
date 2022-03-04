#pragma once


namespace P4v25_SFML
{
	class Enemy;

	class HAura
	{
	private:
		double maxHP;
		double regHP;
		double damC;
		double movS;
		double rad;
	public:
		HAura() : maxHP(0), regHP(0), damC(0), movS(0) {}
		HAura(const double &, const double &, const double &, const double &, const double &);
		virtual void buff(Enemy &) const;
		friend std::ostream &operator <<(std::ostream &, const HAura &);
		double getRad() const { return rad; }
		virtual ~HAura() {}
		virtual std::string getInfo() const;
	};
}