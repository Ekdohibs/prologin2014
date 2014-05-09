// Max flow
#include "max_flow.hh"
#include "prologin.hh"
#include <algorithm>
using namespace std;

#define START -1
#define NONE -2
#define INF 10000000

int file[MAXN];
int prevs[MAXN];
int capacites[MAXN][MAXN];
int flot[MAXN][MAXN];

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

int dist[MAXN];
int prix[MAXN][MAXN];

void max_flow_min_cout(int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      flot[i][j] = 0;
  
  while (true) {
    for (int i = 0; i < n; i++) {
      prevs[i] = NONE;
      dist[i] = INF;
    }

    dist[0] = 0;
    prevs[0] = START;
    
    vector<position> aretes;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
	if (capacites[i][j] - flot[i][j] > 0) {
	  aretes.push_back(position(i, j));
	}
      }
    }

    for (int u = 0; u < n; u++) {
      for (unsigned int i = 0; i < aretes.size(); i++) {
	position p = aretes[i];
	if (dist[p.x] + prix[p.x][p.y] < dist[p.y]) {
	  dist[p.y] = dist[p.x] + prix[p.x][p.y];
	  prevs[p.y] = p.x;
	}
      }
    }
    
    if (prevs[n-1] == NONE)
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
