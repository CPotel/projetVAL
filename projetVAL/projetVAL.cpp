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
	if ((!this->sens && this->station < taille_ligne) || (this->sens && this->station <= 1)) {
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
	for (int i = 1; i < 11; i++) {
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
					if (stat_nom == liste_station.size() || (stat_nom == 1 && rame1.reverse())) { //si terminus
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
	
	//affichage sur une console :
	sf::RenderWindow window(sf::VideoMode(1600, 900), "Visualisation du métro Lillois");
	

	sf::Texture TextureGare;
	sf::Sprite Gare1, Gare2, Gare3, Gare4, Gare5, Gare6, Gare7, Gare8, Gare9, Gare10;
	sf::RectangleShape Ligne1Gare1Gare2, Ligne1Gare2Gare3, Ligne1Gare3Gare4, Ligne1Gare4Gare5, Ligne1Gare5Gare6, Ligne1Gare6Gare7, Ligne1Gare7Gare8, Ligne1Gare8Gare9, Ligne1Gare9Gare10, Ligne2Gare1Gare2, Ligne2Gare2Gare3, Ligne2Gare3Gare4, Ligne2Gare4Gare5, Ligne2Gare5Gare6, Ligne2Gare6Gare7, Ligne2Gare7Gare8, Ligne2Gare8Gare9, Ligne2Gare9Gare10;
	sf::CircleShape AllerRetour1(30), AllerRetour2(30);
	if (!TextureGare.loadFromFile(std::string("C:/Program Files/SFML/img/gare.png")))
	{
		cerr << "Erreur pendant le chargement des images" << endl;
		//return EXIT_FAILURE;
	}
	//On mets à toutes les gares la même texture
	
	Gare1.setTexture(TextureGare);
	Gare2.setTexture(TextureGare);
	Gare3.setTexture(TextureGare);
	Gare4.setTexture(TextureGare);
	Gare5.setTexture(TextureGare);
	Gare6.setTexture(TextureGare);
	Gare7.setTexture(TextureGare);
	Gare8.setTexture(TextureGare);
	Gare9.setTexture(TextureGare);
	Gare10.setTexture(TextureGare);
	//MaJ des positions et de la tailles des différentes gares
	//Les gares :
	Gare1.setPosition(sf::Vector2f(125, 100));
	Gare1.setScale(sf::Vector2f(0.1, 0.1));
	Gare2.setPosition(sf::Vector2f(225, 300));
	Gare2.setScale(sf::Vector2f(0.1, 0.1));
	Gare3.setPosition(sf::Vector2f(350, 600));
	Gare3.setScale(sf::Vector2f(0.1, 0.1));
	Gare4.setPosition(sf::Vector2f(550, 750));
	Gare4.setScale(sf::Vector2f(0.1, 0.1));
	Gare5.setPosition(sf::Vector2f(750, 750));
	Gare5.setScale(sf::Vector2f(0.1, 0.1));
	Gare6.setPosition(sf::Vector2f(900, 600));
	Gare6.setScale(sf::Vector2f(0.1, 0.1));
	Gare7.setPosition(sf::Vector2f(1000, 400));
	Gare7.setScale(sf::Vector2f(0.1, 0.1));
	Gare8.setPosition(sf::Vector2f(1100, 250));
	Gare8.setScale(sf::Vector2f(0.1, 0.1));
	Gare9.setPosition(sf::Vector2f(1300, 350));
	Gare9.setScale(sf::Vector2f(0.1, 0.1));
	Gare10.setPosition(sf::Vector2f(1450, 550));
	Gare10.setScale(sf::Vector2f(0.1, 0.1));
	//gare 1 --> gare 2
	Ligne1Gare1Gare2.setPosition(sf::Vector2f(150, 150));
	Ligne1Gare1Gare2.setSize(sf::Vector2f(2, 195));
	Ligne1Gare1Gare2.rotate(335);
	Ligne1Gare1Gare2.setFillColor(sf::Color(0, 0, 0));
	Ligne2Gare1Gare2.setPosition(sf::Vector2f(175, 150));
	Ligne2Gare1Gare2.setSize(sf::Vector2f(2, 170));
	Ligne2Gare1Gare2.rotate(335);
	Ligne2Gare1Gare2.setFillColor(sf::Color(0, 0, 0));
	//gare 2 --> gare 3
	Ligne1Gare2Gare3.setPosition(sf::Vector2f(240, 350));
	Ligne1Gare2Gare3.setSize(sf::Vector2f(2, 304));
	Ligne1Gare2Gare3.rotate(338);
	Ligne1Gare2Gare3.setFillColor(sf::Color(0, 0, 0));
	Ligne2Gare2Gare3.setPosition(sf::Vector2f(265, 350));
	Ligne2Gare2Gare3.setSize(sf::Vector2f(2, 275));
	Ligne2Gare2Gare3.rotate(338);
	Ligne2Gare2Gare3.setFillColor(sf::Color(0, 0, 0));
	//gare 3 --> gare 4
	Ligne1Gare3Gare4.setPosition(sf::Vector2f(360, 650));
	Ligne1Gare3Gare4.setSize(sf::Vector2f(2, 233));
	Ligne1Gare3Gare4.rotate(305);
	Ligne1Gare3Gare4.setFillColor(sf::Color(0, 0, 0));
	Ligne2Gare3Gare4.setPosition(sf::Vector2f(400, 650));
	Ligne2Gare3Gare4.setSize(sf::Vector2f(2, 200));
	Ligne2Gare3Gare4.rotate(305);
	Ligne2Gare3Gare4.setFillColor(sf::Color(0, 0, 0));
	//gare 4 --> gare 5
	Ligne1Gare4Gare5.setPosition(sf::Vector2f(600, 790));
	Ligne1Gare4Gare5.setSize(sf::Vector2f(2, 155));
	Ligne1Gare4Gare5.rotate(270);
	Ligne1Gare4Gare5.setFillColor(sf::Color(0, 0, 0));
	Ligne2Gare4Gare5.setPosition(sf::Vector2f(585, 765));
	Ligne2Gare4Gare5.setSize(sf::Vector2f(2, 182));
	Ligne2Gare4Gare5.rotate(270);
	Ligne2Gare4Gare5.setFillColor(sf::Color(0, 0, 0));
	//gare 5 --> gare 6
	Ligne1Gare5Gare6.setPosition(sf::Vector2f(800, 790));
	Ligne1Gare5Gare6.setSize(sf::Vector2f(2, 200));
	Ligne1Gare5Gare6.rotate(225);
	Ligne1Gare5Gare6.setFillColor(sf::Color(0, 0, 0));
	Ligne2Gare5Gare6.setPosition(sf::Vector2f(785, 765));
	Ligne2Gare5Gare6.setSize(sf::Vector2f(2, 170));
	Ligne2Gare5Gare6.rotate(225);
	Ligne2Gare5Gare6.setFillColor(sf::Color(0, 0, 0));
	//gare 6 --> gare 7
	Ligne1Gare6Gare7.setPosition(sf::Vector2f(925, 600));
	Ligne1Gare6Gare7.setSize(sf::Vector2f(2, 175));
	Ligne1Gare6Gare7.rotate(210);
	Ligne1Gare6Gare7.setFillColor(sf::Color(0, 0, 0));
	Ligne2Gare6Gare7.setPosition(sf::Vector2f(940, 630));
	Ligne2Gare6Gare7.setSize(sf::Vector2f(2, 210));
	Ligne2Gare6Gare7.rotate(210);
	Ligne2Gare6Gare7.setFillColor(sf::Color(0, 0, 0));
	//gare 7 --> gare 8
	Ligne1Gare7Gare8.setPosition(sf::Vector2f(1030, 400));
	Ligne1Gare7Gare8.setSize(sf::Vector2f(2, 120));
	Ligne1Gare7Gare8.rotate(215);
	Ligne1Gare7Gare8.setFillColor(sf::Color(0, 0, 0));
	Ligne2Gare7Gare8.setPosition(sf::Vector2f(1045, 425));
	Ligne2Gare7Gare8.setSize(sf::Vector2f(2, 150));
	Ligne2Gare7Gare8.rotate(215);
	Ligne2Gare7Gare8.setFillColor(sf::Color(0, 0, 0));
	//gare 8 --> gare 9
	Ligne1Gare8Gare9.setPosition(sf::Vector2f(1130, 255));
	Ligne1Gare8Gare9.setSize(sf::Vector2f(2, 210));
	Ligne1Gare8Gare9.rotate(300);
	Ligne1Gare8Gare9.setFillColor(sf::Color(0, 0, 0));
	Ligne2Gare8Gare9.setPosition(sf::Vector2f(1150, 290));
	Ligne2Gare8Gare9.setSize(sf::Vector2f(2, 180));
	Ligne2Gare8Gare9.rotate(300);
	Ligne2Gare8Gare9.setFillColor(sf::Color(0, 0, 0));
	//gare 9 --> gare 10
	Ligne1Gare9Gare10.setPosition(sf::Vector2f(1325, 400));
	Ligne1Gare9Gare10.setSize(sf::Vector2f(2, 220));
	Ligne1Gare9Gare10.rotate(320);
	Ligne1Gare9Gare10.setFillColor(sf::Color(0, 0, 0));
	Ligne2Gare9Gare10.setPosition(sf::Vector2f(1350, 400));
	Ligne2Gare9Gare10.setSize(sf::Vector2f(2, 195));
	Ligne2Gare9Gare10.rotate(320);
	Ligne2Gare9Gare10.setFillColor(sf::Color(0, 0, 0));
	//Les aller retours
	/*AllerRetour1.setPosition(sf::Vector2f(100, 70));
	AllerRetour1.setOutlineThickness(2);
	AllerRetour1.setOutlineColor(sf::Color(0, 0, 0));*/
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
		window.draw(Gare1);
		window.draw(Gare2);
		window.draw(Gare3);
		window.draw(Gare4);
		window.draw(Gare5);
		window.draw(Gare6);
		window.draw(Gare7);
		window.draw(Gare8);
		window.draw(Gare9);
		window.draw(Gare10);
		window.draw(Ligne1Gare1Gare2);
		window.draw(Ligne1Gare2Gare3);
		window.draw(Ligne1Gare3Gare4);
		window.draw(Ligne1Gare4Gare5);
		window.draw(Ligne1Gare5Gare6);
		window.draw(Ligne1Gare6Gare7);
		window.draw(Ligne1Gare7Gare8);
		window.draw(Ligne1Gare8Gare9);
		window.draw(Ligne1Gare9Gare10);
		window.draw(Ligne2Gare1Gare2);
		window.draw(Ligne2Gare2Gare3);
		window.draw(Ligne2Gare3Gare4);
		window.draw(Ligne2Gare4Gare5);
		window.draw(Ligne2Gare5Gare6);
		window.draw(Ligne2Gare6Gare7);
		window.draw(Ligne2Gare7Gare8);
		window.draw(Ligne2Gare8Gare9);
		window.draw(Ligne2Gare9Gare10);
		window.display();
	}
}
