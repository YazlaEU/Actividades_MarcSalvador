#include "Enemy.h"
#include <string>


Enemy::Enemy(std::vector<string> auxiliar)
{
	experience = std::stoi(auxiliar.back());
	auxiliar.pop_back();
	speed = std::stoi(auxiliar.back());
	auxiliar.pop_back();
	defense = std::stoi(auxiliar.back());
	auxiliar.pop_back();
	damage = std::stoi(auxiliar.back());
	auxiliar.pop_back();
	health = std::stoi(auxiliar.back());
	auxiliar.pop_back();
	enemyID = std::stoi(auxiliar.front());

}


Enemy::~Enemy()
{
}
