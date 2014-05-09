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

inline int nb_sorciers_adv(position p) {
  return nb_sorciers(p, players_ids[1]) +
    nb_sorciers(p ,players_ids[2]) +
    nb_sorciers(p, players_ids[3]);
}

struct objective {
  position pos;
  int value;
  int tower_delay;
  int tower_s;
  int tower_increase_delay;
  int tower_increase_s;

  objective() {};
  objective(position pos_, int value_, int tower_delay_, int tower_increase_delay_):
    pos(pos_), value(value_), tower_delay(tower_delay_), tower_s(0),
    tower_increase_delay(tower_increase_delay_), tower_increase_s(0) {};

  inline bool operator < (const objective &other) const {
    return value > other.value;
  };
};

extern std::vector<objective> objectives;

void partie_debut();
std::vector<position> pos_in_range(position pos, int portee);

#endif
