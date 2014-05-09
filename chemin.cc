// Dijkstra
#include "chemin.hh"
#include "prologin.hh"
#include "common.hh"
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

//#define START -1
//#define NONE -2
const position START = position(-1, -1);
const position NONE = position(-2, -2);
#define INF 10000000
//#define MAXN 1000
int danger[TAILLE_TERRAIN][TAILLE_TERRAIN];

void update_danger() {
  for (int i = 0; i < TAILLE_TERRAIN; i++) {
    for (int j = 0; j < TAILLE_TERRAIN; j++) {
      danger[i][j] = nb_sorciers_adv(position(i, j));
    }
  }
  for (int adv = 1; adv < 4; adv++) {
    vector<tourelle> tourelles = tourelles_joueur(players_ids[adv]);
    for (unsigned int i = 0; i < tourelles.size(); i++) {
      vector<position> pos_atteignables = pos_in_range(tourelles[i].pos, tourelles[i].portee);
      for (unsigned int j = 0; j < pos_atteignables.size(); j++) {
	position p = pos_atteignables[j];
	danger[p.x][p.y]++;
      }
    }
  }
}


struct cle {
  position pos;
  int danger;
  int temps;
  cle() {};
  cle(position pos_, int danger_, int temps_):
    pos(pos_), danger(danger_), temps(temps_) {};
  
  inline bool operator < (const cle &other) const {
    return (danger > other.danger) || (danger == other.danger &&
	    temps > other.temps);
  };
};

position deps[4] = {position(0, 1), position(0, -1), position(1, 0), position(-1, 0)};

position pres[TAILLE_TERRAIN][TAILLE_TERRAIN];

vector<position> get_access(position p) {
  vector<position> u = pos_in_range(p, PORTEE_SORCIER);
  vector<position> v;
  for (unsigned int i = 0; i < u.size(); i++) {
    if (chemin(p, u[i]).size() <= PORTEE_SORCIER) {
      v.push_back(u[i]);
    }
  }
  return v;
}

safe_path safe_chemin(position depart, position arrivee) {
  priority_queue<cle> tas;
  for (int i = 0; i < TAILLE_TERRAIN; i++) {
    for (int j = 0; j < TAILLE_TERRAIN; j++) {
      pres[i][j] = NONE;
    }
  }

  pres[depart.x][depart.y] = START;
  tas.push(cle(depart, 0, 0));
  int dang = 0;
  while (!tas.empty()) {
    cle a = tas.top(); tas.pop();
    bool found = false;
    vector<position> access = get_access(a.pos);
    for (unsigned int i = 0; i < access.size(); i++) {
      position p = access[i];
      if (!valide(p) || pres[p.x][p.y] != NONE || info_case(p) == CASE_TOURELLE)
	continue;
      pres[p.x][p.y] = a.pos;
      if (p == arrivee) {
	found = true;
	dang = a.danger + danger[p.x][p.y];
	break;
      }
      tas.push(cle(p, a.danger + danger[p.x][p.y], a.temps + 1));
    }
    if (found)
      break;
  }

  vector<position> c;
  position v = arrivee;
  if (pres[v.x][v.y] == NONE) {
    return safe_path(0, c);
  }
  while (pres[v.x][v.y] != START) {
    c.push_back(v);
    v = pres[v.x][v.y];
  }
  unsigned int n = c.size();
  for (unsigned int i = 0; i < n/2; i++) {
    position temp = c[i];
    c[i] = c[n-1-i];
    c[n-1-i] = temp;
  }
  return safe_path(dang, c);
}
