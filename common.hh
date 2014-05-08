#ifndef COMMON_H
#define COMMON_H

#include "prologin.hh"
#include <vector>

inline int distance(position p1, position p2) {
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

inline bool valide(position pos) {
  return (pos.x >= 0) && (pos.x < TAILLE_TERRAIN) &&
    (pos.y >= 0) && (pos.y < TAILLE_TERRAIN);
}

inline position mid(position p1, position p2) {
  return position((p1.x + p2.x)/2, (p1.y + p2.y)/2);
}

extern int players_ids[4];
extern position fontaines[4];
extern position artefact;
extern std::vector<position> objectives;

void partie_debut();
std::vector<position> pos_in_range(position pos, int portee);

#endif
