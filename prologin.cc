// Main file
#include <vector>
#include <iostream>
#include <algorithm>
#include "prologin.hh"
using namespace std;

void partie_debut() {}

void deplacer_(position depart, position arrivee, int nb) {
  vector<position> path = chemin(depart, arrivee);
  int d = min((int)path.size(), PORTEE_SORCIER);
  deplacer(depart, path[d-1], nb);
}

void phase_construction() {
  creer(magie(moi())/COUT_SORCIER);
}

void phase_deplacement() {
  position p1 = base_joueur(moi());
  position p2 = base_joueur((moi() + 1)%4);
  deplacer(p1, p2, nb_sorciers(p1, moi()));
}

void phase_tirs() {

}

void phase_siege() {

}

void partie_fin() {}

