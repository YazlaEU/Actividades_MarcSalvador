#pragma once
#include <vector>
#include <string>
using namespace std;
class Enemy
{
public:
	int enemyID;
	int health;
	int damage;
	int defense;
	int speed;
	int experience;
	std::vector<int> weapon();
	Enemy(std::vector<string> auxiliar);
	~Enemy();
};

