// Main file
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include "prologin.hh"
#include "max_flow.hh"
#include "chemin.hh"
#include "common.hh"
using namespace std;

#define INF 10000000

void dump_path(vector<position> path) {
  for (unsigned int i = 0; i < path.size(); i++) {
    position p = path[i];
    cout << "(" << p.x << ", " << p.y << ") ";
  }
  cout << endl;
}

void deplacer_(position depart, position arrivee, int nb) {
  if (nb == 0) {
    return;
  }
  vector<position> path = safe_chemin(depart, arrivee);
  //dump_path(path);
  //int d = min((int)path.size(), PORTEE_SORCIER);
  int d = min((int)path.size(), 1);
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

vector<position> sorciers_adv() {
  vector<position> positions;
  for (int i = 0; i < TAILLE_TERRAIN; i++) {
    for (int j = 0; j < TAILLE_TERRAIN; j++) {
      position p = position(i, j);
      for (int joueur = 1; joueur < 4; joueur ++) {
	if (nb_sorciers(p, players_ids[joueur])) {
	  positions.push_back(p);
	  break;
	}
      }
    }
  }
  return positions;
}

int nb_sorciers_adv(position p) {
  return nb_sorciers(p, players_ids[1]) +
    nb_sorciers(p ,players_ids[2]) +
    nb_sorciers(p, players_ids[3]);
}

bool construire_vers(position objectif) {
  position pmin;
  int dmin = INF;
  for (int i = 0; i < TAILLE_TERRAIN; i++) {
    for (int j = 0; j < TAILLE_TERRAIN; j++) {
      position p = position(i, j);
      if (distance(p, objectif) < dmin && constructible(p, moi())) {
	dmin = distance(p, objectif);
	pmin = p;
      }
    }
  }
  if (dmin == INF) {
    return false;
  }
  return (construire(pmin, 3) == OK);
}

bool is_protected(position pos) {
  vector<tourelle> tourelles = tourelles_joueur(moi());
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    if (distance(pos, tourelles[i].pos) <= tourelles[i].portee) {
      return true;
    }
  }
  return false;
}

void phase_construction() {
  for (unsigned int i = 0; i < objectives.size(); i++) {
    if (!is_protected(objectives[i])) {
	construire_vers(objectives[i]);
    }
  }
  creer(magie(moi())/COUT_SORCIER);
}

void phase_deplacement() {
  update_danger();
  position p2 = objectives[0];
  vector<position> positions = sorciers(moi());
  for (unsigned int i = 0; i < positions.size(); i++) {
    position p1 = positions[i];
    int ns = nb_sorciers_deplacables(p1, moi());
    if (p1 == base_joueur(moi()))
      ns /= 2;
    deplacer_(p1, p2, ns);
  }
}

void phase_tirs() {
  vector<tourelle> tourelles = tourelles_joueur(moi());
  vector<position> sadv = sorciers_adv();
  int n = tourelles.size() + sadv.size() + 2;
  //assert(n <= MAXN);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      capacites[i][j] = 0;
    }
  }
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    capacites[0][i+1] = ATTAQUE_TOURELLE;
  }
  int m = tourelles.size(); 
  for (unsigned int i = 0; i < sadv.size(); i++) {
    capacites[i+m+1][n-1] = nb_sorciers_adv(sadv[i]);
  }
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    for (unsigned int j = 0; j < sadv.size(); j++) {
      if (distance(tourelles[i].pos, sadv[j]) <= tourelles[i].portee) {
	capacites[i+1][j+m+1] = INF;
      }
    }
  }
  max_flow(n);
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    for (unsigned int j = 0; j < sadv.size(); j++) {
      tirer(flot[i+1][j+m+1], tourelles[i].pos, sadv[j]);
    }
  }
}

void phase_siege() {

}

void partie_fin() {}

