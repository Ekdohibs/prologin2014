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
  safe_path path = safe_chemin(depart, arrivee);
  //if (nb <= path.danger)
  //  return;
  //dump_path(path);
  //int d = min((int)path.size(), PORTEE_SORCIER);
  int d = min((int)path.path.size(), 1);
  if (d == 0) {
    return;
  }
  deplacer(depart, path.path[d-1], nb);
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
	//if (joueur_case(p) != moi() && joueur_case(p) != -1) {
	  positions.push_back(p);
	  break;
	}
      }
    }
  }
  return positions;
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

inline bool elimine(int joueur) {
  return joueur_case(base_joueur(joueur)) != joueur;
}

int jbase(position p) {
  if ((p.x != 0 && p.x != TAILLE_TERRAIN - 1) || (p.y != 0 && p.y != TAILLE_TERRAIN - 1))
    return -1;
  return ((p.y != 0) << 1) | ((p.y != 0) ^ (p.x != 0));
}

void update_objectives() {
  vector<objective> old_objectives;
  objectives.swap(old_objectives);
  vector<position> sadv = sorciers_adv();
  int menace = 0;
  for (unsigned int i = 0; i < sadv.size(); i++) {
    if (distance(sadv[i], base_joueur(moi())) <= PORTEE_SORCIER) {
      menace += nb_sorciers_adv(sadv[i]);
    }
  }
  if (menace > nb_sorciers(base_joueur(moi()), moi())) {
    objective panic = objective(base_joueur(moi()), 5, 3, 10);
    panic.tower_s = 3;
    objectives.push_back(panic);
    cout << "Panic mode\n";
  }
  for (unsigned int i = 0; i < old_objectives.size(); i++) {
    int j = jbase(old_objectives[i].pos);
    if (j != -1) {
      if (!elimine(players_ids[j])) {
	objectives.push_back(old_objectives[i]);
      }
    } else {
      objectives.push_back(old_objectives[i]);
    }
  }
}

void phase_construction() {
  update_objectives();
  if (tour_actuel() == 1) {
    creer(magie(moi())/COUT_SORCIER);
  }
  sort(objectives.begin(), objectives.end());
  for (unsigned int i = 0; i < objectives.size(); i++) {
    objectives[i].tower_s++;
    if (objectives[i].tower_s >= objectives[i].tower_delay) {
      if (construire_vers(objectives[i].pos)) {
	objectives[i].tower_s = 0;
	objectives[i].tower_increase_s++;
	if (objectives[i].tower_increase_s >= objectives[i].tower_increase_delay) {
	    objectives[i].tower_increase_s = 0;
	    objectives[i].tower_delay++;
	}
      }
    }
  }
  creer(magie(moi())/COUT_SORCIER);
}

void phase_deplacement() {
  update_danger();
  vector<position> positions = sorciers(moi());
  for (unsigned int i = 0; i < positions.size(); i++) {
    position p1 = positions[i];
    int ns = nb_sorciers_deplacables(p1, moi());
    if (p1 == base_joueur(moi())) {
      ns /= 2;
    }
    vector<unsigned int> objs;
    //if (p1 == base_joueur(moi())) {
      for (unsigned int i = 0; i < objectives.size(); i++) {
	objs.push_back(i);
      }
    /*} else {
      int dmin = INF;
      for (unsigned int i = 0; i < objectives.size(); i++) {
	int d = distance(p1, objectives[i].pos);
	if (d < dmin) {
	  dmin = d;
	  objs.clear();
	  objs.push_back(i);
	} else if (d == dmin) {
	  objs.push_back(i);
	}
      }
    }*/
    int s = 0;
    for (unsigned int i = 0; i < objs.size(); i++) {
      s += objectives[objs[i]].value;
    }
    s = objectives[0].value;
    objs.clear(); objs.push_back(0);
    for (unsigned int i = 0; i < objs.size(); i++) {
      deplacer_(p1, objectives[objs[i]].pos, (objectives[objs[i]].value*ns)/s);
    }
    //cout << endl;
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
    int d = nb_sorciers_adv(sadv[i]);
    //if (d == 0)
    //  d = 10; // Tour
    capacites[i+m+1][n-1] = d;
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

