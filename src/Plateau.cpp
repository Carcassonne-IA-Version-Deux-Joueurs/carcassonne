// LIBRAIRIES
#include "Plateau.hpp"

/* Initialise variable statique d'une instance Plateau */
int Plateau::current_index_plateau = 0;
std::vector<Noeud *> Plateau::list_noeuds;
std::vector<Plateau *> Plateau::list_plateau;
Plateau * Plateau::current_plateau = nullptr;

// FONCTIONS
Plateau::Plateau()
{
    Logging::log(Logging::TRACE, "Création du plateau");
}

// créer une nouvelle image du plateau
Plateau::Plateau(const Plateau & plateau)
{
    Logging::log(Logging::TRACE, "clonnage du plateau %d", &plateau);
    this->grille = plateau.grille;
    this->pioche = plateau.pioche;

    this->nombre_tuiles_type_pioche[0] = 4;
    this->nombre_tuiles_type_pioche[1] = 3;
    this->nombre_tuiles_type_pioche[2] = 3;
    this->nombre_tuiles_type_pioche[3] = 5;
    this->nombre_tuiles_type_pioche[4] = 3;
    this->nombre_tuiles_type_pioche[5] = 2;
    this->nombre_tuiles_type_pioche[6] = 8;
    this->nombre_tuiles_type_pioche[7] = 9;
    this->nombre_tuiles_type_pioche[8] = 3;
    this->nombre_tuiles_type_pioche[9] = 2;
    this->nombre_tuiles_type_pioche[10] = 3;
    this->nombre_tuiles_type_pioche[11] = 2;
    this->nombre_tuiles_type_pioche[12] = 1;
    this->nombre_tuiles_type_pioche[13] = 2;
    this->nombre_tuiles_type_pioche[14] = 3;
    this->nombre_tuiles_type_pioche[15] = 1;
    this->nombre_tuiles_type_pioche[16] = 1;
    this->nombre_tuiles_type_pioche[17] = 1;
    this->nombre_tuiles_type_pioche[18] = 2;
    this->nombre_tuiles_type_pioche[19] = 4;
    this->nombre_tuiles_type_pioche[20] = 2;
    this->nombre_tuiles_type_pioche[21] = 3;
    this->nombre_tuiles_type_pioche[22] = 4;
    this->nombre_tuiles_type_pioche[23] = 1;
    
    this->liste_tuiles_emplacements_libres = plateau.liste_tuiles_emplacements_libres; 
    this->element_libre = plateau.element_libre;
    
    this->mapJoueursPions = plateau.mapJoueursPions; 
    this->tuiles_candidates = plateau.tuiles_candidates;

    this->noeuds_plateau = plateau.noeuds_plateau; 
}


Plateau::~Plateau()
{
    /*
    for(auto mapjoueurpion : mapJoueursPions) 
    {
        Pion * pion = this->get_pion_joueur(mapjoueurpion.first); 
        delete pion;
        delete mapjoueurpion.first;
    }

    for(int i = 0; i < 144; i++)
    {
        for(int j = 0; j < 144; j++)
        {
            if(this->grille[i][j] != nullptr)
            {
                delete this->grille[i][j];
            }
        }
    }
    */
}

void Plateau::init_plateau()
{

    Logging::log(Logging::TRACE, "Initilisation de la grille");

    for(int i = 0; i < (NBR_TUILES * 2); i++)
    {
        for(int j = 0; j < (NBR_TUILES * 2); j++)
        {
            current_plateau->grille[i][j] = nullptr;
        }
    }
   
    Logging::log(Logging::DEBUG, "Ajout de la Tuile de base au plateau %d ", current_plateau->pioche[0]);
    std::array<int, 3> coord = {NBR_TUILES-1, NBR_TUILES-1, 0};
    current_plateau->poser_tuile(current_plateau->pioche[0], coord);
    current_plateau->pioche.erase(current_plateau->pioche.begin());
    Logging::log(Logging::DEBUG, "pioche size :%d", current_plateau->pioche.size());
}

void Plateau::ajouter_joueur(Joueur * joueur, Pion * pion)
{
    current_plateau->mapJoueursPions.insert(std::pair<Joueur *, Pion *>(joueur, pion));
}

std::map<Noeud *, std::vector<Noeud*>> * get_table_node_data()
{
    /*
    return & Plateau::list_plateau[Plateau::current_index_plateau]->noeuds_plateau;
    */
    return nullptr;
}

Tuile *Plateau::piocher_tuile_aleat()
{
    srand(time(NULL));
	int random = rand() % current_plateau->pioche.size();
    Logging::log(Logging::TRACE, "Pioche la tuile à l'emplacement %d", random);
    return current_plateau->piocher_tuile(random);
}

Tuile *Plateau::piocher_tuile(int index)
{
    Tuile *tuile = current_plateau->pioche[index];
	current_plateau->pioche.erase(current_plateau->pioche.begin() + index);
    return tuile;
}

Tuile *Plateau::piocher_tuile_type(int type)
{
    for(unsigned long int i = 0; i < this->pioche.size(); i++)
    {
        if(this->pioche.at(i)->get_id_groupe() == type)
        {
            return piocher_tuile(i);
        }
    }

    return piocher_tuile_aleat();
}

Tuile *Plateau::get_tuile_grille(int x, int y)
{
    return current_plateau->grille[x][y];
}

/**
 * @title : Calculer les emplacments libres
 *
 * tuiles candidates, on effectue des rotations et on vérifie si les 
 * interfaces de bordures sont compatibles.
 *
 * @param : tuile : tuile à poser
 * */
void Plateau::calcul_emplacements_libres(Tuile *tuile)
{
    current_plateau->liste_tuiles_emplacements_libres.clear();
    // Logging::log(Logging::TRACE, "Tuile = %d", tuile->get_id());
    // On parcourt la liste des emplacements candidates
    for(auto tuile_candidate : current_plateau->tuiles_candidates) 
    {
        std::pair<int, int> emplacement = tuile_candidate.second;
        // Logging::log(Logging::DEBUG, "Tuile candidates : <%d,%d> %d", emplacement.first, emplacement.second, tuile_candidate.first->get_id());
        std::array<std::pair<int,int>, 4> tuile_coords_voisines = {
            std::make_pair(emplacement.first, emplacement.second + 1),
            std::make_pair(emplacement.first + 1, emplacement.second),
            std::make_pair(emplacement.first, emplacement.second - 1),
            std::make_pair(emplacement.first - 1, emplacement.second)
        };
        
        int bordure_voisines = 2;
        int bordure_tuiles = 0;


        for(int i = 0; i < 4; i++)  // on tourne la carte 
        {
            bool est_compatible = true; // permet de savoir si c'est compatible 
            
            for(auto tuile_coord_voisine : tuile_coords_voisines)
            {
            
                Tuile *tuile_voisine = current_plateau->grille[tuile_coord_voisine.first][tuile_coord_voisine.second];
                if(tuile_voisine != nullptr)
                {
                    if(tuile_voisine->get_id() != -1)
                    {
                        if(tuile_voisine->get_id() > 72)
                        {
                            Logging::log(Logging::DEBUG, "L'ID de la tuile voisine ne fait pas du domaine de définition");
                            
                        }

                        //Logging::log(Logging::TRACE, "Tuile voisine %d", tuile_voisine->get_id());
                        //Logging::log(Logging::TRACE, "Comparaison des bordures voisines <%d==%d> ", bordure_voisines, bordure_tuiles);
                         
                        Bordure *bordure_voisine = tuile_voisine->getBordure(bordure_voisines);
                        Bordure *bordure_tuile = tuile->getBordure(bordure_tuiles);
                        
                        if((bordure_voisine == nullptr) || (bordure_tuile == nullptr))
                        {
                            Logging::log(Logging::DEBUG, "La Bordure de la tuile de la tuile %d à comparer est null", tuile->get_id());
                            Logging::log(Logging::DEBUG, "La Bordure de la tuile voisine de la tuile %d à comparer est null", tuile_voisine->get_id());
                            
                        }
                        for(int k = 0; k < 3; k++) 
                        {
                            if((bordure_voisine->get_bordure_fils(2 - k) == nullptr) || (bordure_tuile->get_bordure_fils(k) == nullptr))
                            {
                                Logging::log(Logging::DEBUG, "La Bordure de la tuile fils %d de la tuile %d à comparer est null", 2-k, tuile->get_id());
                                Logging::log(Logging::DEBUG, "La Bordure de la tuile fils voisine %d de la tuile %d à comparer est null", k, tuile_voisine->get_id());
                                
                            }
                            //Logging::log(Logging::DEBUG, "Tuile voisine bordure %d", bordure_voisine->get_bordure_fils(k)->get_type_element());
                            //Logging::log(Logging::DEBUG, "Tuile bordure Tuile pioche %d", bordure_tuile->get_bordure_fils(k)->get_type_element());
                            Noeud::type_element type_element_voisine = bordure_voisine->get_bordure_fils(2 - k)->get_type_element();
                            Noeud::type_element type_element_tuile = bordure_tuile->get_bordure_fils(k)->get_type_element();
                            if(type_element_tuile == Noeud::VILLE || type_element_tuile == Noeud::VILLE_BLASON)
                            {
                                if(type_element_voisine != Noeud::VILLE && type_element_voisine != Noeud::VILLE_BLASON)
                                {
                                    est_compatible = false;
                                }

                            } 
                            else 
                            {
                                if(type_element_tuile != type_element_voisine)
                                {
                                    est_compatible = false;
                                }
                            }
                        }
                    } 
                }
                bordure_voisines = (1 + bordure_voisines) % 4;
                bordure_tuiles = (1 + bordure_tuiles) % 4;
            }
            if(est_compatible)
            {
                std::array<int, 3> emplacement_libre = {emplacement.first, emplacement.second, i};
                current_plateau->liste_tuiles_emplacements_libres.push_back(emplacement_libre);
                //Logging::log(Logging::DEBUG, "ajout de l'emplacement libre <%d,%d,%d>", emplacement_libre.at(0), emplacement_libre.at(1), emplacement_libre.at(2));
            }
            tuile->rotationHoraire();
        }   
        //Logging::log(Logging::DEBUG, "on change d'emplacement");
    }
    if(current_plateau->liste_tuiles_emplacements_libres.size() == 0) 
    {
        Logging::log(Logging::DEBUG, "Aucun emplacement libre de trouvé");
    }
}

/** 
 * @titre : Fonction qui permet de poser une tuile sur le plateau.
 *
 * @description : Place une tuile selon les paramètres données par 
 * emplacement. La tuile peut prendre la place d'une tuile candidates 
 * qui sera remplacé par la tuile placée. Les bordures sont connectées 
 * entre les deux tuiles.
 *
 * @param tuile : tuile à poser
 * @param coord : coordonnées de la tuile
 */
void Plateau::poser_tuile(Tuile *tuile, std::array<int, 3> emplacement)
{
    if(tuile->get_id() == -1 || tuile->get_id() > 72)
    {
        Logging::log(Logging::DEBUG, "La tuile ID %d ne peut pas être placé", tuile->get_id());
        return;
    }
    // effectue la rotation de la tuile à placer
    for(int i = 0; i < emplacement.at(2); i++)
    {
        tuile->rotationHoraire();
    }
    
    // supprime l'ancienne tuile candidate qu'on doit remplacer 
    if(current_plateau->grille[emplacement.at(0)][emplacement.at(1)] != nullptr) 
    {
        Tuile *tuile_a_suppr = current_plateau->grille[emplacement.at(0)][emplacement.at(1)];
        Logging::log(Logging::TRACE, "supprime tuile tmp %d : <%d %d>", tuile_a_suppr, emplacement.at(0), emplacement.at(1));
        current_plateau->tuiles_candidates.erase(tuile_a_suppr);
        //delete tuile_a_suppr; 
        current_plateau->grille[emplacement.at(0)][emplacement.at(1)] = nullptr;
    } 
    else 
    {
        Logging::log(Logging::DEBUG, "supprime pas tuile tmp null");
        Logging::log(Logging::DEBUG, "%d <%d,%d>", current_plateau->grille[emplacement.at(0)][emplacement.at(1)], emplacement.at(0), emplacement.at(1));
    }

    // placement de la tuile (tableau de rotation de tuile)
    current_plateau->grille[emplacement.at(0)][emplacement.at(1)] = tuile;

    // liste des coordonnées des voisins de la tuile 
    std::array<std::pair<int,int>, 4> tuile_voisines = {
        std::make_pair(emplacement.at(0), emplacement.at(1) + 1),
        std::make_pair(emplacement.at(0) + 1, emplacement.at(1)),
        std::make_pair(emplacement.at(0), emplacement.at(1) - 1),
        std::make_pair(emplacement.at(0) - 1, emplacement.at(1))
    };
    
    // les interfaces des tuiles voisines sont décalés 
    int bordure_voisines = 2; 
    int bordure_tuiles = 0;


    // ajout des tuiles candidates sur la grille et connection des bordures
    for(auto tuile_coord_voisine : tuile_voisines)
    {
        Tuile * tuile_voisine = current_plateau->grille[tuile_coord_voisine.first][tuile_coord_voisine.second];

        if(tuile_voisine == nullptr)
        {
            std::array<Bordure *, 4> bordure_tmp = {nullptr, nullptr, nullptr, nullptr};
            std::vector<Element *> element_tmp = {nullptr};
            Tuile * tuile_candidate = new Tuile(-1, -1, bordure_tmp, element_tmp);
            current_plateau->grille[tuile_coord_voisine.first][tuile_coord_voisine.second] = tuile_candidate;
            current_plateau->tuiles_candidates[tuile_candidate] = std::make_pair(tuile_coord_voisine.first,tuile_coord_voisine.second);
        } 
        else 
        {
            if(tuile_voisine->get_id() != -1) 
            {
                if(tuile_voisine->get_id() > 72)
                {
                    Logging::log(Logging::DEBUG, "L'ID de la tuile n'est pas reconnue");
                }
                Logging::log(Logging::TRACE, "set border tuile voisine %d", tuile_voisine->get_id());
                for(int i = 0; i < 3; i++)
                {
                    Bordure * bordure_tuile_voisine_fils = tuile_voisine->getBordure(bordure_voisines)->get_bordure_fils(i);
                    Bordure * bordure_tuile_fils = tuile->getBordure(bordure_tuiles)->get_bordure_fils(2 - i); // inverse le sens depuis le reférencielle de l'autre tuile
                    
                    if(bordure_tuile_voisine_fils->get_type_element() != Noeud::VOID) 
                    {
                        // on enlève le pointeur nullptr des bodures fils des tuiles 
                        bordure_tuile_voisine_fils->remove_tuile_voisin(nullptr);
                        bordure_tuile_fils->remove_tuile_voisin(nullptr); 
                        bordure_tuile_voisine_fils->set_voisin(bordure_tuile_fils);      
                    }
                }
            }
        }

        bordure_tuiles = (1 + bordure_tuiles) % 4;      
        bordure_voisines = (1 + bordure_voisines) % 4;
    }

    current_plateau->nombre_tuiles_type_pioche[tuile->get_id_groupe()]--;
}

/**
 * Retourne la probabilité de tirer une tuile d'un certain type.
 *
 * @param type numéro du type de tuile recherché
 * */
float Plateau::proba_type_tuile(int type)
{
    if(current_plateau->nombre_tuiles_type_pioche[type] == 0)
    {
        return 0;
    }

    return current_plateau->pioche.size()/current_plateau->nombre_tuiles_type_pioche[type];
}

/**
 * Recherche le Joueur qui à obtenue le plus de pions.
 *
 * @param mapJoueurListeMeeple Joueur associé à une liste de Meeple
 * */
int rechercher_max_list_meeple(std::map<Joueur *, std::list<Meeple *>> mapJoueurListeMeeple) 
{
    std::list<Joueur *> list_joueur;
    int max = 0;
    for(auto const & joueur : mapJoueurListeMeeple)
    {
        int tmp = joueur.second.size();
        if(max < tmp) {
            max = tmp;
        }
    }
    return max;
}

/**
 * @titre : Désindexe les Meeples du plateau.
 *
 * @param mapJoueurListeMeeple Joueur associé à une liste de Meeple 
 * */
void Plateau::desindexer_Meeple_dans_la_map(std::map<Joueur*, std::list<Meeple *>> mapJoueurListeMeeple)
{
    Logging::log(Logging::TRACE, "Desindexation des meeples");
    for(auto const & itMap : mapJoueurListeMeeple)
    {
        Joueur * joueur = itMap.first;
        for(auto const &meeple : itMap.second)
        {
            current_plateau->mapJoueursPions.at(joueur)->supprimer_meeple(meeple);
            Logging::log(Logging::TRACE, "Desindexe meeple: %d",meeple);
        }
    }
}

/**
 * @title : Evaluation des Meeples pour chaque Joueurs
 *
 * @description : Cette méthode permet d'à partir de chaque Meeple 
 * des Joueurs d'évaluer s'il y a possibilité d'attribuer des points 
 * aux Joueurs. L'évaluation s'effectue sur tous les Meeples et les 
 * points sont distribuée selon la quantité de Meeple obtenu 
 * (pour l'évalation concurrencielle, ce n'est pas le cas pour les 
 * abbayes par exemple)
 *
 * @param status_du_jeu :  permet de faire une évaluation soit final 
 * soit en cours du jeu
 * */
void Plateau::evaluer_meeple(int status_du_jeu)
{
    for(auto const &itMap : current_plateau->mapJoueursPions)
    {
        Logging::log(Logging::TRACE, "Evaluation pour des meeples du Joueur %d qui a %d meeple posé", itMap.first->get_score(), 7 - itMap.second->get_nbr_meeple());
        const std::array<Meeple *, 7> arrayMeeple_const = itMap.second->get_stack_meeple();
        std::array<Meeple *, 7> arrayMeeple = arrayMeeple_const; // clonnage car il y a mise à jour implicite lors de la desindexation
        
        for(Meeple * itMeeple: arrayMeeple)
        {
            if(itMeeple != nullptr)
            {
                int score = 0;
                std::map<Joueur *, std::list<Meeple *>> mapJoueurListeMeeple; // Associe un joueur et une pile de pions
                Logging::log(Logging::TRACE, "Comptage de points de %d", itMeeple);
                bool est_complet = itMeeple->compter_points(status_du_jeu, & mapJoueurListeMeeple, &score);
                Logging::log(Logging::TRACE, "Score obtenu : %d", score);

                if(est_complet == true || status_du_jeu == STATUS_FINAL)
                {
                    Logging::log(Logging::TRACE, "Evaluation des points");
                    int max_lenght = 0; // valeur max de meeple
                    max_lenght = rechercher_max_list_meeple(mapJoueurListeMeeple);
                    Logging::log(Logging::TRACE, "max meeple: %d", max_lenght);
                    for(auto const &mapitr : mapJoueurListeMeeple)
                    {
                        if((int) mapitr.second.size() == max_lenght)
                        {
                            mapitr.first->add_score(score);
                            Logging::log(Logging::TRACE, "ajout du score %d",score);
                        }
                    }
                    current_plateau->desindexer_Meeple_dans_la_map(mapJoueurListeMeeple);
                    arrayMeeple = itMap.second->get_stack_meeple(); // mise à jour de l'array non mise à jour car const
                }
            }
        }
    }
}

std::vector<std::array<int, 3>> Plateau::get_liste_tuiles_emplacements_libres()
{
    return current_plateau->liste_tuiles_emplacements_libres;
}

bool Plateau::stack_meeple_vide(Joueur * joueur)
{
    return !current_plateau->mapJoueursPions.at(joueur)->si_pion_non_place();
}

/**
 * Permet de poser un meeple
 *
 * @return bool si le meeple peut être posé
 * */
void Plateau::poser_meeple(Joueur * joueur, Element *elem, Meeple * meeple, int indice)
{
    Logging::log(Logging::DEBUG, "Ajout meeple %d dans element", meeple);
    elem->ajouter_meeple(meeple);
    // Logging::log(Logging::DEBUG, "Ajout meeple %d dans pion pour le joueur %d", meeple, joueur->get_type_joueur());
    Pion * pion = current_plateau->mapJoueursPions.at(joueur);
    pion->ajouter_meeple(meeple, indice);
}

void Plateau::poser_meeple(Joueur * joueur, Element *elem, std::pair<int, int> position)
{
    Meeple * meeple = Pion::generate_meeple(joueur, elem, &current_plateau->grille, position);
    Logging::log(Logging::DEBUG, "Ajout meeple %d dans element", meeple);
    elem->ajouter_meeple(meeple);
    Pion * pion = current_plateau->mapJoueursPions.at(joueur);
    pion->ajouter_meeple(meeple, pion->get_premier_indice_libre());
}

void Plateau::ajouter_tuile_pioche(Tuile *tuile)
{
    this->pioche.push_back(tuile);
}

/**
 * @title: Vérifie si il y a un meeple dans une même zone
 *
 * @description: réalise un parcours en profondeur et récolte les meeples.
 *
 * @param: Noeud est le noeud de départ
 * @param: type_element pour ne faire que le parcour si un même type d'éléments 
 *
 * @return si il y a un meeple sur une même zone
 * */
bool Plateau::verifier_si_meeple_voisin(Noeud * noeud, Noeud::type_element type_element) 
{
    std::list<Noeud*> pilenoeud;   // pile pour le parcours des fils
    std::list<Noeud*> noeudmarque; // marque tous les noeuds rencontrés

    bool meeple_est_present = false;

    pilenoeud.push_back(noeud);
    noeudmarque.push_back(noeud);
   
    while(!pilenoeud.empty())
    {
        std::list<Noeud*>::iterator iterNoeud = pilenoeud.begin();

        Noeud * noeudcentrale = *iterNoeud;
        
        Element * element = dynamic_cast<Element *>(noeudcentrale);
        if(element != nullptr)
        {
            if(noeudcentrale != noeud)
            {
                Meeple * meeple = element->get_meeple();
                if(meeple != nullptr) 
                {
                    Logging::log(Logging::DEBUG, "meeple trouvé %d %d", meeple->get_noeud()->get_type_element(), type_element);
                    meeple_est_present = true;
                    break;
                }
            }
            
        } 

        // logging::log(logging::trace, "evaluation d'un noeud %d", noeudcentrale);
        
        pilenoeud.pop_front();

        int i;
    
        for(i = 0; i < noeudcentrale->get_nbr_voisins(); i++)
        {
            Noeud * noeud_fils = noeudcentrale->get_voisin(i);
            // logging::log(logging::trace, "noeud fils %d %d", i, noeud_fils);
            // logging::log(logging::trace, "noeud fils %d est non null", i);
            if(noeud_fils != nullptr)
            {
                if(Noeud::compare_type_element(noeud_fils->get_type_element(), type_element)) // un prés peut pointer vers une ville
                {
                    if(noeudmarque.end() == std::find(noeudmarque.begin(), noeudmarque.end(), noeud_fils))
                    {
                        //logging::log(logging::trace, "noeud fils %d n'est pas marqué", i);
                        pilenoeud.push_back(noeud_fils);
                        noeudmarque.push_back(noeud_fils);
                    } else {
                        //logging::log(logging::trace, "noeud fils %d est déjà marqué", i);
                    }
                }
            }
        }
    }
    return meeple_est_present;
}

int Plateau::get_nbr_meeple(Joueur * joueur) 
{
    Pion *pion = current_plateau->mapJoueursPions[joueur];
    return pion->get_nbr_meeple();
}

void Plateau::calculer_element_libres(Tuile * tuile) {
    std::vector<Element *> list_elements = tuile->getElements();
    current_plateau->element_libre.clear();

    for(auto element : list_elements) {
        if(! Plateau::verifier_si_meeple_voisin(element, element->get_type_element())) 
        {
            current_plateau->element_libre.push_back(element);
        }
    }
}

const std::vector<Element *> Plateau::get_element_libre() 
{
    return current_plateau->element_libre;
}

const std::array<std::array<Tuile *, 144>, 144> * Plateau::get_grille() 
{
    return &current_plateau->grille;
}

Pion * Plateau::get_pion_joueur(Joueur * joueur) 
{
    return current_plateau->mapJoueursPions[joueur];
}

bool Plateau::pioche_est_vide()
{
    if(current_plateau->pioche.empty()) 
    {
        return true;
    }
    return false;
}

/**
 * @title: vider la table d'élément libres
 * 
 * @description: vide seulement la table sans détruitre les objets pointé
 */ 
void Plateau::clear_element_libres() 
{
    current_plateau->element_libre.clear();
}

/**
 * @title: vider la table d'emplacement libres
 * 
 * @description: vide seulement la table sans détruitre les objets pointé
 */ 
void Plateau::clear_emplacement_libres() 
{
    current_plateau->liste_tuiles_emplacements_libres.clear();
}

void Plateau::update_noeud()
{
    for(auto noeud : Plateau::list_noeuds)
    {
        noeud->set_noeud_plateau(&current_plateau->noeuds_plateau);
    }
}

const std::map<Tuile *,std::pair<int,int>> Plateau::get_tuiles_candidates()
{
    return current_plateau->tuiles_candidates;
}