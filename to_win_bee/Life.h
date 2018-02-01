#pragma once

class Life
{
private:
	int life;

public:
	Life() :
		life(0)
		{}

	~Life() {}

	void SetLife(int set);

	void MinusLife(int damage);

	int GetLife();
};