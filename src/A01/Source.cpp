#include "../../dep/inc/XML/rapidxml.hpp"
#include "../../dep/inc/XML/rapidxml_utils.hpp"
#include "../../dep/inc/XML/rapidxml_iterators.hpp"
#include "../../dep/inc/XML/rapidxml_print.hpp"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include "Enemy.h"
using namespace std;

class weapon {
	int weaponID;
	string weaponClass;
	int weaponRange;
};

int main() {
	rapidxml::xml_document<> doc;
	std::ifstream file("../../res/files/config.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);
	std::cout << "Nombre de root: " << doc.first_node()->name() << "\n";
	std::cout << "Value: " << doc.first_node()->value() << "\n";
	std::vector<Enemy> enemies; //No inicializar el vector, utilizar push para crear mas espacio en memoria.
	std::vector<string> aux;
	std::vector<weapon> weapon; 
	rapidxml::xml_node<> *pRoot = doc.first_node();
		for (rapidxml::xml_node<> *pNode = pRoot->first_node("Enemy"); pNode; pNode = pNode->next_sibling()) {
			//Establere modo de leer el archivo XML e introducirlo en el vector.
			for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI->next_sibling()) {
				aux.push_back(pNodeI->value());
			}
			enemies.push_back(Enemy(aux));
			aux.clear();
		}
	return 0;
}