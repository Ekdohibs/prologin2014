// Max flow

#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#define MAXN 2000
extern int file[MAXN];
extern int prevs[MAXN];
extern int capacites[MAXN][MAXN];
extern int flot[MAXN][MAXN];
#define START -1
#define NONE -2
#define INF 10000000
void max_flow(int n);

#endif
