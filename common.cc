#include "prologin.hh"
#include "common.hh"
#include <vector>
using namespace std;

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
