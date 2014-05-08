// Max flow
#include "max_flow.hh"
#include <algorithm>
using namespace std;

void max_flow(int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      flot[i][j] = 0;
  
  while (true) {
    for (int i = 0; i < n; i++)
      prevs[i] = NONE;

    file[0] = 0;
    prevs[0] = START;
    int index = 0;
    int indexchange = 1;
    int indexadd = 1;
    
    bool found = false;
    while (index < indexchange) {
      while (index < indexchange) {
	int u = file[index++];
	for (int v = 0; v < n; v++) {
	  if (capacites[u][v] - flot[u][v] && prevs[v] == NONE) {
	    prevs[v] = u;
	    file[indexadd++] = v;
	    if (v == n - 1) {
	      found = true;
	      break;
	    }
	  }
	}
      }
      indexchange = indexadd;
      if (found)
	break;
    }
    if (!found)
      return;

    int cmin = INF;
    {
      int v = n - 1;
      while (prevs[v] != START) {
	int u = prevs[v];
	cmin = min(cmin, capacites[u][v] - flot[u][v]);
	v = u;
      }
    }
    {
      int v = n - 1;
      while (prevs[v] != START) {
	int u = prevs[v];
	flot[u][v] += cmin;
	flot[v][u] -= cmin;
	v = u;
      }
    }
  }
}
