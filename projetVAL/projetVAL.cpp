// projetVAL.cpp : définit le point d'entrée de l'application.

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

//Initialisation des setters de la classe station
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

//Initialisation de méthodes pour l'affichages des gares
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

//Initialisation de méthodes pour l'affichages des voies
void station::ChangementTailleVoie1(const sf::Vector2f taille) {
	this->voie1.setSize(taille);
}
void station::ChangementTailleVoie2(const sf::Vector2f taille) {
	this->voie2.setSize(taille);
}
void station::RotationVoie(const int n) {
	this->voie1.rotate(n);
	this->voie2.rotate(n);
}
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


//Initialisation des getters de la classe metro
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
const bool metro::reverse() {
	return this->sens;
}
//Initialisation des setters de la classe metro
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


//Initialisation de méthodes pour l'affichages des metros
void metro::affichage_demi_tour(int i) {
	if (this->sens) {
		this->ChangementPositionMetro(sf::Vector2f(100 - 10 * i, 250));
	}
	else {
		this->ChangementPositionMetro(sf::Vector2f(1390 + 50 + 10 * i, 200));
	}

}
void metro::ChangementTextureWagon(const sf::Texture& Text) {
	this->wagon.setTexture(Text);
}

void metro::ChangementPositionMetro(const sf::Vector2f& pos) {
	this->wagon.setPosition(pos);
}
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
	//Initialisation du caractère tabulation ainsi que du nombre de répétition afin de créer 3 colonnes dans la console
	char Tab = '\t';
	int Col2 = 7;
	int Col3 = 14;

	//Définition des tailles et positions des voies
	const int posX_voie[8] = { 130,280,480,580,780,930,1105,1280 };
	const int posY_voie1 = 200;
	const int posY_voie2 = 250;
	const int taille_voie[8] = { 150,200,100,200,150,175,180,140 };
	const int taille_metro[2] = { 512, 296 };
	const int PosXGare[9] = { 100,250,450,550,750,900,1075,1250,1390 };
	const int PosYGare = 200;

	//seed aléatoire pour la montée et descente des passagers
	std::default_random_engine re(time(0)); 

	//Initialisation liste de stations et instalation de stations de métro dans notre système
	vector<station> liste_station;
	for (int i = 1; i < 10; i++) {
		liste_station.push_back(station(i, 10));
	}

	//nombre de stations créées
	int size = liste_station.size();

	//Initialisation des rames de métro (3 rames)
	metro metro1 = metro(0, 1, 0, 0, 0, 1);
	metro metro2 = metro(0, 2, 0, 0, 0, 2);
	metro metro3 = metro(0, 3, 0, 0, 0, 3);

	//Initialisation de booleens  pour créer les pannes
	bool stopped = false;
	bool panne1 = false;
	bool panne2 = false;
	bool panne3 = false;

	//Initialisation des textures de l'affichage des pannes
	sf::Texture TexturePanne1, TexturePanne2, TexturePanne3;
	if (!TexturePanne1.loadFromFile(std::string("C:/Program Files/SFML/img/Panne1.png")) || !TexturePanne2.loadFromFile(std::string("C:/Program Files/SFML/img/Panne2.png")) || !TexturePanne3.loadFromFile(std::string("C:/Program Files/SFML/img/Panne3.png")))
	{
		cerr << "Erreur pendant le chargement des images" << endl;

	}

	//Initialisation des textures de gare et de métro
	sf::Texture TextureGare, TextureWagon;
	if (!TextureGare.loadFromFile(std::string("C:/Program Files/SFML/img/gare.png")) || !TextureWagon.loadFromFile(std::string("C:/Program Files/SFML/img/RameMetro2.png")))
	{
		cerr << "Erreur pendant le chargement des images" << endl;

	}
	
	//Initialisation des sprites d'indicateurs de pannes
	sf::Sprite ImgPanne1, ImgPanne2, ImgPanne3;

	//Initialisation des voies de demi tour
	sf::RectangleShape demi_tour1, demi_tour2;

	//Initialisation et lancement du thread de la rame 1
	std::jthread rame1(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &metro3, &posX_voie, &posY_voie1, &posY_voie2, &taille_voie, &taille_metro, &panne1, &panne2, &panne3,&ImgPanne1]
		{
			cout << "Rame 1 prete" << endl;
			//On effectue un premier affichage du metro
			metro1.ChangementPositionMetro(sf::Vector2f(posX_voie[0], posY_voie1 - 30));
			//On indique qu'il est dans la gare
			metro1.arrivee_station(size);
			//On fait monter des passagers
			int aquai = liste_station.at(metro1.get_station() - 1).get_passager();
			std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai
			int montee = montee_pif(re);
			cout << "Montee de " << montee << " passagers dans la rame 1." << endl;
			//Le temps d'attente dans une gare est de 3 secondes
			std::this_thread::sleep_for(3s);
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
					std::uniform_int_distribution<int> chance_panne{ 0,1000 };
					int panne_pot = chance_panne(re);
					if (panne_pot == 449) {
						panne1 = true;
						cout << "Panne de la rame 1 !" << endl;
						//Affichage de l'indicateur Panne 1
						ImgPanne1.setColor(sf::Color(255, 255, 255, 255));
						std::this_thread::sleep_for(30s);
						cout << "La rame 1 est de nouveau operationnelle ! " << endl;
						//On enleve l'indicateur Panne 1
						ImgPanne1.setColor(sf::Color(255, 255, 255, 0));
						panne1 = false;
					}
					//S'il n'y a pas de panne, alors on déplace le métro et on l'affiche
					cout << "Progression de la rame 1 : " << pourcent << " %" << endl; //affichage de sa progression
					metro1.set_position(pourcent + vit); //déplacement
					
					//Affichage du wagon :
					//S'il est dans le sens des gare croissants
					if (metro1.reverse() == false) {
						metro1.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante1 - 2] + pourcent * pow(10, -2) * taille_voie[numero_station_suivante1 - 2] - (taille_metro[0] * pow(10, -1)) / 2, posY_voie1 - taille_metro[1] * pow(10, -1)));
						metro1.RotationMetro(0);
					}
					//S'il est dans le sens des gare décroissants
					else { 
						metro1.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante1 - 1] + taille_voie[numero_station_suivante1 - 1] * (1 - pourcent * pow(10, -2)) + (taille_metro[0] * pow(10, -1)) / 2, posY_voie2 + taille_metro[1] * pow(10, -1)));
						metro1.RotationMetro(180);
					}
					if (vit < 7) { //tant qu'on a pas atteint la vitesse de croisière 
						metro1.acceleration(3); //la rame accélère jusqu'à l'atteindre
					}
					if(pourcent>75 && vit > 1){ //a l'approche d'une gare
						metro1.freinage(3); //on fait freiner la rame
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
						for (int i = 1; i < 6; i++) {
							metro1.affichage_demi_tour(i);
							std::this_thread::sleep_for(0.2s);
						}
						for (int i = 5; i > 0; i--) {
							metro1.affichage_demi_tour(i);
							std::this_thread::sleep_for(0.2s);
						}
						metro1.demi_tour();
						cout << "Passage par la voie de demi-tour." << endl;
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
							cout << "Metro 1 bloque car la voie est deja prise" << endl;
							std::this_thread::sleep_for(0.2s);
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
							cout << "Metro 1 bloque car la voie est deja prise" << endl;
							std::this_thread::sleep_for(0.2s);
						}
						//On fait redémarrer le metro
						metro1.depart_station(vit);
						stat_actu.depart_train();
					}
				}
			}
		}
	);
	
	//Initialisation et lancement du thread de la rame 2
	std::jthread rame2(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &metro3, &posX_voie, &posY_voie1, &posY_voie2, &taille_voie, &taille_metro, &panne1, &panne2, &panne3, &Col2, &Tab, &ImgPanne2]
		{
			cout << string(Col2, Tab) << "Rame 2 prete" << endl;
			//On effectue un premier affichage du metro
			metro2.ChangementPositionMetro(sf::Vector2f(posX_voie[1], posY_voie1 - 30));
			//On indique qu'il est dans la gare
			metro2.arrivee_station(size);
			//On fait monter des passagers
			int aquai = liste_station.at(metro2.get_station() - 1).get_passager();
			std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai
			int montee = montee_pif(re);
			cout << string(Col2, Tab) << "Montee de " << montee << " passagers dans la rame 2." << endl;
			//Le temps d'attente dans une gare est de 3 secondes
			std::this_thread::sleep_for(3s);
			//On organise le premier départ
			cout << string(Col2, Tab) << "Depart de la rame 2 de la station 2" << endl;
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
					cout << string(Col2, Tab) << "Panne de la rame 1, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (panne3) {
					cout << string(Col2, Tab) << "Panne de la rame 3, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}

				//si elle n'a pas atteint la station, donc que l'on doit encore se déplacer
				if (pourcent < 100) { 
					//On utilise une variable aléatoire permettant de créer une panne
					//Si une panne est crée, on va stopper tous les métros pendant 30 secondes
					std::uniform_int_distribution<int> chance_panne{ 0,1000 };
					int panne_pot = chance_panne(re);
					if (panne_pot == 16) {
						panne2 = true;
						cout << string(Col2, Tab) << "Panne de la rame 2 !" << endl;
						//Affichage de l'indicateur Panne 2
						ImgPanne2.setColor(sf::Color(255, 255, 255, 255));
						std::this_thread::sleep_for(30s);
						cout << string(Col2, Tab) << "La rame 2 est de nouveau operationnelle" << endl;
						//On cache l'indicateur Panne 2
						ImgPanne2.setColor(sf::Color(255, 255, 255, 0));
						panne2 = false;
					}
					//S'il n'y a pas de panne, alors on déplace le métro et on l'affiche
					cout << string(Col2, Tab) << "Progression de la rame 2 : " << pourcent << " %" << endl; //affichage de sa progression
					metro2.set_position(pourcent + vit); //déplacement
					
					//Affichage du wagon
					//S'il est dans le sens des gare croissants
					if (metro2.reverse() == false) { 
						metro2.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante2 - 2] + pourcent * pow(10, -2) * taille_voie[numero_station_suivante2 - 2] - (taille_metro[0] * pow(10, -1)) / 2, posY_voie1 - taille_metro[1] * pow(10, -1)));
						metro2.RotationMetro(0);
					}
					//S'il est dans le sens des gare décroissants
					else { 
						metro2.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante2 - 1] + taille_voie[numero_station_suivante2 - 1] * (1 - pourcent * pow(10, -2)) + (taille_metro[0] * pow(10, -1)) / 2, posY_voie2 + taille_metro[1] * pow(10, -1)));
						metro2.RotationMetro(180);
					}
					if (vit < 7) { //tant qu'on a pas atteint la vitesse de croisière
						metro2.acceleration(3); //la rame accélère jusqu'à l'atteindre
					}
					if (pourcent > 75 && vit > 1) { //a l'approche d'une gare
						metro2.freinage(3); //on fait freiner la rame
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
					cout << string(Col2, Tab) << "Arrivee de la rame 2 a la station numero " << stat_nom << endl;
					//On récupère le nombre de passagers dans le metro et sur le quai
					int passagers = metro2.get_passager_dedans();
					int aquai = stat_actu.get_passager(); //récupération du nombre de passagers à bord et à quai
					//on vérifie que l'on est pas à un terminus
					if (stat_nom == liste_station.size() || (stat_nom == 1 && metro2.reverse())) { //si terminus
						//Si c'est un terminus
						cout << string(Col2, Tab) << "Fin de trajet, preparation du demi-tour." << endl;
						//On fait descendre tous les passagers
						cout << string(Col2, Tab) << "Descente des " << passagers << " passagers restants de la rame 2." << endl;
						//On fait attendre le metro 3 secondes, le temps d'être sur le quai
						std::this_thread::sleep_for(3s);
						metro2.baisse_passager_dedans(passagers); 
						//On ajoute les passagers au quai
						aquai += passagers;
						stat_actu.set_passager(aquai);
						//On effectue le demi tour au metro
						for (int i = 1; i < 6; i++) {
							metro2.affichage_demi_tour(i);
							std::this_thread::sleep_for(0.2s);
						}
						for (int i = 5; i > 0; i--) {
							metro2.affichage_demi_tour(i);
							std::this_thread::sleep_for(0.2s);
						}
						metro2.demi_tour();
						cout << string(Col2, Tab) << "Passage par la voie de demi-tour." << endl;
						
						cout << string(Col2, Tab) << "Demi-tour effectue." << endl;
						//On fait monter un nombre aléatoire de passagers
						std::uniform_int_distribution<int> montee_terminus{ 0, aquai };
						int montee = montee_terminus(re);
						metro2.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << string(Col2, Tab) << "Montee de " << montee << " passagers dans la rame 2." << endl;
						//On fait attendre le metro 3 secondes, le temps d'être sur le quai
						std::this_thread::sleep_for(3s);
						cout << string(Col2, Tab) << "Depart de la rame 2 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro2.get_prochain_arret() == metro1.get_prochain_arret() || metro2.get_prochain_arret() == metro1.get_station())  && metro1.reverse() == metro2.reverse()) || ((metro2.get_prochain_arret() == metro3.get_prochain_arret() || metro2.get_prochain_arret() == metro3.get_station())  && metro2.reverse() == metro3.reverse())) {
							//On ne fait pas redémarrer le metro tant qu'une rame bloque la voie
							cout << string(Col2, Tab) << "Metro 2 bloque car la voie est deja prise" << endl;
							std::this_thread::sleep_for(0.2s);
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
						cout << string(Col2, Tab) << "Descente de " << descente << " passagers de la rame 2." << endl;
						//On fait attendre le metro 3 secondes pour simuler l'arret à un quai
						std::this_thread::sleep_for(3s);
						//On fait descendre les passagers
						metro2.baisse_passager_dedans(descente);
						aquai += descente;
						stat_actu.set_passager(aquai);
						//On initialise un nombre aléatoire de passagers qui vont monter dans le train
						std::uniform_int_distribution<int> montee_pif{ 0,aquai };
						int montee = montee_pif(re);
						cout << string(Col2, Tab) << "Montee de " << montee << " passagers dans la rame 2." << endl;
						
						metro2.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << string(Col2, Tab) << "Depart de la rame 1 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro2.get_prochain_arret() == metro1.get_prochain_arret() || metro2.get_prochain_arret() == metro1.get_station()) && metro1.reverse() == metro2.reverse()) || ((metro2.get_prochain_arret() == metro3.get_prochain_arret() || metro2.get_prochain_arret() == metro3.get_station()) && metro2.reverse() == metro3.reverse())) {
							//On ne fait pas redémarrer le metro tant qu'une rame bloque la voie
							cout << string(Col2, Tab) << "Metro 2 bloque car la voie est deja prise" << endl;
							std::this_thread::sleep_for(0.2s);
						}
						//On fait redémarrer le metro
						metro2.depart_station(vit);
						stat_actu.depart_train();
					}
				}
			}
		}
	);
	
	//Initialisation et lancement du thread de la rame 3
	std::jthread rame3(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &metro3, &posX_voie, &posY_voie1, &posY_voie2, &taille_voie, &taille_metro, &panne1, &panne2, &panne3, &Col3, &Tab, &ImgPanne3]
		{
			cout << string(Col3, Tab) << "Rame 3 prete" << endl;
			//On effectue un premier affichage du metro
			metro3.ChangementPositionMetro(sf::Vector2f(posX_voie[2], posY_voie1 - 30));
			//On indique qu'il est dans la gare
			metro3.arrivee_station(size);
			//On fait monter des passagers
			int aquai = liste_station.at(metro1.get_station() - 1).get_passager();
			std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai
			int montee = montee_pif(re);
			cout << string(Col3, Tab) << "Montee de " << montee << " passagers dans la rame 3." << endl;
			//Le temps d'attente dans une gare est de 3 secondes
			std::this_thread::sleep_for(3s);
			//On organise le premier départ
			cout << string(Col3, Tab) << "Depart de la rame 3 de la station 1" << endl;
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
					cout << string(Col3, Tab) << "Panne de la rame 1, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				if (panne2) {
					cout << string(Col3, Tab) << "Panne de la rame 2, trafic perturbe" << endl;
					std::this_thread::sleep_for(30s);
				}
				//si elle n'a pas atteint la station, donc que l'on doit encore se déplacer
				if (pourcent < 100) { 
					//On utilise une variable aléatoire permettant de créer une panne
					//Si une panne est crée, on va stopper tous les métros pendant 30 secondes
					std::uniform_int_distribution<int> chance_panne{ 0,1000 };
					int panne_pot = chance_panne(re);
					if (panne_pot == 792) {
						panne3 = true;
						cout << string(Col3, Tab) << "Panne de la rame 3 !" << endl;
						//Affichage de l'indicateur Panne 3
						ImgPanne3.setColor(sf::Color(255, 255, 255, 255));
						std::this_thread::sleep_for(30s);
						cout << string(Col3, Tab) << "La rame 3 est de nouveau operationnelle ! " << endl;
						//On cache l'indicateur Panne 3
						ImgPanne3.setColor(sf::Color(255, 255, 255, 0));
						panne3 = false;
					}
					//S'il n'y a pas de panne, alors on déplace le métro et on l'affiche
					cout << string(Col3, Tab) << "Progression de la rame 3 : " << pourcent << " %" << endl; //affichage de sa progression
					metro3.set_position(pourcent + vit); //déplacement

					//Affichage du wagon
					//S'il est dans le sens des gare croissants
					if (metro3.reverse() == false) { 
						metro3.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante3 - 2] + pourcent * pow(10, -2) * taille_voie[numero_station_suivante3 - 2] - (taille_metro[0] * pow(10, -1)) / 2, posY_voie1 - taille_metro[1] * pow(10, -1)));
						metro3.RotationMetro(0);
					}
					//S'il est dans le sens des gare décroissants
					else { 
						metro3.ChangementPositionMetro(sf::Vector2f(posX_voie[numero_station_suivante3 - 1] + taille_voie[numero_station_suivante3 - 1] * (1 - pourcent * pow(10, -2)) + (taille_metro[0] * pow(10, -1)) / 2, posY_voie2 + taille_metro[1] * pow(10, -1)));
						metro3.RotationMetro(180);
					}
					if (vit < 7) { //tant qu'on a pas atteint la vitesse de croisière
						metro3.acceleration(3); //la rame accélère jusqu'à l'atteindre
					}
					if (pourcent > 75 && vit > 1) { //a l'approche d'une gare
						metro3.freinage(3); //on fait freiner la rame
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
					cout << string(Col3, Tab) << "Arrivee de la rame 3 a la station numero " << stat_nom << endl;
					//On récupère le nombre de passagers dans le metro et sur le quai
					int passagers = metro3.get_passager_dedans();
					int aquai = stat_actu.get_passager(); //récupération du nombre de passagers à bord et à quai
					//on vérifie que l'on est pas à un terminus
					if (stat_nom == liste_station.size() || (stat_nom == 1 && metro1.reverse())) { //si terminus
						//Si c'est un terminus
						cout << string(Col3, Tab) << "Fin de trajet, preparation du demi-tour." << endl;
						//On fait descendre tous les passagers
						cout << string(Col3, Tab) << "Descente des " << passagers << " passagers restants de la rame 3." << endl;
						//On fait attendre le metro 3 secondes, le temps d'être sur le quai
						std::this_thread::sleep_for(3s);
						//On ajoute les passagers au quai
						metro3.baisse_passager_dedans(passagers);
						aquai += passagers;
						stat_actu.set_passager(aquai);
						//On effectue le demi tour au metro
						for (int i = 1; i < 6; i++) {
							metro3.affichage_demi_tour(i);
							std::this_thread::sleep_for(0.2s);
						}
						for (int i = 5; i > 0; i--) {
							metro3.affichage_demi_tour(i);
							std::this_thread::sleep_for(0.2s);
						}
						metro3.demi_tour();
						cout << string(Col3, Tab) << "Passage par la voie de demi-tour." << endl;
					
						cout << string(Col3, Tab) << "Demi-tour effectue." << endl;
						//On fait monter un nombre aléatoire de passager
						std::uniform_int_distribution<int> montee_terminus{ 0, aquai };//montée de passagers
						int montee = montee_terminus(re);
						metro3.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << string(Col3, Tab) << "Montee de " << montee << " passagers dans la rame 3." << endl;
						//On fait attendre le metro 3 secondes, le temps d'être sur le quai
						std::this_thread::sleep_for(3s);
						cout << string(Col3, Tab) << "Depart de la rame 3 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro3.get_prochain_arret() == metro1.get_prochain_arret() || metro3.get_prochain_arret() == metro1.get_station()) && metro1.reverse() == metro3.reverse()) || ((metro3.get_prochain_arret() == metro2.get_prochain_arret() || metro3.get_prochain_arret() == metro2.get_station()) && metro2.reverse() == metro3.reverse())){							
							//On ne fait pas redémarrer le metro tant qu'une rame bloque la voie
							cout << string(Col3, Tab) << "Metro 3 bloque car la voie est deja prise" << endl;
							std::this_thread::sleep_for(0.2s);
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
						cout << string(Col3, Tab) << "Descente de " << descente << " passagers de la rame 3." << endl;
						//On fait attendre le metro 3 secondes pour simuler l'arret à un quai
						std::this_thread::sleep_for(3s);
						//On fait descendre les passagers
						metro3.baisse_passager_dedans(descente);
						aquai += descente;
						stat_actu.set_passager(aquai);
						//On initialise un nombre aléatoire de passagers qui vont monter dans le train
						std::uniform_int_distribution<int> montee_pif{ 0,aquai };
						int montee = montee_pif(re);
						cout << string(Col3, Tab) << "Montee de " << montee << " passagers dans la rame 3." << endl;
						
						metro3.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << string(Col3, Tab) << "Depart de la rame 3 de la station " << stat_nom << endl;
						//On vérifie s'il n'y a pas déjà une rame de présente sur le parcours
						while (((metro3.get_prochain_arret() == metro1.get_prochain_arret() || metro3.get_prochain_arret() == metro1.get_station()) && metro1.reverse() == metro3.reverse()) || ((metro3.get_prochain_arret() == metro2.get_prochain_arret() || metro3.get_prochain_arret() == metro2.get_station())  && metro2.reverse() == metro3.reverse())) {
							//On ne fait pas redémarrer le metro tant qu'une rame bloque la voie
							cout << string(Col3, Tab) << "Metro 3 bloque car la voie est deja prise" << endl;
							std::this_thread::sleep_for(0.2s);
						}
						//On fait redémarrer le metro
						metro3.depart_station(vit); //reprise du trajet
						stat_actu.depart_train();
					}
				}
			}
		}
	);
	
	//Partie affichage Sur une Console :

	//création d'une nouvelle console d'affichage
	sf::RenderWindow window(sf::VideoMode(1600, 500), "Visualisation du métro Lillois");

	//Modification de la texture des rames de métro
	metro1.ChangementTextureWagon(TextureWagon);
	metro2.ChangementTextureWagon(TextureWagon);
	metro3.ChangementTextureWagon(TextureWagon);

	//Modification de la taille des métros
	metro1.ChangementTailleMetro(sf::Vector2f(0.1, 0.1));
	metro2.ChangementTailleMetro(sf::Vector2f(0.1, 0.1));
	metro3.ChangementTailleMetro(sf::Vector2f(0.1, 0.1));

	//Modification de la texture , de la taille et de la position de chacune des gares
	for (int i = 0; i < liste_station.size(); i++) {
		liste_station[i].ChangementTextureGare(TextureGare);
		liste_station[i].ChangementTailleGare(sf::Vector2f(0.1, 0.1));
		liste_station[i].ChangementPositionGare(sf::Vector2f(PosXGare[i], PosYGare));

	}

	//Mise à jour des positions des voies et de leurs tailles
	for (int i = 0; i < 8; i++) {
		liste_station[i].ChangementPositionVoie1(sf::Vector2f(posX_voie[i], posY_voie1));
		liste_station[i].ChangementPositionVoie2(sf::Vector2f(posX_voie[i], posY_voie2));
		liste_station[i].ChangementTailleVoie1(sf::Vector2f(2, taille_voie[i]));
		liste_station[i].ChangementTailleVoie2(sf::Vector2f(2, taille_voie[i]));
		liste_station[i].RotationVoie(270);
	}

	//Modification de la position et de la taille (du sens et de la couleur) des voies de demi tour
	demi_tour1.setPosition(sf::Vector2f(PosXGare[0], PosYGare + 25));
	demi_tour2.setPosition(sf::Vector2f(PosXGare[8] + 50, PosYGare + 25));
	demi_tour1.setSize(sf::Vector2f(2, 90));
	demi_tour2.setSize(sf::Vector2f(2, 90));
	demi_tour1.setRotation(90);
	demi_tour2.setRotation(270);
	demi_tour1.setFillColor(sf::Color(0, 0, 0));
	demi_tour2.setFillColor(sf::Color(0, 0, 0));


	//Initialisation de la position des indicateurs de panne
	ImgPanne1.setTexture(TexturePanne1);
	ImgPanne2.setTexture(TexturePanne2);
	ImgPanne3.setTexture(TexturePanne3);
	ImgPanne1.setPosition(sf::Vector2f(600, 350));
	ImgPanne2.setPosition(sf::Vector2f(750, 350));
	ImgPanne3.setPosition(sf::Vector2f(900, 350));
	ImgPanne1.setScale(sf::Vector2f(0.2, 0.2));
	ImgPanne2.setScale(sf::Vector2f(0.2, 0.2));
	ImgPanne3.setScale(sf::Vector2f(0.2, 0.2));
	ImgPanne1.setColor(sf::Color(255, 255, 255, 0));
	ImgPanne2.setColor(sf::Color(255, 255, 255, 0));
	ImgPanne3.setColor(sf::Color(255, 255, 255, 0));

	//Affichage  sur la console
	while (window.isOpen())
	{
		//Création de l'évenement fermeture de la console
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Réinitialisation de la console afin de pouvoir créer du mouvement
		window.clear(sf::Color(255, 255, 255));

		//Affichage des gares ainsi que des voies entre elles
		for (int i = 0; i < liste_station.size(); i++) {
			liste_station[i].AffichageGare(window);
			liste_station[i].AffichageVoies(window);
		}

		//Affichage des métros
		metro1.AffichageMetro(window);
		metro2.AffichageMetro(window);
		metro3.AffichageMetro(window);

		//Affichage des indicateurs de pannes
		window.draw(ImgPanne1);
		window.draw(ImgPanne2);
		window.draw(ImgPanne3);

		//Affichage des voies de demi tour
		window.draw(demi_tour1);
		window.draw(demi_tour2);

		//Affichage de la console
		window.display();
	}
}