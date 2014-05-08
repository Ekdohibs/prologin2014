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

std::vector<position> pos_in_range(position pos);

#endif
