#include "prologin.hh"
#include "common.hh"
#include <vector>
using namespace std;

int players_ids[4];
position fontaines[4];
vector<objective> objectives;
position artefact = position(TAILLE_TERRAIN/2, TAILLE_TERRAIN/2);

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
  objectives.push_back(objective(artefact, 25, 1, 2, 10));
  objectives.push_back(objective(fontaines[0], 15, 1, 2, 10));
  objectives.push_back(objective(fontaines[3], 14, 1, 2, 10));
  objectives.push_back(objective(fontaines[1], 6, 1, 2, 2));
  objectives.push_back(objective(fontaines[2], 5, 1, 2, 2));
  objectives.push_back(objective(base_joueur(players_ids[1]), 43, 2, 2, 20));;
  objectives.push_back(objective(base_joueur(players_ids[3]), 37, 2, 2, 10));;
  objectives.push_back(objective(base_joueur(players_ids[2]), 42, 2, 2, 20));;
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
