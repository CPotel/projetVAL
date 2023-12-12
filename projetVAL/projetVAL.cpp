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
	this->wagon.setRotation(n - 90);
}
int main()
{
	//Définition des tailles et positions des voies
	const int posX_voie1[9] = { 150,240,360,600,800,925,1030,1130,1325 };
	const int posY_voie1[9] = { 150,350,650,790,790,600,400,255,400 };
	const int posX_voie2[9] = { 175,265,400,585,785,940,1045,1150,1350 };
	const int posY_voie2[9] = { 150,350,650,765,765,630,425,290,400 };
	const int taille_voie1[9] = { 195,304,233,155,200,175,120,210,220 };
	const int taille_voie2[9] = { 170,275,200,182,170,210,150,180,195 };
	const int rotation_voies[9] = { 335,338,305,270,225,210,215,300,320 };
	const int taille_metro[2] = { 512, 296 };
	const int PosXGare[10] = { 125,225,350,550,750,900,1000,1100,1300,1450 };
	const int PosYGare[10] = { 100,300,600,750,750,600,400,250,350,550 };

	std::default_random_engine re(time(0)); //seed aléatoire pour la montée et descente des passagers
	//Initialisation liste de stations
	//Instalation de stations de métro dans notre système
	vector<station> liste_station;
	for (int i = 1; i < 11; i++) {
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
	metro metro2 = metro(0, 1, 0, 0, 0, 2); //création des 2 rames

	int vit_const1 = 20;
	int vit_const2 = 10; //vitesse des deux rames créees

	int size = liste_station.size(); //nombre de stations créées

	const double pi = 3.14159265358979323846;
	bool stopped = false;
	std::jthread rame1(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &vit_const1, &posX_voie1, &posY_voie1, &posX_voie2, &posY_voie2, &taille_voie1, &taille_voie2, &rotation_voies, &pi, &taille_metro]
		{
			cout << "Rame 1 prete" << endl;
			metro1.ChangementPositionMetro(sf::Vector2f(posX_voie1[0], posY_voie1[0]));
			metro1.RotationMetro(rotation_voies[0] - 180);
			metro1.arrivee_station(size);
			std::this_thread::sleep_for(5s);
			cout << "Montee de passagers depuis la station 1 pour la rame 1" << endl;
			int aquai = liste_station.at(metro1.get_station() - 1).get_passager();
			std::uniform_int_distribution<int> montee_pif{ 0,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai (au moins 1)
			int montee = montee_pif(re);
			cout << "Montee de " << montee << " passagers dans la rame 1." << endl;
			std::this_thread::sleep_for(montee * 0.25s);
			cout << "Depart de la rame 1 de la station 1" << endl;
			metro1.depart_station(5);
			while (!stopped) { //tant qu'on a pas arrêté
				std::this_thread::sleep_for(1s); //attente de 10ms pour simuler le déplacement de la rame
				int pourcent = metro1.get_position(), pourcent_autre = metro2.get_position();

				int vit = metro1.get_vitesse(), vit_autre = metro2.get_vitesse(); //récupération de la position et de la vitesse de la rame
				int numero_station_suivante = metro1.get_prochain_arret();
				cout << "numero de station : " << numero_station_suivante - 2 << endl;
				if (pourcent < 100) { //si elle n'a pas atteint la station
					cout << "Progression de la rame 1 : " << pourcent << " %" << endl; //affichage de sa progression
					metro1.set_position(pourcent + vit); //déplacement
					//Affichage du wagon
					if (metro1.reverse() == false) { //S'il est dans le sens des gare croissants
						metro1.ChangementPositionMetro(sf::Vector2f(posX_voie1[numero_station_suivante - 2] + cos(rotation_voies[numero_station_suivante - 2] * pi / 180 + pi / 2) * (taille_voie1[numero_station_suivante - 2] * pourcent * pow(10, -2) - taille_metro[0] * pow(10, -1)), posY_voie1[numero_station_suivante - 2] + sin(rotation_voies[numero_station_suivante - 2] * pi / 180 + pi / 2) * (taille_voie1[numero_station_suivante - 2] * pourcent * pow(10, -2) + taille_metro[1] * pow(10, -1))));
						cout << "X : " << posX_voie1[numero_station_suivante - 2] + cos(rotation_voies[numero_station_suivante - 2] * pi / 180 + pi / 2) * (taille_voie1[numero_station_suivante - 2] * pourcent * pow(10, -2) - taille_metro[0] * pow(10, -1)) << "\n" << "Y : " << posY_voie1[numero_station_suivante - 2] + sin(rotation_voies[numero_station_suivante - 2] * pi / 180 + pi / 2) * (taille_voie1[numero_station_suivante - 2] * pourcent * pow(10, -2) + taille_metro[1] * pow(10, -1)) << endl;
						metro1.RotationMetro(rotation_voies[numero_station_suivante - 2]);
						cout << "angle : " << rotation_voies[numero_station_suivante - 2] * pi / 280 << "cos : " << cos(rotation_voies[numero_station_suivante - 2] * pi / 180 + pi / 2) << " sin : " << sin(rotation_voies[numero_station_suivante - 2] * pi / 180 + pi / 2) << endl;
					}
					else { //S'il est dans le sens des gare décroissants
						metro1.ChangementPositionMetro(sf::Vector2f(posX_voie2[numero_station_suivante - 1] + cos(rotation_voies[numero_station_suivante - 1] * pi / 180 + pi / 2) * (taille_voie2[numero_station_suivante - 1] * (1-pourcent * pow(10, -2)) - taille_metro[0] * pow(10, -1)), posY_voie2[numero_station_suivante - 1] + sin(rotation_voies[numero_station_suivante - 1] * pi / 180 + pi / 2) * (taille_voie2[numero_station_suivante - 1] * (1- pourcent * pow(10, -2)) + taille_metro[1] * pow(10, -1))));
						metro1.RotationMetro(rotation_voies[numero_station_suivante - 1] /* - 180*/);
						cout << "angle :" << rotation_voies[numero_station_suivante - 2] * pi / 280 << "cos : " << cos(rotation_voies[numero_station_suivante - 2] * pi / 180 + pi / 2) << " sin : " << sin(rotation_voies[numero_station_suivante - 2] * pi / 180 + pi / 2) << endl;
					}
				}
				else { //sinon
					metro1.freinage(vit); //arrêt
					metro1.arrivee_station(size); //arrivée à la station
					metro1.set_position(0); //réinitialisation de sa position
					int stat_nom = metro1.get_station(); //récupération de la station atteinte
					station stat_actu = liste_station.at(stat_nom - 1);
					stat_actu.arrivage_train();
					if (metro2.get_prochain_arret() == stat_nom && metro2.reverse() == metro1.reverse()) { //si les deux rames ont le meme arret et sont dans le meme sens de parcours
						cout << "Arret de la rame 2 pour maintenir une distance de securite" << endl;
						metro2.freinage(vit_autre);
					}
					cout << "Arrivee de la rame 1 a la station numero " << stat_nom << endl;
					int passagers = metro1.get_passager_dedans();
					int aquai = stat_actu.get_passager(); //récupération du nombre de passagers à bord et à quai
					if (stat_nom == liste_station.size() || (stat_nom == 1 && metro1.reverse())) { //si terminus
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
						cout << "Depart de la rame 1 de la station " << stat_nom << endl;
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
						std::this_thread::sleep_for(montee * 0.25s);
						metro1.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Depart de la rame 1 de la station " << stat_nom << endl;
						metro1.depart_station(vit); //reprise du trajet
						stat_actu.depart_train();
					}
				}
			}
		}
	);
	//affichage sur une console :
	sf::RenderWindow window(sf::VideoMode(1600, 900), "Visualisation du métro Lillois");
	/*std::jthread rame2(
		[&stopped, &re, &liste_station, &size, &metro1, &metro2, &vit_const2]
		{
			cout << "Rame 2 prete" << endl;
			metro2.acceleration(10);
			cout << "Rame 2 partie de la station de lancement avec " << metro2.get_passager_dedans() << " passagers" << endl;
			while (!stopped) { //tant qu'on a pas arrêté
				std::this_thread::sleep_for(1s); //attente d'1s pour simuler le déplacement de la rame
				int pourcent = metro2.get_position();
				int vit = metro2.get_vitesse(); //récupération de la position et de la vitesse de la rame
				if (vit == 0 && metro1.get_station() == 0) {
					cout << "Redemarrage de la rame 2" << endl;
					metro2.acceleration(vit_const2);
				}
				if (pourcent < 100) { //si elle n'a pas atteint la station
					cout << "Progression de la rame 2 : " << pourcent << " %" << endl; //affichage de sa progression
					metro2.set_position(pourcent + vit); //déplacement
				}
				else { //sinon
					metro2.freinage(vit); //arrêt
					metro2.arrivee_station(size); //arrivée à la station
					metro2.set_position(0); //réinitialisation de sa position
					int stat_nom = metro2.get_station(); //récupération de la station atteinte
					station stat_actu = liste_station.at(stat_nom - 1);
					stat_actu.arrivage_train();
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
						std::uniform_int_distribution<int> montee_terminus{ 1, aquai };//montée de passagers
						int montee = montee_terminus(re);
						metro2.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Montee de " << montee << " passagers dans la rame 2." << endl;
						std::this_thread::sleep_for(montee * 0.25s);
						cout << "Depart de la rame 2 de la station " << stat_nom << endl;
						metro2.depart_station(vit); //début du trajet en sens inverse
						stat_actu.depart_train();
					}
					else { //sinon
						std::uniform_int_distribution<int> descente_pif{ 1, passagers }; //descente d'un nombre aléatoire de passagers de la rame (au moins 1)
						int descente = descente_pif(re);
						cout << "Descente de " << descente << " passagers de la rame 2." << endl;
						std::this_thread::sleep_for(descente * 0.25s);
						metro2.baisse_passager_dedans(descente);
						aquai += descente;
						stat_actu.set_passager(aquai);
						std::uniform_int_distribution<int> montee_pif{ 1,aquai }; //montée d'un nombre aléatoire de passagers dans la rame depuis le quai (au moins 1)
						int montee = montee_pif(re);
						cout << "Montee de " << montee << " passagers dans la rame 2." << endl;
						std::this_thread::sleep_for(montee * 0.25s);
						metro2.hausse_passager_dedans(montee);
						aquai -= montee;
						stat_actu.set_passager(aquai);
						cout << "Depart de la rame 2 de la station " << stat_nom << endl;
						metro2.depart_station(vit); //reprise du trajet
						stat_actu.depart_train();
					}
				}
			}
		}
	);*/




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
		liste_station[i].ChangementPositionGare(sf::Vector2f(PosXGare[i], PosYGare[i]));

	}

	//MaJ des positions et de la tailles des différentes gares
	//Les gares :
	liste_station[0].ChangementPositionGare(sf::Vector2f(125, 100));

	liste_station[1].ChangementPositionGare(sf::Vector2f(225, 300));

	liste_station[2].ChangementPositionGare(sf::Vector2f(350, 600));

	liste_station[3].ChangementPositionGare(sf::Vector2f(550, 750));

	liste_station[4].ChangementPositionGare(sf::Vector2f(750, 750));

	liste_station[5].ChangementPositionGare(sf::Vector2f(900, 600));

	liste_station[6].ChangementPositionGare(sf::Vector2f(1000, 400));

	liste_station[7].ChangementPositionGare(sf::Vector2f(1100, 250));

	liste_station[8].ChangementPositionGare(sf::Vector2f(1300, 350));

	liste_station[9].ChangementPositionGare(sf::Vector2f(1450, 550));

	metro1.ChangementTailleMetro(sf::Vector2f(0.1, 0.1));
	//Mise à jour des positions des voies
	for (int i = 0; i < 9; i++) {
		liste_station[i].ChangementPositionVoie1(sf::Vector2f(posX_voie1[i], posY_voie1[i]));
		liste_station[i].ChangementPositionVoie2(sf::Vector2f(posX_voie2[i], posY_voie2[i]));
		liste_station[i].ChangementTailleVoie1(sf::Vector2f(2, taille_voie1[i]));
		liste_station[i].ChangementTailleVoie2(sf::Vector2f(2, taille_voie2[i]));
		liste_station[i].RotationVoie(rotation_voies[i]);
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
		window.display();
	}
}

