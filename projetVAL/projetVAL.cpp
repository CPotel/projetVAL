// projetVAL.cpp : définit le point d'entrée de l'application.
//

#include "projetVAL.h"
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
void metro::arrivee_station(int taille_ligne) {
	this->station = this->prochain_arret;
	if (!this->sens && this->station < taille_ligne) {
		this->prochain_arret = this->station + 1;
	}
	else {
		this->prochain_arret = this->station - 1;
	}
}
void metro::depart_station(int vit_depart) {
	this->station = 0; //0 équivaut à être en dehors d'une station;
	acceleration(vit_depart); //Creation du départ A MODIFIER
}

void metro::demi_tour() {
	this->sens = !this->sens;
}

const bool metro::reverse() {
	return this->sens;
}


int main()
{
	std::default_random_engine re(time(0));
	//Initialisation liste de stations
	//Instalation de stations de métro dans notre système
  vector<station> liste_station;
	for (int i = 1; i < 4; i++) {
		liste_station.push_back(station(i, 10));
	}

	int size = liste_station.size();
	//test thread
	bool stopped = false;
	std::jthread rame1(
		[&stopped, &re, &liste_station, &size]
		{
			metro rame1 = metro(25, 1, 0, 0, 0, 1); //création de la rame de metro
			cout << "Rame 1 prete" << endl;
			rame1.acceleration(20); //départ de la rame de la zone de lancement
			cout << "Rame 1 partie de la station de lancement avec " << rame1.get_passager_dedans() << " passagers." << endl;
			while (!stopped) { //tant qu'on a pas arrêté
				std::this_thread::sleep_for(1s); //attente d'1s pour simuler le déplacement de la rame
				int pourcent = rame1.get_position();
				int vit = rame1.get_vitesse(); //récupération de la position et de la vitesse de la rame
				if (pourcent < 100) { //si elle n'a pas atteint la station
					cout << "Progression de la rame 1 : " << pourcent << " %" << endl; //affichage de sa progression
					rame1.set_position(pourcent + vit); //déplacement
				}
				else { //sinon
					rame1.freinage(vit); //arrêt
					rame1.arrivee_station(size); //arrivée à la station
					rame1.set_position(0); //réinitialisation de sa position
					int stat_nom = rame1.get_station(); //récupération de la station atteinte
					station stat_actu = liste_station.at(stat_nom - 1);
					stat_actu.arrivage_train();
					cout << "Arrivee a la station numero " << stat_nom << endl;
					int passagers = rame1.get_passager_dedans();
					int aquai = stat_actu.get_passager(); //récupération du nombre de passagers à bord et à quai
					if (stat_nom == liste_station.size() || (stat_nom ==1 && rame1.reverse())) { //si terminus
						cout << "Fin de trajet, preparation du demi-tour." << endl;
						cout << "Descente des " << passagers << " passagers restants." << endl;
						std::this_thread::sleep_for(passagers * 0.5s);
						rame1.baisse_passager_dedans(passagers); //descente de tous les passagers
						aquai += passagers;
						stat_actu.set_passager(aquai);
						rame1.demi_tour(); //demi tour
						cout << "Passage par la voie de demi-tour." << endl;
						std::this_thread::sleep_for(5s);
						cout << "Demi-tour effectue." << endl;
						std::uniform_int_distribution<int> montee_terminus{ 1, aquai };//montée de passagers
						int montee = montee_terminus(re);
						rame1.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Montee de " << montee << " passagers." << endl;
						std::this_thread::sleep_for(montee * 0.25s);
						cout << "Depart de la station " << stat_nom << endl;
						rame1.depart_station(vit); //début du trajet en sens inverse
						stat_actu.depart_train();
					}
					else{ //sinon
						std::uniform_int_distribution<int> descente_pif{ 1, passagers }; //descente d'un nombre aléatoire de passagers de la rame (au moins 1)
						int descente = descente_pif(re);
						cout << "Descente de " << descente << " passagers" << endl;
						std::this_thread::sleep_for(descente * 0.25s);
						rame1.baisse_passager_dedans(descente);
						aquai += descente;
						stat_actu.set_passager(aquai);
						std::uniform_int_distribution<int> montee_pif{ 1,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai (au moins 1)
						int montee = montee_pif(re);
						cout << "Montee de " << montee << " passagers" << endl;
						std::this_thread::sleep_for(montee * 0.25s);
						rame1.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Depart de la rame de la station " << stat_nom << endl;
						rame1.depart_station(vit); //reprise du trajet
						stat_actu.depart_train();
					}
				}
			}
		}
	);

	std::jthread rame2(
		[&stopped, &re, &liste_station, &size]
		{
			metro rame2(10, 1, 0, 0, 0, 2);
			cout << "Rame 2 prete" << endl;
			cout << "Rame 2 partie de la station de lancement avec " << rame2.get_passager_dedans() << " passagers" << endl;
		}
	);
	/*
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
	*/
}
