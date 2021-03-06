// Main file
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include "prologin.hh"
#include "max_flow.hh"
#include "chemin.hh"
#include "common.hh"
#include <time.h>
using namespace std;

/*
+------------------------------+
|  Fonctionnement du champion  |
+------------------------------+

Fonctionnement global, idée générale : 
--------------------------------------

On a une liste d'objectifs, qu'on veut attaquer/défendre
(artefact, fontaines, ennemis, etc).
A la fin de la partie, on envoie autant d'unités que possible
sur l'artefact (sauf en cas de menace de la base)


Phase I - Construction
----------------------

On met a jour la liste des objectifs.
On construit des suites de tours pour s'approcher des objectifs :
on choisit l'emplacement le plus proche possible constructible.
Si on est attaque, on essaie de se barricader derrière les tours.
On finit par construire des sorciers.

Phase II - Deplacement
----------------------

Phase la plus bugguee... un essai d'utilisation de max_flow_min_cout
s'est solde par un échec cuisant, par conséquent on utilise un 
algorithme glouton assez stupide. On cherche des chemins les moins 
dangereux possibles, et parmi ceux-là, les plus courts. Pour essayer
de s'approcher des tours ennemies, on les cases a cote de celles-ci
un peu moins dangereuse que le reste de leur zone d'attaque. On evite
egalement d'envoyer des sorciers au suicide.

Phase III - Tir
---------------

Simple application d'un algorithme de max_flow pour trouver un couplage
maximal. L'algorithme utilise est Ford-Fulkerson.

Phase IV - Siege
----------------

Idem Phase III.

*/

#define INF 10000000

bool panic = false;
int tact = 0;

inline int tour_actuel() {
  return tact;
}
void dump_path(vector<position> path) {
  for (unsigned int i = 0; i < path.size(); i++) {
    position p = path[i];
    cout << "(" << p.x << ", " << p.y << ") ";
  }
  cout << endl;
}

position endgame_goal = artefact;

bool deplacer_(position depart, position arrivee, int nb) {
  if (nb == 0) {
    return true;
  }
  safe_path path = safe_chemin(depart, arrivee);
  //if (nb <= path.danger)
  //  return;
  //dump_path(path);
  //int d = min((int)path.size(), PORTEE_SORCIER);
  int d = min((int)path.path.size(), 1);
  if (d == 0) {
    return false;
  }
  position but = path.path[d-1];
  if (((nb <= danger[but.x][but.y] || (nb <= path.danger + 10*(int)path.path.size() && danger[but.x][but.y] > 0)) && tour_actuel() < 95) || (tour_actuel() >= 95 && tour_actuel() < 100 && but == endgame_goal)) {
    deplacer(depart, depart, nb);
    return true;
  }
  deplacer(depart, but, nb);
  return true;
}

vector<position> sorciers(int joueur) {
  vector<position> positions;
  for (int i = 0; i < TAILLE_TERRAIN; i++) {
    for (int j = 0; j < TAILLE_TERRAIN; j++) {
      position p = position(i, j);
      if (nb_sorciers(p, joueur)) {
	positions.push_back(p);
      }
    }
  }
  return positions;
}


int nb_sorciers_joueur(int joueur) {
  int s = 0;
  for (int i = 0; i < TAILLE_TERRAIN; i++) {
    for (int j = 0; j < TAILLE_TERRAIN; j++) {
      s += nb_sorciers(position(i, j), joueur);
    }
  }
  return s;
}


vector<position> sorciers_adv() {
  vector<position> positions;
  for (int i = 0; i < TAILLE_TERRAIN; i++) {
    for (int j = 0; j < TAILLE_TERRAIN; j++) {
      position p = position(i, j);
      for (int joueur = 1; joueur < 4; joueur ++) {
	if (nb_sorciers(p, players_ids[joueur])) {
	//if (joueur_case(p) != moi() && joueur_case(p) != -1) {
	  positions.push_back(p);
	  break;
	}
      }
    }
  }
  return positions;
}

vector<tourelle> tourelles_adv() {
  vector<tourelle> tourelles = tourelles_joueur(players_ids[1]);
  for (int i = 2; i < 4; i++) {
    vector<tourelle> tt = tourelles_joueur(players_ids[i]);
    for (unsigned int j = 0; j < tt.size(); j++) {
      tourelles.push_back(tt[j]);
    }
  }
  return tourelles;
}

bool construire_vers(position objectif, bool force = false) {
  position pmin;
  int dmin = INF;
  for (int i = 0; i < TAILLE_TERRAIN; i++) {
    for (int j = 0; j < TAILLE_TERRAIN; j++) {
      position p = position(i, j);
      if (distance(p, objectif) < dmin && constructible(p, moi())) {
	dmin = distance(p, objectif);
	pmin = p;
      }
    }
  }
  int ddmin = INF;
  vector<tourelle> tourelles = tourelles_joueur(moi());
  if (!force) {
    for (unsigned int i = 0; i < tourelles.size(); i++) {
      ddmin = min(ddmin, distance(objectif, tourelles[i].pos));
    }
  }
  if (dmin >= ddmin) {
    return false;
  }
  return (construire(pmin, 3) == OK);
}

bool is_protected(position pos) {
  vector<tourelle> tourelles = tourelles_joueur(moi());
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    if (distance(pos, tourelles[i].pos) <= tourelles[i].portee) {
      return true;
    }
  }
  return false;
}

inline bool elimine(int joueur) {
  return joueur_case(base_joueur(joueur)) != joueur;
}

int jbase(position p) {
  if ((p.x != 0 && p.x != TAILLE_TERRAIN - 1) || (p.y != 0 && p.y != TAILLE_TERRAIN - 1))
    return -1;
  position pp = base_joueur(moi());
  return (((p.y != pp.y) << 1) | ((p.y != pp.y) ^ (p.x != pp.x)));
}

void endgame_objectives() {
    objectives.push_back(objective(fontaines[0], 5, 1000000, 1, 1));
    objectives.push_back(objective(fontaines[3], 5, 1000000, 1, 1));
    objectives.push_back(objective(fontaines[1], 5, 1000000, 1, 1));
    objectives.push_back(objective(fontaines[2], 5, 1000000, 1, 1));
    int nsmax = 0;
    int jmax = 0;
    for (int i = 0; i < 4; i++) {
      if (nb_sorciers_joueur(players_ids[i]) > nsmax) {
	nsmax = nb_sorciers_joueur(players_ids[i]);
	jmax = i;
      }
    }
    //if (jmax == 0) {
      objectives.push_back(objective(artefact, 10, 1, 2, 100000));
      endgame_goal = artefact;
      //} else {
      //position b = base_joueur(players_ids[jmax]);
      //objectives.push_back(objective(b, 10, 1000000, 1, 100000));
      //endgame_goal = b;
      //}
}

int menace_base = 0;
void update_objectives() {
  cout << "Objectives\n";
  vector<objective> old_objectives;
  objectives.swap(old_objectives);
  vector<position> sadv = sorciers_adv();
  int menace = 0;
  int menace2 = 0;
  for (unsigned int i = 0; i < sadv.size(); i++) {
    if (distance(sadv[i], base_joueur(moi())) <= PORTEE_SORCIER) {
      menace += nb_sorciers_adv(sadv[i]);
    }
    if (distance(sadv[i], base_joueur(moi())) <= 2*PORTEE_SORCIER) {
      menace2 += nb_sorciers_adv(sadv[i]);
    }
  }
  if (menace2 < nb_sorciers(base_joueur(moi()), moi()) && menace < nb_sorciers(base_joueur(moi()), moi())/3) {
    panic = false;
  }
  menace_base = menace;
  int ns = nb_sorciers(base_joueur(moi()), moi());
  if (menace > ns || menace2 > 2*ns) {
    //objective panic = objective(base_joueur(moi()), 100, 3, 10, 200);
    //panic.tower_s = 3;
    //objectives.push_back(panic);
    panic = true;
    cout << "Panic mode\n";
  }
  if (tour_actuel() == 92) {
    endgame_objectives();
    return;
  }
 
  bool found = false;
  for (unsigned int i = 0; i < old_objectives.size(); i++) {
    int j = jbase(old_objectives[i].pos);
    if (j != -1) {
      cout << "Ennemi : " << old_objectives[i].pos.x << " " << old_objectives[i].pos.y << endl;
      if (!elimine(players_ids[j])) {
	if (!found) {
	  old_objectives[i].sorciers += 10;
	}
	found = true;
	//old_objectives[i].sorciers += 5;
	objectives.push_back(old_objectives[i]);
      }
    } else {
      objectives.push_back(old_objectives[i]);
    }
  }
}

position autour_artefact[4] = {position(TAILLE_TERRAIN/2 - 1, TAILLE_TERRAIN/2),
			       position(TAILLE_TERRAIN/2 + 1, TAILLE_TERRAIN/2),
			       position(TAILLE_TERRAIN/2, TAILLE_TERRAIN/2 - 1),
			       position(TAILLE_TERRAIN/2, TAILLE_TERRAIN/2 + 1)};

inline void phase_construction_() {
  update_objectives();
  if (tour_actuel() == 1) {
    creer(magie(moi())/COUT_SORCIER);
  } else if (tour_actuel() > 96 && tour_actuel() < 100 && !panic) {
    return;
  } else if (tour_actuel() == 100 && !panic && (endgame_goal != artefact || (joueur_case(artefact) == -1) || true)) {
    vector<tourelle> tourelles = tourelles_joueur(moi());
    for (unsigned int i = 0; i < tourelles.size(); i++) {
      if (distance(tourelles[i].pos, artefact) > 5) {
	supprimer(tourelles[i].pos);
      }
    }
    for (int i = 0; i < 4; i++) {
      construire(autour_artefact[i], 3);
    }
    return;
  }
  vector<tourelle> tourelles = tourelles_joueur(moi());
  bool utilise[9]  = {false};
  for (unsigned int i = 0; i < objectives.size(); i++) {
    int dmin = INF;
    int jmin = 0;
    for (unsigned int j = 0; j < tourelles.size(); j++) {
      if (distance(tourelles[j].pos, objectives[i].pos) < dmin) {
	dmin = distance(tourelles[j].pos, objectives[i].pos);
	jmin = j;
      }
    }
    utilise[jmin] = true;
  }
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    if (!utilise[i]) {
      cout << tourelles[i].pos.x << " " << tourelles[i].pos.y << "\n";
      supprimer(tourelles[i].pos);
    }
  }
  if(panic) {
    while(construire_vers(base_joueur(moi()), true));
  }
  sort(objectives.begin(), objectives.end());
  for (unsigned int i = 0; i < objectives.size(); i++) {
    objectives[i].tower_s++;
    if (objectives[i].tower_s >= objectives[i].tower_delay) {
      if (construire_vers(objectives[i].pos)) {
	objectives[i].tower_s = 0;
	objectives[i].tower_increase_s++;
	if (objectives[i].tower_increase_s >= objectives[i].tower_increase_delay) {
	    objectives[i].tower_increase_s = 0;
	    objectives[i].tower_delay++;
	}
      }
    }
  }
  creer(9*magie(moi())/10/COUT_SORCIER);
}

void phase_construction() {
  clock_t t = clock();
  phase_construction_();
  t = clock() - t;
  cout << moi() << " phase_constuction : " << ((float)t)/CLOCKS_PER_SEC << "s\n"; 
}

inline void phase_deplacement_() {
  update_danger();
  cout << "Deplacement\n";
  vector<position> positions = sorciers(moi());
  vector<int> objectives_sorciers;
  for (unsigned int i = 0; i < objectives.size(); i++) {
    objectives_sorciers.push_back(objectives[i].sorciers);
  }
  /*while (1) {
    int smax = 0;
    int imax = -1;
    for (unsigned int i = 0; i < objectives.size(); i++) {
      if (objectives_sorciers[i] > smax) {
	smax = objectives_sorciers[i];
	imax = i;
      }
    }
    if (smax == 0)
      return;

    int dmin = INF;
    int omin = -1;
    for (unsigned int i = 0; i < positions.size(); i++) {
      int ns = nb_sorciers_deplacables(positions[i], moi());
      if ((ns > 0) && ((10*distance(positions[i], objectives[imax].pos)/objectives[imax].value) < dmin)) {
	dmin = 10*distance(positions[i], objectives[imax].pos)/objectives[imax].value;
	omin = i;
      }
    }
    if (dmin == INF)
      return;
    
    int ns = min(objectives_sorciers[imax], nb_sorciers_deplacables(positions[omin], moi()));
    objectives_sorciers[imax] -= ns;
    deplacer_(positions[omin], objectives[imax].pos, ns);
    
    }*/
  for (unsigned int i = 0; i < positions.size(); i++) {
    position p1 = positions[i];
    int ns = nb_sorciers_deplacables(p1, moi());
    if (p1 == base_joueur(moi())) {
      ns = max(0, ns - 2*menace_base);
    } else if (info_case(p1) == CASE_FONTAINE) {
      ns = max(0, ns - 10);
    }
    float dmin = INF;
    int omin = -1;
    //int dmin = INF;
    //int omin = -1;
    if(panic)
      dmin = 10*distance(p1, base_joueur(moi()))/(150.);
    //unsigned int os = min((int)objectives.size(), 4); 
    unsigned int os = objectives.size();
    for (unsigned int j = 0; j < os; j++) {
      if ((10.*distance(p1, objectives[j].pos))/objectives[j].value < dmin && objectives_sorciers[j] > 0) {
	dmin = (10.*distance(p1, objectives[j].pos))/objectives[j].value;
	omin = j;
      }
    }
    if (omin == -1) {
      //if (panic) {
	position pp = base_joueur(moi());
	deplacer(p1, pp, ns);
	//}
      continue;
    }
    objectives_sorciers[omin] -= ns;
    deplacer_(p1, objectives[omin].pos, ns);
  }
    //vector<unsigned int> objs;
    //if (p1 == base_joueur(moi())) {
    //  for (unsigned int i = 0; i < objectives.size(); i++) {
    //	objs.push_back(i);
    //  }
    /*} else {
      int dmin = INF;
      for (unsigned int i = 0; i < objectives.size(); i++) {
	int d = distance(p1, objectives[i].pos);
	if (d < dmin) {
	  dmin = d;
	  objs.clear();
	  objs.push_back(i);
	} else if (d == dmin) {
	  objs.push_back(i);
	}
      }
      }*/
    //int s = 0;
    //for (unsigned int i = 0; i < objs.size(); i++) {
    //  s += objectives[objs[i]].value;
    //}
    //s = objectives[0].value;
    //objs.clear(); objs.push_back(0);
    //for (unsigned int i = 0; i < objs.size(); i++) {
    //  deplacer_(p1, objectives[objs[i]].pos, (objectives[objs[i]].value*ns)/s);
    //}
    //cout << endl;
  //}
}

void phase_deplacement() {
  clock_t t = clock();
  phase_deplacement_();
  t = clock() - t;
  cout << moi() << " phase_deplacements : " << ((float)t)/CLOCKS_PER_SEC << "s\n"; 
}

inline void phase_tirs_() {
  vector<tourelle> tourelles = tourelles_joueur(moi());
  vector<position> sadv = sorciers_adv();
  int n = tourelles.size() + sadv.size() + 2;
  //assert(n <= MAXN);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      capacites[i][j] = 0;
    }
  }
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    capacites[0][i+1] = ATTAQUE_TOURELLE;
  }
  int m = tourelles.size(); 
  for (unsigned int i = 0; i < sadv.size(); i++) {
    int d = nb_sorciers_adv(sadv[i]);
    capacites[i+m+1][n-1] = d;
  }
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    for (unsigned int j = 0; j < sadv.size(); j++) {
      if (distance(tourelles[i].pos, sadv[j]) <= tourelles[i].portee) {
	capacites[i+1][j+m+1] = INF;
      }
    }
  }
  max_flow(n);
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    for (unsigned int j = 0; j < sadv.size(); j++) {
      tirer(flot[i+1][j+m+1], tourelles[i].pos, sadv[j]);
    }
  }
}

void phase_tirs() {
  clock_t t = clock();
  phase_tirs_();
  t = clock() - t;
  cout << moi() << " phase_tirs : " << ((float)t)/CLOCKS_PER_SEC << "s\n"; 
}

inline void phase_siege_() {
  vector<tourelle> tourelles = tourelles_adv();
  vector<position> smoi = sorciers(moi());
  int n = tourelles.size() + smoi.size() + 2;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      capacites[i][j] = 0;
    }
  }
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    capacites[0][i+1] = tourelles[i].vie;
  }
  int m = tourelles.size(); 
  for (unsigned int i = 0; i < smoi.size(); i++) {
    int d = nb_sorciers(smoi[i], moi());
    capacites[i+m+1][n-1] = d;
  }
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    for (unsigned int j = 0; j < smoi.size(); j++) {
      if (distance(tourelles[i].pos, smoi[j]) <= 1) {
	capacites[i+1][j+m+1] = INF;
      }
    }
  }
  max_flow(n);
  //for (unsigned int u = 0; u < 10; u++) {
  for (unsigned int i = 0; i < tourelles.size(); i++) {
    for (unsigned int j = 0; j < smoi.size(); j++) {
      assieger(smoi[j], tourelles[i].pos, flot[i+1][j+m+1]);
    }
  }
  //}

  tact++;
}

void phase_siege() {
  clock_t t = clock();
  phase_siege_();
  t = clock() - t;
  cout << moi() << " phase_siege : " << ((float)t)/CLOCKS_PER_SEC << "s\n"; 
}

void partie_fin() {}

