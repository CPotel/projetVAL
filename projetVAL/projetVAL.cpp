// projetVAL.cpp : définit le point d'entrée de l'application.
//

#include "projetVAL.h"
#include <thread>
#include <chrono>

using namespace std;


//Initialisation des getters de la classe station
const int station::get_passager(){
	return this->nb_passager;
}
const int station::get_nom() {
	return this->numero_station;
}
const bool station::is_a_train() {
	return this->train_present;
}

//Initialisation de la fonction depart train
void station::depart_train() {
	train_present = false;
}

void station::set_passager(int n) {
	this->nb_passager = n;
}

void station::arrivage_train() {
	if (!this->is_a_train()) {
		this->train_present = true;
	}
}

void station::baisse_passager(int n) {
	this->nb_passager -= n;
}

const int metro::get_passager_dedans() {
	return this->nb_passager_dedans;
}

const int metro::get_position() {
	return this->position;
}

const int metro::get_station() {
	return this->station;
}

const int metro::get_vitesse() {
	return this->vitesse;
}

const int metro::get_prochain_arret() {
	return this->prochain_arret;
}
const int metro::get_numero_train() {
	return this->numero_train;
}
//Setters de la classe metro

void metro::acceleration(int n) {
	this->vitesse += n;
}
void metro::freinage(int n) {
	this->vitesse -= n;
}
void metro::baisse_passager_dedans(int n) {
	this->nb_passager_dedans -= n;
}
void metro::set_passager_dedans(int n) {
	this->nb_passager_dedans = n;
}
void metro::hausse_passager_dedans(int n) {
	this->nb_passager_dedans += n;
}
void metro::set_position(int pos) {
	this->position = pos;
}
void metro::arrivee_station() {
	this->station = this->prochain_arret;
	//Réaliser prochain arret
}
void metro::depart_station() {
	this->station = 0; //0 équivaut à être en dehors d'une station;
	acceleration(1); //Creation du départ A MODIFIER
}


int main()
{
	//Initialisation liste de stations
	vector<station> liste_station;
	//Instalation de stations de métro dans notre système
	//test thread
	station CHU = station(1);
	liste_station.push_back(CHU);
	bool stopped = false;
	std::jthread thr(
		[&stopped]
		{
			metro rame1 = metro(25, 1, 0, 0, 0, 1);
			cout << "Rame prete" << endl;
			rame1.acceleration(7);
			cout << "Rame partie de la station de lancement" << endl;
			while (!stopped) {
				std::this_thread::sleep_for(1s);
				int pourcent = rame1.get_position();
				if (pourcent <= 100) {
					cout << "Progression de la rame :" << pourcent <<endl;
					int vit = rame1.get_vitesse();
					rame1.set_position(pourcent + vit);
				}
				else {
					rame1.freinage(5);
					rame1.arrivee_station();
					cout << "Arrivee a la station numero " << rame1.get_station() << endl;
					stopped = true;
				}
			}
		}
		);
}
