// projetVAL.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once

#include <iostream>
//#include <string>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <random>

class station {
private:
    int nb_passager;
    int numero_station; //std::string nom_station;
    bool train_present;


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
};

class metro {
private:
    int nb_passager_dedans;
    int prochain_arret; //Numero de la prochaine
    //Si on a le temps : problème technique
    int vitesse;
    int position; //pourcentage entre l'ancienne station et la suivante
    int station; //0 si pas dans une station, num station si dans une station
    int numero_train;
    bool sens; // si true, traverse les stations en sens inverse

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

    void acceleration(int n);
    void freinage(int n);
    void baisse_passager_dedans(int n);
    void hausse_passager_dedans(int n);
    void arrivee_station(int taille_ligne); //Change la variable station et prochain_arret
    void depart_station(int vit_depart);
    void demi_tour();
};