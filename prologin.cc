// Main file
#include <vector>
#include <iostream>
#include <algorithm>
#include "prologin.hh"
using namespace std;

void partie_debut() {}

inline int distance(position p1, position p2) {
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

void deplacer_(position depart, position arrivee, int nb) {
  if (nb == 0) {
    return;
  }
  vector<position> path = chemin(depart, arrivee);
  int d = min((int)path.size(), PORTEE_SORCIER);
  if (d == 0) {
    return;
  }
  deplacer(depart, path[d-1], nb);
}

vector<position> sorciers(int joueur) {
  vector<position> positions;
  for (int i = 0; i < TAILLE_TERRAIN; i++) {
    for (int j = 0; j < TAILLE_TERRAIN; j++) {
      position p = position(i, j);
      if (nb_sorciers(p, joueur)) {
	positions.push_back(p);
      }
    }
  }
  return positions;
}

void phase_construction() {
  creer(magie(moi())/COUT_SORCIER);
}

void phase_deplacement() {
  position p2 = base_joueur((moi())%4 + 1);
  vector<position> positions = sorciers(moi());
  for (unsigned int i = 0; i < positions.size(); i++) {
    position p1 = positions[i];
    deplacer_(p1, p2, nb_sorciers_deplacables(p1, moi()));
  }
}

void phase_tirs() {

}

void phase_siege() {

}

void partie_fin() {}

