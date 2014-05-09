#include "prologin.hh"
#include <vector>

extern int danger[TAILLE_TERRAIN][TAILLE_TERRAIN];
void update_danger();
struct safe_path {
  int danger;
  std::vector<position> path;

  safe_path(int danger_, std::vector<position> path_):
    danger(danger_), path(path_) {};
};

safe_path safe_chemin(position depart, position arrivee);
