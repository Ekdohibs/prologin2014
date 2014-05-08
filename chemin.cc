// Dijkstra
#include "chemin.hh"
#include "prologin.hh"
#include "common.hh"
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

#define START -1
#define NONE -2
#define INF 10000000
//#define MAXN 1000
int danger[TAILLE_TERRAIN][TAILLE_TERRAIN];

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

priority_queue<cle> tas;

void update_danger() {
  
}

vector<position> safe_chemin(position depart, position arrivee) {
  
}
