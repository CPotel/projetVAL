// projetVAL.cpp : définit le point d'entrée de l'application.
//

#include "projetVAL.h"

#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <random>
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
	std::default_random_engine re(time(0));
	//Initialisation liste de stations
	//Instalation de stations de métro dans notre système
  vector<station> liste_station;
	for (int i = 1; i < 20; i++) {
		liste_station.push_back(station(i, 10));
	}

	//test thread
	bool stopped = false;
	std::jthread rame1(
		[&stopped, &re, &liste_station]
		{
			metro rame1 = metro(25, 1, 0, 0, 0, 1);
			cout << "Rame prete" << endl;
			rame1.acceleration(10);
			cout << "Rame partie de la station de lancement" << endl;
			while (!stopped) {
				std::this_thread::sleep_for(1s);
				int pourcent = rame1.get_position();
				int vit = rame1.get_vitesse();
				if (pourcent < 100) {
					cout << "Progression de la rame :" << pourcent << endl;
					rame1.set_position(pourcent + vit);
				}
				else {
					rame1.freinage(vit);
					rame1.arrivee_station();
					rame1.set_position(0);
					int stat_nom = rame1.get_station();
					station stat_actu = liste_station.at(stat_nom - 1);
					cout << "Arrivee a la station numero " << stat_nom << endl;
					int passagers = rame1.get_passager_dedans();
					int aquai = stat_actu.get_passager();
					std::uniform_int_distribution<int> descente_pif{ 1, passagers };
					int descente = descente_pif(re);
					cout << "Descente de " << descente << " passagers" << endl;
					rame1.baisse_passager_dedans(descente);
					std::uniform_int_distribution<int> montee_pif{ 1,aquai };
					int montee = montee_pif(re);
					cout << "Montee de " << montee << " passagers" << endl;
					rame1.hausse_passager_dedans(montee);
					stopped = true;
				}
			}
		}
	);

	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Visualisation du métro Lillois");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}
}
