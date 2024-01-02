// projetVAL.cpp : définit le point d'entrée de l'application.
//

#include "projetVAL.h"
using namespace std;


//Initialisation des getters de la classe station
const int station::get_passager() {
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

void station::ChangementTextureGare(const sf::Texture& Text) {
	this->Gare.setTexture(Text);
}

void station::AffichageGare(sf::RenderWindow& Win) {
	Win.draw(this->Gare);
}

void station::ChangementTailleGare(const sf::Vector2f& taille) {
	this->Gare.setScale(taille);
}

void station::ChangementPositionGare(const sf::Vector2f& position) {
	this->Gare.setPosition(position);
}

//Changement Taille Voie
void station::ChangementTailleVoie1(const sf::Vector2f taille) {
	this->voie1.setSize(taille);
}
void station::ChangementTailleVoie2(const sf::Vector2f taille) {
	this->voie2.setSize(taille);
}

//RotationVoie
void station::RotationVoie(const int n) {
	this->voie1.rotate(n);
	this->voie2.rotate(n);
}

//Changement Position Voie
void station::ChangementPositionVoie1(const sf::Vector2f& pos) {
	this->voie1.setPosition(pos);
}
void station::ChangementPositionVoie2(const sf::Vector2f& pos) {
	this->voie2.setPosition(pos);
}
void station::AffichageVoies(sf::RenderWindow& Win) {
	Win.draw(this->voie1);
	Win.draw(this->voie2);

}


//Fonction du metro
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
	if ((!this->sens && this->station < taille_ligne) || (this->sens && this->station <= 1)) {
		this->prochain_arret = this->station + 1;
	}
	else {
		this->prochain_arret = this->station - 1;
	}
}
void metro::depart_station(int vit_depart) {
	this->station = 0; //0 équivaut à être en dehors d'une station;
	acceleration(vit_depart);
}

void metro::demi_tour() {
	this->sens = !this->sens;
}

const bool metro::reverse() {
	return this->sens;
}

//Changement Texture Wagon
void metro::ChangementTextureWagon(const sf::Texture& Text) {
	this->wagon.setTexture(Text);
}

//changement Position Metro
void metro::ChangementPositionMetro(const sf::Vector2f& pos) {
	this->wagon.setPosition(pos);
}

//Affichage Metro
void metro::AffichageMetro(sf::RenderWindow& Win) {
	Win.draw(this->wagon);
}
void metro::ChangementTailleMetro(const sf::Vector2f& pos) {
	this->wagon.setScale(pos);
}
void metro::RotationMetro(const int n) {
	this->wagon.setRotation(n);
}
int main()
{
	//Définition des tailles et positions des voies
	const int posX_voie[8] = { 130,280,480,580,780,930,1105,1280 };
	const int posY_voie1 = 200;
	const int posY_voie2 = 250;
	const int taille_voie[8] = { 150,200,100,200,150,175,180,140 };
	const int taille_metro[2] = { 512, 296 };
	const int PosXGare[9] = { 100,250,450,550,750,900,1075,1250,1390 };
	const int PosYGare = 200;

	std::default_random_engine re(time(0)); //seed aléatoire pour la montée et descente des passagers
	//Initialisation liste de stations
	//Instalation de stations de métro dans notre système
	vector<station> liste_station;
	for (int i = 1; i < 10; i++) {
		liste_station.push_back(station(i, 10));
	}
	//Initialisation des textures de Gare et de Wagon
	sf::Texture TextureGare, TextureWagon;
	if (!TextureGare.loadFromFile(std::string("C:/Program Files/SFML/img/gare.png")) || !TextureWagon.loadFromFile(std::string("C:/Program Files/SFML/img/RameMetro2.png")))
	{
		cerr << "Erreur pendant le chargement des images" << endl;

	}
	metro metro1 = metro(0, 1, 0, 0, 0, 1);
	metro1.ChangementTextureWagon(TextureWagon);
	metro metro2 = metro(0, 2, 0, 0, 0, 2); //création des 2 rames
	metro2.ChangementTextureWagon(TextureWagon);
	metro metro3 = metro(0, 3, 0, 0, 0, 3);
	metro3.ChangementTextureWagon(TextureWagon);

	int size = liste_station.size(); //nombre de stations créées

	const double pi = 3.14159265358979323846;
	bool stopped = false;

	bool panne1 = false;
	bool panne2 = false;
	bool panne3 = false;
	std::jthread rame1(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &metro3, &posX_voie, &posY_voie1, &posY_voie2, &taille_voie, &pi, &taille_metro, &panne1, &panne2, &panne3]
		{
			cout << "Rame 1 prete" << endl;
			metro1.ChangementPositionMetro(sf::Vector2f(posX_voie[0], posY_voie1));
			metro1.arrivee_station(size);
			cout << "Montee de passagers depuis la station 1 pour la rame 1" << endl;
			int aquai = liste_station.at(metro1.get_station() - 1).get_passager();
			std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai (au moins 1)
			int montee = montee_pif(re);
			cout << "Montee de " << montee << " passagers dans la rame 1." << endl;
			std::this_thread::sleep_for(3s);
			cout << "Depart de la rame 1 de la station 1" << endl;
			metro1.depart_station(10);
			while (!stopped) { //tant qu'on a pas arrêté
				std::this_thread::sleep_for(0.1s); //attente de 10ms pour simuler le déplacement de la rame
				int pourcent = metro1.get_position();
				int vit = metro1.get_vitesse(); //récupération de la position et de la vitesse de la rame
				int numero_station_suivante = metro1.get_prochain_arret();
				cout << "numero de station : " << numero_station_suivante - 2 << endl;
				if (panne2) {
					cout << "Panne de la rame 2, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (panne3) {
					cout << "Panne de la rame 3, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (pourcent < 100) { //si elle n'a pas atteint la station
					std::uniform_int_distribution<int> chance_panne{ 0,999 };
					int panne_pot = chance_panne(re);
					if (panne_pot == 449) {
						panne1 = true;
						cout << "La rame 1 vient de tomber en panne ! Le probleme est en train d'etre traite" << endl;
						std::this_thread::sleep_for(30s);
						cout << "La rame 1 est de nouveau operationnelle ! " << endl;
						panne1 = false;
					}
					cout << "Progression de la rame 1 : " << pourcent << " %" << endl; //affichage de sa progression
					metro1.set_position(pourcent + vit); //déplacement
					//Affichage du wagon
					if (metro1.reverse() == false) { //S'il est dans le sens des gare croissants
						metro1.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante - 2] + pourcent * pow(10, -2) * taille_voie[numero_station_suivante - 2] - (taille_metro[0] * pow(10, -1)) / 2, posY_voie1 - taille_metro[1] * pow(10, -1)));
					}
					else { //S'il est dans le sens des gare décroissants
						metro1.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante - 1] + taille_voie[numero_station_suivante - 1] * (1 - pourcent * pow(10, -2)) + (taille_metro[0] * pow(10, -1)) / 2, posY_voie2 + taille_metro[1] * pow(10, -1)));
						metro1.RotationMetro(180);
					}
				}
				else { //sinon
					metro1.freinage(vit); //arrêt
					metro1.arrivee_station(size); //arrivée à la station
					metro1.set_position(0); //réinitialisation de sa position
					int station_atteinte = metro1.get_station(); //récupération de la station atteinte
					station stat_actu = liste_station.at(station_atteinte - 1);
					stat_actu.arrivage_train();

					//if (metro2.get_prochain_arret() == station_atteinte && metro2.reverse() == metro1.reverse()) { //si les deux rames ont le meme arret et sont dans le meme sens de parcours
					//	cout << "Arret de la rame 2 pour maintenir une distance de securite" << endl;
					//	metro2.freinage(vit_autre);
					//}
					cout << "Arrivee de la rame 1 a la station numero " << station_atteinte << endl;
					int passagers = metro1.get_passager_dedans();
					int aquai = stat_actu.get_passager(); //récupération du nombre de passagers à bord et à quai
					if (station_atteinte == liste_station.size() || (station_atteinte == 1 && metro1.reverse())) { //si terminus
						cout << "Fin de trajet, preparation du demi-tour." << endl;
						cout << "Descente des " << passagers << " passagers restants de la rame 1." << endl;
						std::this_thread::sleep_for(passagers * 0.5s);
						metro1.baisse_passager_dedans(passagers); //descente de tous les passagers
						aquai += passagers;
						stat_actu.set_passager(aquai);
						metro1.demi_tour(); //demi tour
						cout << "Passage par la voie de demi-tour." << endl;
						std::this_thread::sleep_for(5s);
						cout << "Demi-tour effectue." << endl;
						std::uniform_int_distribution<int> montee_terminus{ 0, aquai };//montée de passagers
						int montee = montee_terminus(re);
						metro1.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Montee de " << montee << " passagers dans la rame 1." << endl;
						std::this_thread::sleep_for(montee * 0.25s);
						cout << "Depart de la rame 1 de la station " << station_atteinte << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro1.get_prochain_arret() == metro2.get_prochain_arret() || metro1.get_prochain_arret() == metro2.get_station()) && metro1.reverse() == metro2.reverse()) || ((metro1.get_prochain_arret() == metro3.get_prochain_arret() || metro1.get_prochain_arret() == metro3.get_station()) && metro1.reverse() == metro2.reverse())) {
							cout << "Metro 1 bloque en raison d'une autre rame de metro presente a la station suivante" << endl;
						}
						metro1.depart_station(vit); //début du trajet en sens inverse
						stat_actu.depart_train();
					}
					else { //sinon
						std::uniform_int_distribution<int> descente_pif{ 0, passagers }; //descente d'un nombre aléatoire de passagers de la rame (au moins 1)
						int descente = descente_pif(re);
						cout << "Descente de " << descente << " passagers de la rame 1." << endl;
						std::this_thread::sleep_for(descente * 0.25s);
						metro1.baisse_passager_dedans(descente);
						aquai += descente;
						stat_actu.set_passager(aquai);
						std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai (au moins 1)
						int montee = montee_pif(re);
						cout << "Montee de " << montee << " passagers dans la rame 1." << endl;
						std::this_thread::sleep_for(3s);
						metro1.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Depart de la rame 1 de la station " << station_atteinte << endl;
						while (((metro1.get_prochain_arret() == metro2.get_prochain_arret() || metro1.get_prochain_arret() == metro2.get_station()) && metro1.reverse() == metro2.reverse()) || ((metro1.get_prochain_arret() == metro3.get_prochain_arret() || metro1.get_prochain_arret() == metro3.get_station()) && metro1.reverse() == metro2.reverse())) {
							cout << "Metro 1 bloqué en raison d'une autre rame de métro présente à la station suivante" << endl;
						}
						metro1.depart_station(vit); //reprise du trajet
						stat_actu.depart_train();
					}
				}
			}
		}
	);
	//affichage sur une console :
	std::jthread rame2(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &metro3, &posX_voie, &posY_voie1, &posY_voie2, &taille_voie, &pi, &taille_metro, &panne1, &panne2, &panne3]
		{
			cout << "Rame 2 prete" << endl;
			metro2.ChangementPositionMetro(sf::Vector2f(posX_voie[0], posY_voie1));
			metro2.arrivee_station(size);
			cout << "Montee de passagers depuis la station 1 pour la rame 2" << endl;
			int aquai = liste_station.at(metro2.get_station() - 1).get_passager();
			std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai (au moins 1)
			int montee = montee_pif(re);
			cout << "Montee de " << montee << " passagers dans la rame 2." << endl;
			std::this_thread::sleep_for(montee * 0.25s);
			cout << "Depart de la rame 2 de la station 2" << endl;
			metro2.depart_station(10);
			while (!stopped) { //tant qu'on a pas arrêté
				std::this_thread::sleep_for(0.1s); //attente de 10ms pour simuler le déplacement de la rame
				int pourcent = metro2.get_position();
				int vit = metro2.get_vitesse(); //récupération de la position et de la vitesse de la rame
				int numero_station_suivante = metro2.get_prochain_arret();
				cout << "numero de station : " << numero_station_suivante - 2 << endl;
				if (panne1) {
					cout << "Panne de la rame 1, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (panne3) {
					cout << "Panne de la rame 3, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (pourcent < 100) { //si elle n'a pas atteint la station
					std::uniform_int_distribution<int> chance_panne{ 0,999 };
					int panne_pot = chance_panne(re);
					if (panne_pot == 16) {
						panne2 = true;
						cout << "La rame 2 vient de tomber en panne ! Le probleme est en train d'etre traite" << endl;
						std::this_thread::sleep_for(30s);
						cout << "La rame 2 est de nouveau operationnelle" << endl;
						panne2 = false;
					}
					cout << "Progression de la rame 2 : " << pourcent << " %" << endl; //affichage de sa progression
					metro2.set_position(pourcent + vit); //déplacement
					//Affichage du wagon
					if (metro2.reverse() == false) { //S'il est dans le sens des gare croissants
						metro2.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante - 2] + pourcent * pow(10, -2) * taille_voie[numero_station_suivante - 2] - (taille_metro[0] * pow(10, -1)) / 2, posY_voie1 - taille_metro[1] * pow(10, -1)));
					}
					else { //S'il est dans le sens des gare décroissants
						metro2.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante - 1] + taille_voie[numero_station_suivante - 1] * (1 - pourcent * pow(10, -2)) + (taille_metro[0] * pow(10, -1)) / 2, posY_voie2 + taille_metro[1] * pow(10, -1)));
						metro2.RotationMetro(180);
					}
				}
				else { //sinon
					metro2.freinage(vit); //arrêt
					metro2.arrivee_station(size); //arrivée à la station
					metro2.set_position(0); //réinitialisation de sa position
					int stat_nom = metro2.get_station(); //récupération de la station atteinte
					station stat_actu = liste_station.at(stat_nom - 1);
					stat_actu.arrivage_train();
					//if (metro3.get_prochain_arret() == stat_nom && metro3.reverse() == metro2.reverse()) { //si les deux rames ont le meme arret et sont dans le meme sens de parcours
					//	cout << "Arret de la rame 2 pour maintenir une distance de securite" << endl;
					//	metro2.freinage(vit_autre);
					//}
					cout << "Arrivee de la rame 2 a la station numero " << stat_nom << endl;
					int passagers = metro2.get_passager_dedans();
					int aquai = stat_actu.get_passager(); //récupération du nombre de passagers à bord et à quai
					if (stat_nom == liste_station.size() || (stat_nom == 1 && metro2.reverse())) { //si terminus
						cout << "Fin de trajet, preparation du demi-tour." << endl;
						cout << "Descente des " << passagers << " passagers restants de la rame 2." << endl;
						std::this_thread::sleep_for(passagers * 0.5s);
						metro2.baisse_passager_dedans(passagers); //descente de tous les passagers
						aquai += passagers;
						stat_actu.set_passager(aquai);
						metro2.demi_tour(); //demi tour
						cout << "Passage par la voie de demi-tour." << endl;
						std::this_thread::sleep_for(5s);
						cout << "Demi-tour effectue." << endl;
						std::uniform_int_distribution<int> montee_terminus{ 0, aquai };//montée de passagers
						int montee = montee_terminus(re);
						metro2.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Montee de " << montee << " passagers dans la rame 2." << endl;
						std::this_thread::sleep_for(3s);
						cout << "Depart de la rame 2 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro2.get_prochain_arret() == metro1.get_prochain_arret() || metro2.get_prochain_arret() == metro1.get_station()) && metro1.reverse() == metro2.reverse()) || ((metro2.get_prochain_arret() == metro3.get_prochain_arret() || metro1.get_prochain_arret() == metro3.get_station()) && metro2.reverse() == metro3.reverse())) {
							cout << "Metro 2 bloqué en raison d'une autre rame de métro présente à la station suivante" << endl;
						}
						metro2.depart_station(vit); //début du trajet en sens inverse
						stat_actu.depart_train();
					}
					else { //sinon
						std::uniform_int_distribution<int> descente_pif{ 0, passagers }; //descente d'un nombre aléatoire de passagers de la rame (au moins 1)
						int descente = descente_pif(re);
						cout << "Descente de " << descente << " passagers de la rame 2." << endl;
						std::this_thread::sleep_for(descente * 0.25s);
						metro2.baisse_passager_dedans(descente);
						aquai += descente;
						stat_actu.set_passager(aquai);
						std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai (au moins 1)
						int montee = montee_pif(re);
						cout << "Montee de " << montee << " passagers dans la rame 2." << endl;
						std::this_thread::sleep_for(montee * 0.25s);
						metro2.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Depart de la rame 1 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro2.get_prochain_arret() == metro1.get_prochain_arret() || metro2.get_prochain_arret() == metro1.get_station()) && metro1.reverse() == metro2.reverse()) || ((metro2.get_prochain_arret() == metro3.get_prochain_arret() || metro2.get_prochain_arret() == metro3.get_station()) && metro2.reverse() == metro3.reverse())) {
							cout << "Metro 2 bloqué en raison d'une autre rame de métro présente à la station suivante" << endl;
						}
						metro2.depart_station(vit); //reprise du trajet
						stat_actu.depart_train();
					}
				}
			}
		}
	);

	std::jthread rame3(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &metro3, &posX_voie, &posY_voie1, &posY_voie2, &taille_voie, &pi, &taille_metro, &panne3, &panne1, &panne2]
		{
			cout << "Rame 3 prete" << endl;
			metro3.ChangementPositionMetro(sf::Vector2f(posX_voie[0], posY_voie1));
			metro3.arrivee_station(size);
			cout << "Montee de passagers depuis la station 1 pour la rame 1" << endl;
			int aquai = liste_station.at(metro1.get_station() - 1).get_passager();
			std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai (au moins 1)
			int montee = montee_pif(re);
			cout << "Montee de " << montee << " passagers dans la rame 3." << endl;
			std::this_thread::sleep_for(montee * 0.25s);
			cout << "Depart de la rame 3 de la station 1" << endl;
			metro3.depart_station(10);
			while (!stopped) { //tant qu'on a pas arrêté
				std::this_thread::sleep_for(0.1s); //attente de 100ms pour simuler le déplacement de la rame
				int pourcent = metro3.get_position();
				int vit = metro3.get_vitesse(); //récupération de la position et de la vitesse de la rame
				int numero_station_suivante = metro3.get_prochain_arret();
				cout << "numero de station : " << numero_station_suivante - 2 << endl;
				if (panne1) {
					cout << "Panne de la rame 1, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (panne2) {
					cout << "Panne de la rame 2, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (pourcent < 100) { //si elle n'a pas atteint la station
					std::uniform_int_distribution<int> chance_panne{ 0,999 };
					int panne_pot = chance_panne(re);
					if (panne_pot == 792) {
						panne3 = true;
						cout << "La rame 3 vient de tomber en panne ! Le probleme est en train d'etre traite" << endl;
						std::this_thread::sleep_for(30s);
						cout << "La rame 3 est de nouveau operationnelle ! " << endl;
						panne3 = false;
					}
					cout << "Progression de la rame 3 : " << pourcent << " %" << endl; //affichage de sa progression
					metro3.set_position(pourcent + vit); //déplacement
					//Affichage du wagon
					if (metro3.reverse() == false) { //S'il est dans le sens des gare croissants
						metro3.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante - 2] + pourcent * pow(10, -2) * taille_voie[numero_station_suivante - 2] - (taille_metro[0] * pow(10, -1)) / 2, posY_voie1 - taille_metro[1] * pow(10, -1)));

					}
					else { //S'il est dans le sens des gare décroissants
						metro3.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante - 1] + taille_voie[numero_station_suivante - 1] * (1 - pourcent * pow(10, -2)) + (taille_metro[0] * pow(10, -1)) / 2, posY_voie2 + taille_metro[1] * pow(10, -1)));
						metro3.RotationMetro(180);
					}
				}
				else { //sinon
					metro3.freinage(vit); //arrêt
					metro3.arrivee_station(size); //arrivée à la station
					metro3.set_position(0); //réinitialisation de sa position
					int stat_nom = metro3.get_station(); //récupération de la station atteinte
					station stat_actu = liste_station.at(stat_nom - 1);
					stat_actu.arrivage_train();
					//if (metro3.get_prochain_arret() == stat_nom && metro1.reverse() == metro3.reverse()) { //si les deux rames ont le meme arret et sont dans le meme sens de parcours
					//	cout << "Arret de la rame 1 pour maintenir une distance de securite" << endl;
					//	metro1.freinage(vit_autre);
					//}
					cout << "Arrivee de la rame 3 a la station numero " << stat_nom << endl;
					int passagers = metro3.get_passager_dedans();
					int aquai = stat_actu.get_passager(); //récupération du nombre de passagers à bord et à quai
					if (stat_nom == liste_station.size() || (stat_nom == 1 && metro1.reverse())) { //si terminus
						cout << "Fin de trajet, preparation du demi-tour." << endl;
						cout << "Descente des " << passagers << " passagers restants de la rame 3." << endl;
						std::this_thread::sleep_for(passagers * 0.5s);
						metro3.baisse_passager_dedans(passagers); //descente de tous les passagers
						aquai += passagers;
						stat_actu.set_passager(aquai);
						metro3.demi_tour(); //demi tour
						cout << "Passage par la voie de demi-tour." << endl;
						std::this_thread::sleep_for(5s);
						cout << "Demi-tour effectue." << endl;
						std::uniform_int_distribution<int> montee_terminus{ 0, aquai };//montée de passagers
						int montee = montee_terminus(re);
						metro3.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Montee de " << montee << " passagers dans la rame 3." << endl;
						std::this_thread::sleep_for(montee * 0.25s);
						cout << "Depart de la rame 3 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro3.get_prochain_arret() == metro1.get_prochain_arret() || metro3.get_prochain_arret() == metro1.get_station()) && metro1.reverse() == metro3.reverse()) || ((metro3.get_prochain_arret() == metro2.get_prochain_arret() || metro3.get_prochain_arret() == metro2.get_station()) && metro2.reverse() == metro3.reverse())) {
							cout << "Metro 3 bloqué en raison d'une autre rame de métro présente à la station suivante" << metro3.get_prochain_arret() << metro2.get_prochain_arret() << metro1.get_prochain_arret() << metro2.get_station() << metro1.get_station() << endl;
						}
						metro3.depart_station(vit); //début du trajet en sens inverse
						stat_actu.depart_train();
					}
					else { //sinon
						std::uniform_int_distribution<int> descente_pif{ 0, passagers }; //descente d'un nombre aléatoire de passagers de la rame (au moins 1)
						int descente = descente_pif(re);
						cout << "Descente de " << descente << " passagers de la rame 3." << endl;
						std::this_thread::sleep_for(descente * 0.25s);
						metro3.baisse_passager_dedans(descente);
						aquai += descente;
						stat_actu.set_passager(aquai);
						std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai (au moins 1)
						int montee = montee_pif(re);
						cout << "Montee de " << montee << " passagers dans la rame 3." << endl;
						std::this_thread::sleep_for(montee * 0.25s);
						metro3.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Depart de la rame 3 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro3.get_prochain_arret() == metro1.get_prochain_arret() || metro3.get_prochain_arret() == metro1.get_station()) && metro1.reverse() == metro3.reverse()) || ((metro3.get_prochain_arret() == metro2.get_prochain_arret() || metro3.get_prochain_arret() == metro2.get_station()) && metro2.reverse() == metro3.reverse())) {
							cout << "Metro 3 bloqué en raison d'une autre rame de métro présente à la station suivante" << metro3.get_prochain_arret() << metro2.get_prochain_arret() << metro1.get_prochain_arret() << metro2.get_station() << metro1.get_station() << endl;
						}
						metro3.depart_station(vit); //reprise du trajet
						stat_actu.depart_train();
					}
				}
			}
		}
	);


	sf::RenderWindow window(sf::VideoMode(1600, 900), "Visualisation du métro Lillois");


	sf::CircleShape AllerRetour1(30), AllerRetour2(30);
	if (!TextureGare.loadFromFile(std::string("C:/Program Files/SFML/img/gare.png")))
	{
		cerr << "Erreur pendant le chargement des images" << endl;
		//return EXIT_FAILURE;
	}
	//On mets à toutes les gares la même texture
	for (int i = 0; i < liste_station.size(); i++) {
		liste_station[i].ChangementTextureGare(TextureGare);
		liste_station[i].ChangementTailleGare(sf::Vector2f(0.1, 0.1));
		liste_station[i].ChangementPositionGare(sf::Vector2f(PosXGare[i], PosYGare));

	}

	//MaJ des positions et de la tailles des différentes gares
	//Les gares :

	metro1.ChangementTailleMetro(sf::Vector2f(0.1, 0.1));
	metro2.ChangementTailleMetro(sf::Vector2f(0.1, 0.1));
	metro3.ChangementTailleMetro(sf::Vector2f(0.1, 0.1));
	//Mise à jour des positions des voies
	for (int i = 0; i < 8; i++) {
		liste_station[i].ChangementPositionVoie1(sf::Vector2f(posX_voie[i], posY_voie1));
		liste_station[i].ChangementPositionVoie2(sf::Vector2f(posX_voie[i], posY_voie2));
		liste_station[i].ChangementTailleVoie1(sf::Vector2f(2, taille_voie[i]));
		liste_station[i].ChangementTailleVoie2(sf::Vector2f(2, taille_voie[i]));
		liste_station[i].RotationVoie(270);
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color(255, 255, 255));
		//Affichage des gares et des lignes de metro
		//window.draw(AllerRetour1);
		//window.draw(AllerRetour2);
		for (int i = 0; i < liste_station.size(); i++) {
			liste_station[i].AffichageGare(window);
			liste_station[i].AffichageVoies(window);
		}
		metro1.AffichageMetro(window);
		metro2.AffichageMetro(window);
		metro3.AffichageMetro(window);
		window.display();
	}
}

