// projetVAL.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once

#include <iostream>

// TODO: Référencez ici les en-têtes supplémentaires nécessaires à votre programme.

// Projet visualisation VAL.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once

#include <iostream>
//#include <string>
// TODO: Référencez ici les en-têtes supplémentaires nécessaires à votre programme.
class station {
public:
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

private:
    int nb_passager;
    int numero_station; //std::string nom_station;
    bool train_present;
};

class metro {
public:
    //creation des getters
    const int get_passager_dedans();
    const int prochain_arret();
    const int get_vitesse();
    const int get_position();
    const int get_station();

    //creation des setters :
    void acceleration();
    void freinage();
    void baisse_passager();
    void hausse_passager();
    void changement_position();
    void arrivee_station(); //Change la variable station et prochain_arret
    void depart_station();

private:
    int nb_passager_dedans;
    int prochain_arret; //Numero de la prochaine
    //Si on a le temps : problème technique
    int vitesse;
    int position; //pourcentage entre l'ancienne station et la suivante
    int station; //0 si pas dans une station, num station si dans une station

};