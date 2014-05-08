///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#ifndef PROLOGIN_HH_
# define PROLOGIN_HH_

# include <vector>

# include <string>

///
// Taille du terrain (longueur et largeur)
//
# define TAILLE_TERRAIN            31

///
// Nombre de joueurs dans la partie
//
# define NB_JOUEURS                4

///
// Nombre maximum de tours à jouer avant la fin de la partie
//
# define MAX_TOUR                  100

///
// Magie gagnée à chaque tour
//
# define MAGIE_TOUR                20

///
// Magie gagnée à chaque tour pour chaque fontaine possédée
//
# define MAGIE_FONTAINES           15

///
// Magie gagnée à chaque sorcier tué
//
# define MAGIE_COMBAT              1

///
// Magie récupérée à chaque tourelle supprimée
//
# define MAGIE_SUPPRESSION         10

///
// Nombre de points de magie par sorcier
//
# define COUT_SORCIER              2

///
// Nombre de points de magie par tourelle
//
# define COUT_TOURELLE             20

///
// Coût exponentiel pour chaque case de portée supplémentaire
//
# define COUT_PORTEE               4

///
// Nombre maximum de cases qu'un sorcier peut franchir à chaque tour.
//
# define PORTEE_SORCIER            4

///
// Portée de base d'une tourelle
//
# define PORTEE_TOURELLE           3

///
// Portée de construction des tourelles
//
# define CONSTRUCTION_TOURELLE     3

///
// Points de vie d'une tourelle à sa création
//
# define VIE_TOURELLE              10

///
// Points d'attaque d'une tourelle au début d'un tour
//
# define ATTAQUE_TOURELLE          10

///
// Points gagnés pour avoir survécu à la fin de la partie
//
# define POINTS_SURVIVRE           1

///
// Points gagnés pour avoir vaincu un adversaire
//
# define POINTS_VAINQUEUR          1

///
// Points gagnés pour contrôler une fontaine à la fin de la partie
//
# define POINTS_CONTROLE_FONTAINE  1

///
// Points gagnés pour contrôler un artefact à la fin de la partie
//
# define POINTS_CONTROLE_ARTEFACT  4

///
// Information sur les cases
//
typedef enum case_info {
  CASE_SIMPLE, /* <- Case simple */
  CASE_TOURELLE, /* <- Tourelle */
  CASE_BASE, /* <- Base du joueur */
  CASE_FONTAINE, /* <- Fontaine magique */
  CASE_ARTEFACT, /* <- Artefact magique */
  CASE_ERREUR, /* <- Erreur */
} case_info;


///
// Erreurs possibles
//
typedef enum erreur {
  OK, /* <- L'action s'est effectuée avec succès */
  ANNULER_IMPOSSIBLE, /* <- Aucune action à annuler */
  CASE_IMPOSSIBLE, /* <- Cette case n'existe pas */
  CASE_ADVERSE, /* <- Vous ne contrôlez pas cette case */
  CASE_UTILISEE, /* <- Cette case n'est pas libre */
  CASE_VIDE, /* <- Cette case est vide */
  VALEUR_INVALIDE, /* <- Cette valeur est invalide */
  MAGIE_INSUFFISANTE, /* <- Vous n'avez pas assez de magie */
  SORCIERS_INSUFFISANTS, /* <- Vous n'avez pas assez de sorciers */
  ATTAQUE_INSUFFISANTE, /* <- Vous n'avez pas assez de points d'attaque */
  PHASE_INCORRECTE, /* <- Cette action ne peut pas être utilisée lors de cette phase du jeu. */
  PORTEE_INSUFFISANTE, /* <- Vous n'avez pas assez de portée pour effectuer cette action */
  PERDANT, /* <- Vous avez perdu et ne pouvez pas effectuer d'actions */
} erreur;


///
// Représente la position sur la carte
//
typedef struct position {
  int x;  /* <- Coordonnée en X */
  int y;  /* <- Coordonnée en Y */
} position;


///
// Représente une tourelle
//
typedef struct tourelle {
  position pos;  /* <- Position de la tourelle */
  int portee;  /* <- Portée de la tourelle */
  int joueur;  /* <- Joueur qui possède la tourelle */
  int vie;  /* <- Nombre de points de vie de la tourelle */
  int attaque;  /* <- Nombre de points d'attaque de la tourelle */
} tourelle;


///
// Retourne le type de la case à l'emplacement `pos`
//
extern "C" case_info api_info_case(position pos);
static inline case_info info_case(position pos)
{
  return api_info_case(pos);
}


///
// Retourne la liste des tourelles qui appartiennent au joueur ``joueur``
//
extern "C" std::vector<tourelle> api_tourelles_joueur(int joueur);
static inline std::vector<tourelle> tourelles_joueur(int joueur)
{
  return api_tourelles_joueur(joueur);
}


///
// Retourne la magie que possède le joueur ``joueur``
//
extern "C" int api_magie(int joueur);
static inline int magie(int joueur)
{
  return api_magie(joueur);
}


///
// Retourne le nombre de sorciers du joueur ``joueur`` sur la case ``pos``
//
extern "C" int api_nb_sorciers(position pos, int joueur);
static inline int nb_sorciers(position pos, int joueur)
{
  return api_nb_sorciers(pos, joueur);
}


///
// Retourne le nombre de sorciers du joueur ``joueur`` déplacables sur la case ``pos``
//
extern "C" int api_nb_sorciers_deplacables(position pos, int joueur);
static inline int nb_sorciers_deplacables(position pos, int joueur)
{
  return api_nb_sorciers_deplacables(pos, joueur);
}


///
// Retourne le numéro du joueur qui contrôle la case ``pos``
//
extern "C" int api_joueur_case(position pos);
static inline int joueur_case(position pos)
{
  return api_joueur_case(pos);
}


///
// Retourne la tourelle située sur la case ``pos``
//
extern "C" tourelle api_tourelle_case(position pos);
static inline tourelle tourelle_case(position pos)
{
  return api_tourelle_case(pos);
}


///
// Retourne la position de la base du joueur ``joueur``
//
extern "C" position api_base_joueur(int joueur);
static inline position base_joueur(int joueur)
{
  return api_base_joueur(joueur);
}


///
// Retourne vrai si l'on peut construire sur la case ``pos``
//
extern "C" bool api_constructible(position pos, int joueur);
static inline bool constructible(position pos, int joueur)
{
  return api_constructible(pos, joueur);
}


///
// Retourne la liste des positions constituant le plus court chemin allant de la case ``pos1`` à la case ``pos2``. Attention : Cette fonction est lente.
//
extern "C" std::vector<position> api_chemin(position pos1, position pos2);
static inline std::vector<position> chemin(position pos1, position pos2)
{
  return api_chemin(pos1, pos2);
}


///
// Construire une tourelle à la position ``pos``
//
extern "C" erreur api_construire(position pos, int portee);
static inline erreur construire(position pos, int portee)
{
  return api_construire(pos, portee);
}


///
// Supprimer une tourelle à la position ``pos``
//
extern "C" erreur api_supprimer(position pos);
static inline erreur supprimer(position pos)
{
  return api_supprimer(pos);
}


///
// Tirer avec ``pts`` points de dégats depuis la tourelles ``tourelle`` sur la position ``cible``
//
extern "C" erreur api_tirer(int pts, position tourelle, position cible);
static inline erreur tirer(int pts, position tourelle, position cible)
{
  return api_tirer(pts, tourelle, cible);
}


///
// Créer ``nb`` sorciers dans la base
//
extern "C" erreur api_creer(int nb);
static inline erreur creer(int nb)
{
  return api_creer(nb);
}


///
// Déplace ``nb`` sorciers de la position ``depart`` jusqu'à la position ``arrivee``.
//
extern "C" erreur api_deplacer(position depart, position arrivee, int nb);
static inline erreur deplacer(position depart, position arrivee, int nb)
{
  return api_deplacer(depart, arrivee, nb);
}


///
// Attaquer la tourelle à la position ``cible`` depuis la position ``pos``
//
extern "C" erreur api_assieger(position pos, position cible, int nb_sorciers);
static inline erreur assieger(position pos, position cible, int nb_sorciers)
{
  return api_assieger(pos, cible, nb_sorciers);
}


///
// Retourne le numéro de votre joueur
//
extern "C" int api_moi();
static inline int moi()
{
  return api_moi();
}


///
// Retourne la liste des numéros de vos adversaires
//
extern "C" std::vector<int> api_adversaires();
static inline std::vector<int> adversaires()
{
  return api_adversaires();
}


///
// Retourne le numéro du tour actuel
//
extern "C" int api_tour_actuel();
static inline int tour_actuel()
{
  return api_tour_actuel();
}


///
// Retourne la distance entre deux positions
//
extern "C" int api_distance(position depart, position arrivee);
static inline int distance(position depart, position arrivee)
{
  return api_distance(depart, arrivee);
}


///
// Annule la dernière action
//
extern "C" erreur api_annuler();
static inline erreur annuler()
{
  return api_annuler();
}


///
// Affiche le contenu d'une valeur de type case_info
//
extern "C" void api_afficher_case_info(case_info v);
static inline void afficher_case_info(case_info v)
{
  api_afficher_case_info(v);
}


///
// Affiche le contenu d'une valeur de type erreur
//
extern "C" void api_afficher_erreur(erreur v);
static inline void afficher_erreur(erreur v)
{
  api_afficher_erreur(v);
}


///
// Affiche le contenu d'une valeur de type position
//
extern "C" void api_afficher_position(position v);
static inline void afficher_position(position v)
{
  api_afficher_position(v);
}


///
// Affiche le contenu d'une valeur de type tourelle
//
extern "C" void api_afficher_tourelle(tourelle v);
static inline void afficher_tourelle(tourelle v)
{
  api_afficher_tourelle(v);
}



extern "C" {

///
// Fonction appelée au début de la partie
//
void partie_debut();

///
// Fonction appelée pendant la phase de construction
//
void phase_construction();

///
// Fonction appelée pendant la phase de déplacement
//
void phase_deplacement();

///
// Fonction appelée pendant la phase de tirs des tourelles
//
void phase_tirs();

///
// Fonction appelée pendant la phase de siège des tourelles
//
void phase_siege();

///
// Fonction appelée à la fin de la partie
//
void partie_fin();

}
#endif
