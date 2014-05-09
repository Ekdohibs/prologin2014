// Max flow

#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#define MAXN 2000
extern int capacites[MAXN][MAXN];
extern int flot[MAXN][MAXN];
extern int prix[MAXN][MAXN];
void max_flow(int n);
void max_flow_min_cout(int n);

#endif
