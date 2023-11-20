// projetVAL.cpp : définit le point d'entrée de l'application.
//

#include "projetVAL.h"

using namespace std;




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

int main()
{
	cout << "Hello CMake." << endl;
	return 0;
}
