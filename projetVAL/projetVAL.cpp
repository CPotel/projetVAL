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
	metro metro2 = metro(0, 2, 0, 0, 0, 2);
	metro2.ChangementTextureWagon(TextureWagon);
	metro metro3 = metro(0, 3, 0, 0, 0, 3);
	metro3.ChangementTextureWagon(TextureWagon);

	int size = liste_station.size(); //nombre de stations créées

	bool stopped = false;
	bool panne1 = false;
	bool panne2 = false;
	bool panne3 = false;

	
	
	std::jthread rame1(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &metro3, &posX_voie, &posY_voie1, &posY_voie2, &taille_voie, &taille_metro, &panne1, &panne2, &panne3]
		{
			cout << "Rame 1 prete" << endl;
			//On effectue un premier affichage du metro
			metro1.ChangementPositionMetro(sf::Vector2f(posX_voie[0], posY_voie1 - 30));
			//On indique qu'il est dans la gare
			metro1.arrivee_station(size);
			//On fait monter des passagers
			cout << "Montee de passagers depuis la station 1 pour la rame 1" << endl;
			int aquai = liste_station.at(metro1.get_station() - 1).get_passager();
			std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai
			int montee = montee_pif(re);
			cout << "Montee de " << montee << " passagers dans la rame 1." << endl;
			//Le temps d'attente dans une gare est de 3 secondes
			std::this_thread::sleep_for(5s);
			//On organise le premier départ
			cout << "Depart de la rame 1 de la station 1" << endl;
			metro1.depart_station(1);
			//Boucle infinie permettant de réaliser le mouvement du métro
			while (!stopped) { 
				//On fait une pause dans le thread afin de simuler un déplacement du métro
				std::this_thread::sleep_for(0.2s);
				//On récupère la position du metro, sa vitesse ainsi que la station d'arret suivante
				int pourcent = metro1.get_position();
				int vit = metro1.get_vitesse();
				int numero_station_suivante1 = metro1.get_prochain_arret();
				//On récupère les pannes des autres metros, si une est active on arrête le métro pour 30 secondes
				if (panne2) {
					cout << "Panne de la rame 2, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (panne3) {
					cout << "Panne de la rame 3, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				//si elle n'a pas atteint la station, donc que l'on doit encore se déplacer
				if (pourcent < 100) {
					//On utilise une variable aléatoire permettant de créer une panne
					//Si une panne est crée, on va stopper tous les métros pendant 30 secondes
					std::uniform_int_distribution<int> chance_panne{ 0,999 };
					int panne_pot = chance_panne(re);
					if (panne_pot == 449) {
						panne1 = true;
						cout << "La rame 1 vient de tomber en panne ! Le probleme est en train d'etre traite" << endl;
						std::this_thread::sleep_for(30s);
						cout << "La rame 1 est de nouveau operationnelle ! " << endl;
						panne1 = false;
					}
					//S'il n'y a pas de panne, alors on déplace le métro et on l'affiche
					cout << "Progression de la rame 1 : " << pourcent << " %" << "		numero de station suivante : " << numero_station_suivante1 << endl; //affichage de sa progression
					metro1.set_position(pourcent + vit); //déplacement
					
					//Affichage du wagon :
					//S'il est dans le sens des gare croissants
					if (metro1.reverse() == false) {
						metro1.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante1 - 2] + pourcent * pow(10, -2) * taille_voie[numero_station_suivante1 - 2] - (taille_metro[0] * pow(10, -1)) / 2, posY_voie1 - taille_metro[1] * pow(10, -1)));
					}
					//S'il est dans le sens des gare décroissants
					else { 
						metro1.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante1 - 1] + taille_voie[numero_station_suivante1 - 1] * (1 - pourcent * pow(10, -2)) + (taille_metro[0] * pow(10, -1)) / 2, posY_voie2 + taille_metro[1] * pow(10, -1)));
						metro1.RotationMetro(180);
					}
					if (pourcent < 75 && vit < 5) { //tant qu'on a pas atteint la vitesse de croisière (limitée à une certaine portion du trajet)
						metro1.acceleration(1); //la rame accélère jusqu'à l'atteindre
					}
					if(pourcent>80 && vit > 1){ //a l'approche d'une gare
						metro1.freinage(1); //on fait freiner la rame
					}
				}

				//Sinon, c'est qu'il est arrivé à un station
				else { 
					//On arrête le metro et on indique qu'il est arrivé à la station, enfin on réinitialise sa position
					metro1.freinage(vit);
					metro1.arrivee_station(size);
					metro1.set_position(0);

					//On récupère la station que l'on vient d'atteindre afin de gérer le flux de passager
					int station_atteinte = metro1.get_station();
					station stat_actu = liste_station.at(station_atteinte - 1);
					stat_actu.arrivage_train();
					cout << "Arrivee de la rame 1 a la station numero " << station_atteinte << endl;
					//On récupère le nombre de passagers dans le metro et sur le quai
					int passagers = metro1.get_passager_dedans();
					int aquai = stat_actu.get_passager(); 
					//on vérifie que l'on est pas à un terminus
					if (station_atteinte == liste_station.size() || (station_atteinte == 1 && metro1.reverse())) { 
						//Si c'est un terminus
						cout << "Fin de trajet, preparation du demi-tour." << endl;
						//On fait descendre tous les passagers
						cout << "Descente des " << passagers << " passagers restants de la rame 1." << endl;
						//On fait attendre le metro 3 secondes, le temps d'être sur le quai
						std::this_thread::sleep_for(3s);
						metro1.baisse_passager_dedans(passagers);
						//On ajoute les passagers au quai
						aquai += passagers;
						stat_actu.set_passager(aquai);
						//On effectue le demi tour au metro
						metro1.demi_tour();
						cout << "Passage par la voie de demi-tour." << endl;
						std::this_thread::sleep_for(5s);
						cout << "Demi-tour effectue." << endl;
						//On fait monter un nombre aléatoire de passagers
						std::uniform_int_distribution<int> montee_terminus{ 0, aquai };
						int montee = montee_terminus(re);
						metro1.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Montee de " << montee << " passagers dans la rame 1." << endl;
						//On fait attendre le metro 3 secondes, le temps d'être sur le quai
						std::this_thread::sleep_for(3s);
						cout << "Depart de la rame 1 de la station " << station_atteinte << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro1.get_prochain_arret() == metro2.get_prochain_arret() || metro1.get_prochain_arret() == metro2.get_station()) && metro1.reverse() == metro2.reverse()) || ((metro1.get_prochain_arret() == metro3.get_prochain_arret() || metro1.get_prochain_arret() == metro3.get_station()) && metro1.reverse() == metro2.reverse())) {
							//On ne fait pas redémarrer le metro tant qu'une rame bloque la voie
							//out << "Metro 1 bloqué en raison d'une autre rame de métro présente à la station suivante" << endl;
						}
						//On fait redémarrer le metro (qui sera en sens inverse)
						metro1.depart_station(vit);
						stat_actu.depart_train();
					}
					//Si ce n'est pas un terminus
					else { 
						//On initialise aléatoirement le nombre de passagers qui vont descendre
						std::uniform_int_distribution<int> descente_pif{ 0, passagers };
						int descente = descente_pif(re);
						cout << "Descente de " << descente << " passagers de la rame 1." << endl;
						//On fait attendre le metro 3 secondes pour simuler l'arret à un quai
						std::this_thread::sleep_for(3s);
						//On fait descendre les passagers
						metro1.baisse_passager_dedans(descente);
						aquai += descente;
						stat_actu.set_passager(aquai);
						//On initialise un nombre aléatoire de passagers qui vont monter dans le train
						std::uniform_int_distribution<int> montee_pif{ 0,aquai }; 
						int montee = montee_pif(re);
						cout << "Montee de " << montee << " passagers dans la rame 1." << endl;
						metro1.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);

						cout << "Depart de la rame 1 de la station " << station_atteinte << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro1.get_prochain_arret() == metro2.get_prochain_arret() || metro1.get_prochain_arret() == metro2.get_station()) && metro1.reverse() == metro2.reverse()) || ((metro1.get_prochain_arret() == metro3.get_prochain_arret() || metro1.get_prochain_arret() == metro3.get_station()) && metro1.reverse() == metro2.reverse())) {
							//On ne fait pas redémarrer le metro tant qu'une rame bloque la voie
							cout << "Metro 1 bloqué en raison d'une autre rame de métro présente à la station suivante" << endl;
						}
						//On fait redémarrer le metro
						metro1.depart_station(vit);
						stat_actu.depart_train();
					}
				}
			}
		}
	);
	//affichage sur une console :
	
	std::jthread rame2(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &metro3, &posX_voie, &posY_voie1, &posY_voie2, &taille_voie, &taille_metro, &panne1, &panne2, &panne3]
		{
			cout << "Rame 2 prete" << endl;
			//On effectue un premier affichage du metro
			metro2.ChangementPositionMetro(sf::Vector2f(posX_voie[1], posY_voie1 - 30));
			//On indique qu'il est dans la gare
			metro2.arrivee_station(size);
			//On fait monter des passagers
			cout << "Montee de passagers depuis la station 1 pour la rame 2" << endl;
			int aquai = liste_station.at(metro2.get_station() - 1).get_passager();
			std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai
			int montee = montee_pif(re);
			cout << "Montee de " << montee << " passagers dans la rame 2." << endl;
			//Le temps d'attente dans une gare est de 3 secondes
			std::this_thread::sleep_for(5s);
			//On organise le premier départ
			cout << "Depart de la rame 2 de la station 2" << endl;
			metro2.depart_station(1);
			//Boucle infinie permettant de réaliser le mouvement du métro
			while (!stopped) {
				//On fait une pause dans le thread afin de simuler un déplacement du métro
				std::this_thread::sleep_for(0.2s); 
				//On récupère la position du metro, sa vitesse ainsi que la station d'arret suivante
				int pourcent = metro2.get_position();
				int vit = metro2.get_vitesse(); 
				int numero_station_suivante2 = metro2.get_prochain_arret();
				
				//On récupère les pannes des autres metros, si une est active on arrête le métro pour 30 secondes
				if (panne1) {
					cout << "Panne de la rame 1, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (panne3) {
					cout << "Panne de la rame 3, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}

				//si elle n'a pas atteint la station, donc que l'on doit encore se déplacer
				if (pourcent < 100) { 
					//On utilise une variable aléatoire permettant de créer une panne
					//Si une panne est crée, on va stopper tous les métros pendant 30 secondes
					std::uniform_int_distribution<int> chance_panne{ 0,999 };
					int panne_pot = chance_panne(re);
					if (panne_pot == 16) {
						panne2 = true;
						cout << "La rame 2 vient de tomber en panne ! Le probleme est en train d'etre traite" << endl;
						std::this_thread::sleep_for(30s);
						cout << "La rame 2 est de nouveau operationnelle" << endl;
						panne2 = false;
					}
					//S'il n'y a pas de panne, alors on déplace le métro et on l'affiche
					cout << "Progression de la rame 2 : " << pourcent << " %" << "		numero de station suivante : " << numero_station_suivante2 << endl; //affichage de sa progression
					metro2.set_position(pourcent + vit); //déplacement
					
					//Affichage du wagon
					//S'il est dans le sens des gare croissants
					if (metro2.reverse() == false) { 
						metro2.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante2 - 2] + pourcent * pow(10, -2) * taille_voie[numero_station_suivante2 - 2] - (taille_metro[0] * pow(10, -1)) / 2, posY_voie1 - taille_metro[1] * pow(10, -1)));
					}
					//S'il est dans le sens des gare décroissants
					else { 
						metro2.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante2 - 1] + taille_voie[numero_station_suivante2 - 1] * (1 - pourcent * pow(10, -2)) + (taille_metro[0] * pow(10, -1)) / 2, posY_voie2 + taille_metro[1] * pow(10, -1)));
						metro2.RotationMetro(180);
					}
					if (pourcent < 75 && vit < 5) { //tant qu'on a pas atteint la vitesse de croisière (limitée à une certaine portion du trajet)
						metro2.acceleration(1); //la rame accélère jusqu'à l'atteindre
					}
					if (pourcent > 80 && vit > 1) { //a l'approche d'une gare
						metro2.freinage(1); //on fait freiner la rame
					}
				}
				//Sinon, c'est qu'il est arrivé à un station
				else { 
					//On arrête le metro et on indique qu'il est arrivé à la station, enfin on réinitialise sa position
					metro2.freinage(vit);
					metro2.arrivee_station(size);
					metro2.set_position(0); 
					//On récupère la station que l'on vient d'atteindre afin de gérer le flux de passager
					int stat_nom = metro2.get_station(); 
					station stat_actu = liste_station.at(stat_nom - 1);
					stat_actu.arrivage_train();
					cout << "Arrivee de la rame 2 a la station numero " << stat_nom << endl;
					//On récupère le nombre de passagers dans le metro et sur le quai
					int passagers = metro2.get_passager_dedans();
					int aquai = stat_actu.get_passager(); //récupération du nombre de passagers à bord et à quai
					//on vérifie que l'on est pas à un terminus
					if (stat_nom == liste_station.size() || (stat_nom == 1 && metro2.reverse())) { //si terminus
						//Si c'est un terminus
						cout << "Fin de trajet, preparation du demi-tour." << endl;
						//On fait descendre tous les passagers
						cout << "Descente des " << passagers << " passagers restants de la rame 2." << endl;
						//On fait attendre le metro 3 secondes, le temps d'être sur le quai
						std::this_thread::sleep_for(3s);
						metro2.baisse_passager_dedans(passagers); 
						//On ajoute les passagers au quai
						aquai += passagers;
						stat_actu.set_passager(aquai);
						//On effectue le demi tour au metro
						metro2.demi_tour();
						cout << "Passage par la voie de demi-tour." << endl;
						std::this_thread::sleep_for(5s);
						cout << "Demi-tour effectue." << endl;
						//On fait monter un nombre aléatoire de passagers
						std::uniform_int_distribution<int> montee_terminus{ 0, aquai };
						int montee = montee_terminus(re);
						metro2.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Montee de " << montee << " passagers dans la rame 2." << endl;
						//On fait attendre le metro 3 secondes, le temps d'être sur le quai
						std::this_thread::sleep_for(3s);
						cout << "Depart de la rame 2 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						cout << metro2.get_prochain_arret() << "	" << metro1.get_prochain_arret()<< "	" << metro1.get_station() << "	" << metro3.get_prochain_arret() << "		" << metro3.get_station() << endl;
						while (((metro2.get_prochain_arret() == metro1.get_prochain_arret() || metro2.get_prochain_arret() == metro1.get_station())  && metro1.reverse() == metro2.reverse()) || ((metro2.get_prochain_arret() == metro3.get_prochain_arret() || metro2.get_prochain_arret() == metro3.get_station())  && metro2.reverse() == metro3.reverse())) {
							//On ne fait pas redémarrer le metro tant qu'une rame bloque la voie
							cout << "Metro 2 bloqué en raison d'une autre rame de métro présente à la station suivante" << endl;
						}
						//On fait redémarrer le metro (qui sera en sens inverse)
						metro2.depart_station(vit);
						stat_actu.depart_train();
					}
					//Si ce n'est pas un terminus
					else { 
						//On initialise aléatoirement le nombre de passagers qui vont descendre
						std::uniform_int_distribution<int> descente_pif{ 0, passagers };
						int descente = descente_pif(re);
						cout << "Descente de " << descente << " passagers de la rame 2." << endl;
						//On fait attendre le metro 3 secondes pour simuler l'arret à un quai
						std::this_thread::sleep_for(3s);
						//On fait descendre les passagers
						metro2.baisse_passager_dedans(descente);
						aquai += descente;
						stat_actu.set_passager(aquai);
						//On initialise un nombre aléatoire de passagers qui vont monter dans le train
						std::uniform_int_distribution<int> montee_pif{ 0,aquai };
						int montee = montee_pif(re);
						cout << "Montee de " << montee << " passagers dans la rame 2." << endl;
						
						metro2.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Depart de la rame 1 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						cout << metro2.get_prochain_arret() << "	" << metro1.get_prochain_arret() << "	" << metro1.get_station() << "	" << metro3.get_prochain_arret() << "		" << metro3.get_station() << endl;
						while (((metro2.get_prochain_arret() == metro1.get_prochain_arret() || metro2.get_prochain_arret() == metro1.get_station()) && metro1.reverse() == metro2.reverse()) || ((metro2.get_prochain_arret() == metro3.get_prochain_arret() || metro2.get_prochain_arret() == metro3.get_station()) && metro2.reverse() == metro3.reverse())) {
							//On ne fait pas redémarrer le metro tant qu'une rame bloque la voie
							cout << "Metro 2 bloqué en raison d'une autre rame de métro présente à la station suivante" << endl;
						}
						//On fait redémarrer le metro
						metro2.depart_station(vit);
						stat_actu.depart_train();
					}
				}
			}
		}
	);
	
	std::jthread rame3(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &metro3, &posX_voie, &posY_voie1, &posY_voie2, &taille_voie, &taille_metro, &panne1, &panne2, &panne3]
		{
			cout << "Rame 3 prete" << endl;
			//On effectue un premier affichage du metro
			metro3.ChangementPositionMetro(sf::Vector2f(posX_voie[2], posY_voie1 - 30));
			//On indique qu'il est dans la gare
			metro3.arrivee_station(size);
			cout << "Montee de passagers depuis la station 1 pour la rame 1" << endl;
			//On fait monter des passagers
			int aquai = liste_station.at(metro1.get_station() - 1).get_passager();
			std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai
			int montee = montee_pif(re);
			cout << "Montee de " << montee << " passagers dans la rame 3." << endl;
			//Le temps d'attente dans une gare est de 3 secondes
			std::this_thread::sleep_for(5s);
			//On organise le premier départ
			cout << "Depart de la rame 3 de la station 1" << endl;
			metro3.depart_station(1);
			//Boucle infinie permettant de réaliser le mouvement du métro
			while (!stopped) {
				//On fait une pause dans le thread afin de simuler un déplacement du métro
				std::this_thread::sleep_for(0.2s);
				//On récupère la position du metro, sa vitesse ainsi que la station d'arret suivante
				int pourcent = metro3.get_position();
				int vit = metro3.get_vitesse(); 
				int numero_station_suivante3 = metro3.get_prochain_arret();
				
				//On récupère les pannes des autres metros, si une est active on arrête le métro pour 30 secondes
				if (panne1) {
					cout << "Panne de la rame 1, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (panne2) {
					cout << "Panne de la rame 2, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				//si elle n'a pas atteint la station, donc que l'on doit encore se déplacer
				if (pourcent < 100) { 
					//On utilise une variable aléatoire permettant de créer une panne
					//Si une panne est crée, on va stopper tous les métros pendant 30 secondes
					std::uniform_int_distribution<int> chance_panne{ 0,999 };
					int panne_pot = chance_panne(re);
					if (panne_pot == 792) {
						panne3 = true;
						cout << "La rame 3 vient de tomber en panne ! Le probleme est en train d'etre traite" << endl;
						std::this_thread::sleep_for(30s);
						cout << "La rame 3 est de nouveau operationnelle ! " << endl;
						panne3 = false;
					}
					//S'il n'y a pas de panne, alors on déplace le métro et on l'affiche
					cout << "Progression de la rame 3 : " << pourcent << " %" << "		numero de station suivante : " << numero_station_suivante3 << endl; //affichage de sa progression
					metro3.set_position(pourcent + vit); //déplacement

					//Affichage du wagon
					//S'il est dans le sens des gare croissants
					if (metro3.reverse() == false) { 
						metro3.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante3 - 2] + pourcent * pow(10, -2) * taille_voie[numero_station_suivante3 - 2] - (taille_metro[0] * pow(10, -1)) / 2, posY_voie1 - taille_metro[1] * pow(10, -1)));

					}
					//S'il est dans le sens des gare décroissants
					else { 
						metro3.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante3 - 1] + taille_voie[numero_station_suivante3 - 1] * (1 - pourcent * pow(10, -2)) + (taille_metro[0] * pow(10, -1)) / 2, posY_voie2 + taille_metro[1] * pow(10, -1)));
						metro3.RotationMetro(180);
					}
					if (pourcent < 75 && vit < 5) { //tant qu'on a pas atteint la vitesse de croisière (limitée à une certaine portion du trajet)
						metro3.acceleration(1); //la rame accélère jusqu'à l'atteindre
					}
					if (pourcent > 80 && vit > 1) { //a l'approche d'une gare
						metro3.freinage(1); //on fait freiner la rame
					}
				}
				//Sinon, c'est qu'il est arrivé à un station
				else {
					//On arrête le metro et on indique qu'il est arrivé à la station, enfin on réinitialise sa position
					metro3.freinage(vit);
					metro3.arrivee_station(size); 
					metro3.set_position(0); 
					//On récupère la station que l'on vient d'atteindre afin de gérer le flux de passager
					int stat_nom = metro3.get_station();
					station stat_actu = liste_station.at(stat_nom - 1);
					stat_actu.arrivage_train();
					cout << "Arrivee de la rame 3 a la station numero " << stat_nom << endl;
					//On récupère le nombre de passagers dans le metro et sur le quai
					int passagers = metro3.get_passager_dedans();
					int aquai = stat_actu.get_passager(); //récupération du nombre de passagers à bord et à quai
					//on vérifie que l'on est pas à un terminus
					if (stat_nom == liste_station.size() || (stat_nom == 1 && metro1.reverse())) { //si terminus
						//Si c'est un terminus
						cout << "Fin de trajet, preparation du demi-tour." << endl;
						//On fait descendre tous les passagers
						cout << "Descente des " << passagers << " passagers restants de la rame 3." << endl;
						//On fait attendre le metro 3 secondes, le temps d'être sur le quai
						std::this_thread::sleep_for(3s);
						//On ajoute les passagers au quai
						metro3.baisse_passager_dedans(passagers);
						aquai += passagers;
						stat_actu.set_passager(aquai);
						//On effectue le demi tour au metro
						metro3.demi_tour();
						cout << "Passage par la voie de demi-tour." << endl;
						std::this_thread::sleep_for(5s);
						cout << "Demi-tour effectue." << endl;
						//On fait monter un nombre aléatoire de passager
						std::uniform_int_distribution<int> montee_terminus{ 0, aquai };//montée de passagers
						int montee = montee_terminus(re);
						metro3.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Montee de " << montee << " passagers dans la rame 3." << endl;
						//On fait attendre le metro 3 secondes, le temps d'être sur le quai
						std::this_thread::sleep_for(3s);
						cout << "Depart de la rame 3 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro3.get_prochain_arret() == metro1.get_prochain_arret() || metro3.get_prochain_arret() == metro1.get_station()) && metro1.reverse() == metro3.reverse()) || ((metro3.get_prochain_arret() == metro2.get_prochain_arret() || metro3.get_prochain_arret() == metro2.get_station()) && metro2.reverse() == metro3.reverse())){							
							//On ne fait pas redémarrer le metro tant qu'une rame bloque la voie
							cout << "Metro 3 bloqué en raison d'une autre rame de métro présente à la station suivante" << metro3.get_prochain_arret() << metro2.get_prochain_arret() << metro1.get_prochain_arret() << metro2.get_station() << metro1.get_station() << endl;
						}
						//On fait redémarrer le metro (qui sera en sens inverse)
						metro3.depart_station(vit);
						stat_actu.depart_train();
					}
					//Si ce n'est pas un terminus
					else {
						//On initialise aléatoirement le nombre de passagers qui vont descendre
						std::uniform_int_distribution<int> descente_pif{ 0, passagers }; //descente d'un nombre aléatoire de passagers de la rame (au moins 1)
						int descente = descente_pif(re);
						cout << "Descente de " << descente << " passagers de la rame 3." << endl;
						//On fait attendre le metro 3 secondes pour simuler l'arret à un quai
						std::this_thread::sleep_for(3s);
						//On fait descendre les passagers
						metro3.baisse_passager_dedans(descente);
						aquai += descente;
						stat_actu.set_passager(aquai);
						//On initialise un nombre aléatoire de passagers qui vont monter dans le train
						std::uniform_int_distribution<int> montee_pif{ 0,aquai };
						int montee = montee_pif(re);
						cout << "Montee de " << montee << " passagers dans la rame 3." << endl;
						
						metro3.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Depart de la rame 3 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro3.get_prochain_arret() == metro1.get_prochain_arret() || metro3.get_prochain_arret() == metro1.get_station()) && metro1.reverse() == metro3.reverse()) || ((metro3.get_prochain_arret() == metro2.get_prochain_arret() || metro3.get_prochain_arret() == metro2.get_station())  && metro2.reverse() == metro3.reverse())) {
							//On ne fait pas redémarrer le metro tant qu'une rame bloque la voie
							cout << "Metro 3 bloqué en raison d'une autre rame de métro présente à la station suivante" << metro3.get_prochain_arret() << metro2.get_prochain_arret() << metro1.get_prochain_arret() << metro2.get_station() << metro1.get_station() << endl;
						}
						//On fait redémarrer le metro
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
	//Affichage des voies de demi tour
	sf::RectangleShape demi_tour1, demi_tour2;
	demi_tour1.setPosition(sf::Vector2f(PosXGare[0], PosYGare + 25));
	demi_tour2.setPosition(sf::Vector2f(PosXGare[8] + 50, PosYGare + 25));
	demi_tour1.setSize(sf::Vector2f(2, 90));
	demi_tour2.setSize(sf::Vector2f(2, 90));
	demi_tour1.setRotation(90);
	demi_tour2.setRotation(270);
	demi_tour1.setFillColor(sf::Color(0, 0, 0));
	demi_tour2.setFillColor(sf::Color(0, 0, 0));


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
		window.draw(demi_tour1);
		window.draw(demi_tour2);
		window.display();
	}
}

