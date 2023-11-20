// projetVAL.cpp : définit le point d'entrée de l'application.
//

#include "projetVAL.h"

using namespace std;


//Initialisation des getters de la classe station
const int station::get_passager(){
	return this->nb_passager;
}
const std::string station::get_nom() {
	return this->nom_station;
}
const bool station::is_a_train() {
	return this->train_present;
}

//Initialisation de la fonction depart train
void station::depart_train() {
	train_present = false;
}


int main()
{
	cout << "Hello CMake." << endl;
	return 0;
}
