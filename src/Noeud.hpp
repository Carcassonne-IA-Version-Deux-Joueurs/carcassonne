// DIRECTIVES
#ifndef NOEUD_HPP
#define NOEUD_HPP

// LIBRAIRIES
#include <vector>
#include <map>
#include <algorithm>

#include "Logging.hpp"

/**
 * @titre: Noeud
 * 
 * @description: Permet de faire l'interface pour le pacours de la couche 'noeud'.
 * La couche 'noeud' est un réseau constitué d'élément de bordure indistingable lors de parcours du réseau.
 * Un noeuds peut être de plusieurs type, comme une route, une ville, une plaine ou bien une abbaye.
 * */
class Noeud
{
    public:
        // VARIABLES
        static enum type_element{VOID, ROUTE, VILLE, VILLE_BLASON, ABBAYE, PLAINE} element_enum;

    private:
        std::map<Noeud*, std::vector<Noeud *>> * noeuds_plateau;  
    
    protected:        
        type_element element;
        int point_en_cours;
        int point_final;
        Noeud();

    public:
         // CONSTRUCTEURS
        // DESTRUCTEUR
        virtual ~Noeud();

        // METHODES
        void set_lien(Noeud * noeud);   // ajoute un lien simple
        void set_voisin(Noeud * noeud); // ajoute un lien voisin
        Noeud * get_voisin(int index);  // retourne le voisin front
        
        // std::map<Noeud*, std::vector<Noeud *>> * get_noeud_plateau();
        void set_noeud_plateau(std::map<Noeud*, std::vector<Noeud *>> *);

        virtual int get_points(int status_du_jeu);
        virtual void supprimer_meeple();
        void remove_tuile_voisin(Noeud * noeud); // supprime en fonction de l'addresse du noeuds donné en param
        bool static compare_type_element(Noeud::type_element type_element1, Noeud::type_element type_element2); // compare deux types éléments sans prendre en compte des options (ville est égale à ville_blason)
        bool has_nullptr();
        int get_nbr_voisins();
        Noeud::type_element get_type_element();
        std::map<Noeud*, std::vector<Noeud *>> * get_noeud_plateau();
        void set_type_element(Noeud::type_element type);
        Noeud * noeud_fils(Noeud * noeud_pere, int indexe);
        /*
        void test(Plateau * p1)
        {
            p1->init_plateau();
        };
        */
        std::vector<Noeud *> * get_noeud_voisins();
};

#endif
