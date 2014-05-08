// Main file
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include "prologin.hh"
using namespace std;

#define INF 10000000

vector<position> objectives;
int players_ids[4];
position fontaines[4];

#define MAXN 2000;
int file[MAXN];
int prev[MAXN];
int capacites[MAXN][MAXN];
int flot[MAXN][MAXN];
#define START -1;
#define NONE -2;
void max_flow(int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      flot[i][j] = 0;
  
  while (true) {
    for (int i = 0; i < n; i++)
      prev[i] = NONE;

    file[0] = 0;
    prev[0] = START;
    index = 0;
    indexchange = 1;
    indexadd = 1;
    
    bool found = false;
    while (index < indexchange) {
      while (index < indexchange) {
	int u = file[index++];
	for (int v = 0; v < n; v++) {
	  if (capacites[u][v] - flot[u][v] && prev[v] == NONE) {
	    prev[v] = u;
	    file[indexadd++] = v;
	    if (v == n - 1) {
	      found = true;
	      break
	    }
	  }
	}
      }
      indexchange = indexadd;
      if (found)
	break
    }
    if (!found)
      return;

    int cmin = INF;
    {
      int v = n - 1;
      while (prev[v] != START) {
	int u = prev[v];
	cmin = min(cmin, capacites[u][v] - flot[u][v]);
	v = u;
      }
    }
    {
      int v = n - 1;
      while (prev[v] != START) {
	flot[u][v] += cmin;
	flot[v][u] -= cmin;
      }
    }
  }
}

position mid(position p1, position p2) {
  return position((p1.x + p2.x)/2, (p1.y + p2.y)/2);
}

void partie_debut() {
  players_ids[0] = moi();
  position pp = base_joueur(moi());
  for (int i = 0; i < 3; i++) {
    int id = adversaires()[i];
    position p = base_joueur(id);
    int k = 1;
    if ((p.x == TAILLE_TERRAIN - 1 - pp.x) && (p.y == TAILLE_TERRAIN - 1 - pp.y)) {
      k = 2;
    } else if ((p.x == pp.y) && (p.y == TAILLE_TERRAIN - 1 - pp.x)) {
      k = 3;
    }
    players_ids[k] = id;
  }
  for (int i = 0; i < 4; i++) {
    fontaines[i] = mid(base_joueur(players_ids[i]), 
		       base_joueur(players_ids[(i+1)%4]));
  }
}

inline int distance(position p1, position p2) {
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

inline bool valide(position pos) {
  return (pos.x >= 0) && (pos.x < TAILLE_TERRAIN) &&
    (pos.y >= 0) && (pos.y < TAILLE_TERRAIN);
}

vector<position> pos_in_range(position pos, int range) {
  vector<position> positions;
  for (int x = pos.x - range; x <= pos.x + range; x++) {
    for (int y = pos.y - range; y <= pos.y + range; y++) {
      position p = position(x, y);
      if (valide(p) && distance(p, pos) <= range) {
	positions.push_back(p);
      }
    }
  }
  return positions;
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

bool construire_vers(position objectif) {
  //vector<tourelle> tourelles = tourelles_joueur(players_id[0]);
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

void phase_construction() {
  //creer(magie(moi())/COUT_SORCIER);
  if (tour_actuel() < 6)
    construire_vers(fontaines[0]);
  else
    creer(magie(moi())/COUT_SORCIER);
}

void phase_deplacement() {
  position p2 = fontaines[0];
  vector<position> positions = sorciers(moi());
  for (unsigned int i = 0; i < positions.size(); i++) {
    position p1 = positions[i];
    deplacer_(p1, p2, nb_sorciers_deplacables(p1, moi()));
  }
}

void phase_tirs() {
  vector<tourelles> tourelles = tourelles_joueur(moi());
  
}

void phase_siege() {

}

void partie_fin() {}

