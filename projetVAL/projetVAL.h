﻿// projetVAL.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <random>
#include <cmath>
#include <cstdlib>

class station {
private:
    int nb_passager; //nombres de passagers sur le quai
    int numero_station; //std::string nom_station;
    bool train_present; //si un train est a quai
    sf::Sprite Gare; //image de la gare pour l'affichage
    sf::RectangleShape voie1, voie2; //La voie 1 vas en station croissante (1,2,3,etc) et la voie 2 en station décroissante (9,8,7,etc)

public:
    station(int num_station = 1, int nb_passagers = 0, bool trains_present = false) {
        nb_passager = nb_passagers;
        train_present = trains_present;
        if (num_station != 0) {
            numero_station = num_station;
        }
        else {
            std::cout<<"ERROR : Station number is 0"<<std::endl;
            exit(0);
        }

        //Couleur des voies : Noir
        voie1.setFillColor(sf::Color(0, 0, 0));
        voie2.setFillColor(sf::Color(0, 0, 0));

    }

    //Creation des getters
    const int get_passager();
    const int get_nom();
    const bool is_a_train();

    //Creation des setters
    void set_passager(int n);
    void arrivage_train();

    //Fonction de baisse de passager
    void baisse_passager(int n);

    //Fonction depart d'un train
    void depart_train();

    //Changement de texture de gare
    void ChangementTextureGare(const sf::Texture &Text);

    //Affichage de la Gare
    void AffichageGare(sf::RenderWindow &Win);

    //Changement Taille Gare
    void ChangementTailleGare(const sf::Vector2f &taille);

    //Changement Position Gare
    void ChangementPositionGare(const sf::Vector2f &position);

    //Changement Taille Voie
    void ChangementTailleVoie1(const sf::Vector2f taille);
    void ChangementTailleVoie2(const sf::Vector2f taille);

    //RotationVoie
    void RotationVoie(const int n);
    //Changement Position Voie
    void ChangementPositionVoie1(const sf::Vector2f& pos);
    void ChangementPositionVoie2(const sf::Vector2f& pos);

    //Affichage Voies
    void AffichageVoies(sf::RenderWindow& Win);

};

class metro {
private:
    int nb_passager_dedans; //nombre de passagers à bord
    int prochain_arret; //Numero de la prochaine station
    int vitesse; //vitesse actuelle
    int position; //pourcentage entre l'ancienne station et la suivante (entre 0 et 100)
    int station; //0 si pas dans une station, num station si dans une station
    int numero_train; //identifiant du train
    bool sens; // si true, traverse les stations en sens inverse
    sf::Sprite wagon; //image de la rame pour l'affichage

public:

    metro(int passagers = 0, int arret_suiv = 1, int speed = 0, int pos = 0, int station_actu = 0, int numero_metro = 1, bool sens_trajet = false) {
        nb_passager_dedans = passagers;
        prochain_arret = arret_suiv;
        vitesse = speed;
        position = pos;
        station = station_actu;
        numero_train = numero_metro;
        sens = sens_trajet;
       
    }

    //creation des getters
    const int get_passager_dedans();
    const int get_prochain_arret();
    const int get_vitesse();
    const int get_position();
    const int get_station();
    const int get_numero_train();
    const bool reverse();

    //creation des setters et changeurs de variables:
    void set_passager_dedans(int n);
    void set_position(int pos);

    void acceleration(int n); // hausse de la vitesse
    void freinage(int n); //baisse de la vitesse
    void baisse_passager_dedans(int n); //descente de passagers
    void hausse_passager_dedans(int n); //montee de passagers
    void arrivee_station(int taille_ligne); //Change la variable station et prochain_arret
    void depart_station(int vit_depart); //quitte la station
    void demi_tour(); //changement de sens
    void ChangementTextureWagon(const sf::Texture& Text); //changement de l'image
    void ChangementPositionMetro(const sf::Vector2f& pos); //déplacement de l'image
    void ChangementTailleMetro(const sf::Vector2f& pos); //modification de la taille de l'image
    void RotationMetro(const int n); //rotation d'un certain angle
    void AffichageMetro(sf::RenderWindow &Win); //affichage de la rame
    void affichage_demi_tour(int i); //cas spécial du demi-tour
};