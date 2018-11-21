#include <iostream>
#include <random>
#include <time.h>
#include <assert.h>
using namespace std;

/** Taille du plateau en P4S */
const int NBMAX = 8;

/** Nombre de lignes en puissance 4 */
const int LP4 = 6;

/** Nombre de colonnes en puissance 4 */
const int CP4 = 7;

/** Nombre maximal de joueur */
const size_t NJOUEURS = 2;

/** Joueur
	1 : joueur n°1
	2 : joueur n°2
	0 : joueur non déclarer
	-1 : tous les joueurs
	*/
using Joueur = int;

/** Symboles pour représenter les joueurs  */
using Symbole = char;

/** plateau de jeu */
struct Grille
{
	/** Tableau contenant toute les cases de la grille */
	Joueur tab[NBMAX][NBMAX];
	/** Mode de jeu : Puissance 4 ou P4S */
	bool mode;
};

struct Position
{
	/** Ligne */
	int lin;
	/** Colonne */
	int col;
};

/** Permer de definir les MAX en fontion du mode de jeu,
	Remplace dans le code "initMax" par "int MAXL = gr.mode ? NBMAX : LP4, MAXC = gr.mode ? NBMAX : CP4" */
#define initMax size_t MAXL = gr.mode ? NBMAX : LP4, MAXC = gr.mode ? NBMAX : CP4

	/**
		Teste si la grille est pleine
		@param[in] gr - Une grille
		@return si la grille est pleine
	*/
bool Grillepleine(const Grille &gr)
{
	initMax;
	for (size_t i = 0; i < MAXL; i++)
	{
		for (size_t j = 0; j < MAXC; j++)
		{
			if (gr.tab[i][j] == 0) {
				return false;
			}
		}
	}
	return true;
}

/**
	Teste si la grille à été inicialisé (pour les asserts)
	@param[in] gr - Une grille
*/
bool grilleInit(const Grille &gr)
{
	initMax;
	for (size_t i = 0; i < MAXL; i++)
	{
		for (size_t j = 0; j < MAXC; j++)
		{
			if (gr.tab[i][j]<0 || gr.tab[i][j] > static_cast<Joueur>(NJOUEURS)) {
				return false;
			}
		}
	}
	return true;
}

/**
	Teste si le tableau de symboles à été inicialisé (pour les asserts)
	@param[in] Sj - Tableau contenant les symboles
*/
bool symbolesInit(const Symbole Sj[NJOUEURS + 1])
{
	for (size_t i = 0; i <= static_cast<Joueur>(NJOUEURS); i++)
	{
		// Si ce n'est pas un caractère affichable
		if (!((Sj[i] >= static_cast<char>(32) && Sj[i] <= static_cast<char>(126)) || Sj[i] == static_cast<char>(9)))
		{
			return false;
		}
	}
	return true;
}

/**
		Demande au joueur où il veut plasser son point et le place
		@param[in/out] gr - Une grille
		@param[in] jr - Le joueur actif
		@pre gr initialisée
		@pre jr dans {1,NJOUEURS}
	*/
void placerPion(Grille &gr, Joueur jr)
{
	assert(grilleInit(gr));
	assert(jr >= 1 && jr <= static_cast<Joueur>(NJOUEURS));
	initMax;

	size_t col; /** colonne où le point est placer */
	bool corecte = false;
	do
	{
		cout << "Joueur " << jr << " sur quel colonne places-tu ton pion ? ";
		cin >> col;
		--col; // pour le joueur les ° des colonnes commence par 1
		if (!cin.good()) // Un caractère non numérique à été saisie
		{
			cin.clear();
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n'); // nettoie la pile d'attente du cin
		}
		else if (col < 0 || col >= MAXC) {
			cout << "La colonne n'est pas dans la grille" << endl;
		}
		else if (gr.tab[0][col] != 0) {
			cout << "La colonne est pleine" << endl;
		}
		else
		{
			corecte = true;
		}
	} while (!corecte);
	int i = MAXL - 1;
	while (gr.tab[i][col] != 0)
	{
		--i;
	}
	gr.tab[i][col] = jr;
}

/**
		Détermine aléatoirement le joueur qui commence
		@return Le joueur déterminé
	*/
Joueur ramdomJoueur()
{
	return static_cast<Joueur>(rand() % NJOUEURS + 1);
}

/**
		Initialise la grille avec des 0 et son mode de jeu
		@param[out] gr - Une grille
		@param[in] mode - Le mode de jeu qui va être utlisé
	*/
void initGrille(Grille &gr, bool mode)
{
	gr.mode = mode;
	initMax;
	for (size_t i = 0; i < MAXL; i++)
	{
		for (size_t j = 0; j < MAXC; j++)
		{
			gr.tab[i][j] = 0;
		}
	}
}

/**
		Demande aux joueurs les symboles qu'ils veulent utiliser
		@param[out] Sj - Tableau contenant les symboles
	*/
void assSymbole(Symbole Sj[NJOUEURS + 1])
{
	cout << "Quel est le symbole de case vide : ";
	cin >> Sj[0];
	for (Joueur i = 1; i < static_cast<Joueur>(NJOUEURS) + 1; i++)
	{
		cout << "Joueur " << i << " quel est ton symbole : ";
		cin >> Sj[i];
	}
}

/**
		Affiche la grille avec le symbole des joueurs
		@param[in] gr - Une grille
		@param[in] Sj - Tableau contenant les symboles
		@pre gr initialisée
		@pre Sj initialisée
	*/
void afficherGrille(const Grille &gr, const Symbole Sj[NJOUEURS + 1])
{
	assert(grilleInit(gr));
	assert(symbolesInit(Sj));
	initMax;
	for (size_t i = 0; i < MAXL; i++)
	{
		for (size_t j = 0; j < MAXC; j++)
		{
			cout << Sj[gr.tab[i][j]] << ' ';
		}
		cout << endl;
	}
}

/**
		Teste si la case est gagnante dans les 4 directions (horizontal, vertical, 2 diagonales)
		@param[in] gr - Une grille
		@param[in] i - Ligne de la case à tester
		@param[in] j - Colonne de la case à tester
		@return Le joueur gagnant en cette case ou 0 si pas de gagnant
		@pre gr initialisée
		@pre i dans {0,MAXL}
		@pre j dans {0,MAXC}
	*/
Joueur checkCase(const Grille &gr, size_t i, size_t j)
{
	assert(grilleInit(gr));
	initMax;
	assert(i <= MAXL); // size_t --> forcément supérieur à 0
	assert(j <= MAXC);
	Joueur jr = gr.tab[i][j];
	if (jr != 0) {
		vector<Position> directions;
		directions.reserve(4);
		//On cherche toutes les directions possible en fonction de la position de la case sur la grille
		if (j <= MAXC - 4) {
			directions.push_back({ 0,1 });
			if (i <= MAXC - 4) {
				directions.push_back({ 1,0 });
				directions.push_back({ 1,1 });
			}
			if (i >= 3)
			{
				directions.push_back({ -1,1 });
			}
		}
		else if (i <= MAXC - 4) {
			directions.push_back({ 1,0 });
		}

		for (const Position &d : directions)
		{
			//si il y a 4 cases du même joueur
			if (gr.tab[i + d.lin][j + d.col] == jr &&
				gr.tab[i + d.lin * static_cast <size_t>(2)][j + d.col * static_cast <size_t>(2)] == jr &&
				gr.tab[i + d.lin * static_cast <size_t>(3)][j + d.col * static_cast <size_t>(3)] == jr)
			{
				return jr;
			}
		}
	}
	return 0;
}

/**
		Cherche sur toute la grille si un ou plusieurs joueurs ont gagner
		@param[in] gr - Une grille
		@return Le joueur gagnant en cette case ou 0 si pas de gagnant ou -1 si égalité
		@pre gr initialisée
	*/
Joueur Gagner(const Grille &gr)
{
	assert(grilleInit(gr));
	initMax;
	Joueur gagnant = 0;
	Joueur gagnantP4S = 0;
	for (size_t i = 0; i < MAXL; i++)
	{
		for (size_t j = 0; j < MAXC; j++)
		{
			gagnant = checkCase(gr, i, j);
			if (gagnant) {
				if (!gr.mode) //Puissance 4
					return gagnant;
				else		  //P4S
				{
					if (gagnantP4S != 0 && gagnantP4S != gagnant) {
						return -1; //Egalité
					}
					else
					{
						gagnantP4S = gagnant;
					}
				}
			}
		}
	}
	return gagnantP4S; // = 0 en puissance 4 sinon le l'unique gagnant en P4S
}

/**
		Enlèves des cases à la grille si 2 cases d'un joueur entoure 3 cases d'un autre joueur
		@param[in/out] gr - Une grille
		@return Si des cases ont étaient enlevé
		@pre gr initialisée
	*/
bool enleverCases(Grille &gr)
{
	assert(grilleInit(gr));
	initMax;
	for (size_t i = 0; i < MAXL; i++)
	{
		for (size_t j = 0; j <= MAXC - 5; j++)
		{
			//Si 2 cases d'un joueur entoure 3 cases d'un autre joueur
			if (gr.tab[i][j] != 0 && gr.tab[i][j] != gr.tab[i][j + 1] &&
				gr.tab[i][j + 1] != 0 &&
				gr.tab[i][j + 2] == gr.tab[i][j + 1] &&
				gr.tab[i][j + 3] == gr.tab[i][j + 1] &&
				gr.tab[i][j + 4] == gr.tab[i][j])
			{
				//pour chaqu'une des 3 cases
				for (size_t j2 = 1; j2 <= 3; j2++)
				{
					//Faire tomber les cases au dessus
					size_t i2 = i;
					for (; i2 > 0 && gr.tab[i2 - 1][j2] != 0; i2--)
					{
						gr.tab[i2][j + j2] = gr.tab[i2 - 1][j + j2];
					}
					gr.tab[i2][j + j2] = 0;
				}
				return true;
			}
		}
	}
	return false;
}

/** Fonction principal du puissance 4 */
void jouerPartirP4()
{
	srand(static_cast<unsigned int>(time(NULL)));
	Grille gr;
	Joueur j;
	Symbole Sj[NJOUEURS + 1];
	Joueur gagnant = 0;
	initGrille(gr, false);
	assSymbole(Sj);
	afficherGrille(gr, Sj);
	j = ramdomJoueur();
	do
	{
		placerPion(gr, j);
		afficherGrille(gr, Sj);
		j = j % static_cast<Joueur>(NJOUEURS) + 1; //change de joueur actif
		gagnant = Gagner(gr);
	} while (gagnant == 0 && !Grillepleine(gr));
	if (gagnant) {
		cout << endl << "Le gagnant est le joueur " << gagnant << endl;
	}
}

/** Fonction principal du P4S */
void jouerPartirP4S()
{
	srand(static_cast<unsigned int>(time(NULL)));
	Grille gr;
	Joueur j;
	Symbole Sj[NJOUEURS + 1];
	Joueur gagnant = 0;
	initGrille(gr, true);
	assSymbole(Sj);
	afficherGrille(gr, Sj);
	j = ramdomJoueur();
	do
	{
		placerPion(gr, j);
		while (enleverCases(gr));
		afficherGrille(gr, Sj);
		j = j % static_cast<Joueur>(NJOUEURS) + 1; //change de joueur actif
		gagnant = Gagner(gr);
	} while (gagnant == 0 && !Grillepleine(gr));
	if (gagnant == -1) {
		cout << "Egalitee " << endl;
	}
	else if (gagnant)
	{
		cout << endl << "Le gagnant est le joueur " << gagnant << endl;
	}
}
int main()
{
	jouerPartirP4S();
	return 0;
}
