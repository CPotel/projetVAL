// projetVAL.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once

#include <iostream>

// TODO: Référencez ici les en-têtes supplémentaires nécessaires à votre programme.

// Projet visualisation VAL.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once

#include <iostream>
#include <string>
// TODO: Référencez ici les en-têtes supplémentaires nécessaires à votre programme.
class station {
public:
    //Creation des getters
    int get_passager();
    int get_nom();
    bool is_a_train();

    //Creation des setters
    void set_passager(int n);
    void arrivage_train();

    //Fonction de baisse de passager
    void baisse_passager(int n);

    //Fonction depart d'un train
    void depart_train();

private:
    int nb_passager;
    std::string nom_station;
    bool train_present;
};