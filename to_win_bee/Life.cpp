#include "Life.h"

void Life::SetLife(int set)
{
	life = set;
}

void Life::MinusLife(int damage)
{
	life -= damage;
}

int Life::GetLife()
{
	return life;
}