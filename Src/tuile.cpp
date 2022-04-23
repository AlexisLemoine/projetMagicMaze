#include "tuile.hpp"
#include <iostream>

#include <random>
#include <functional>
#include <chrono>


tuile::tuile(int n):Union_find(n){
    pad.ajouter_tuile(0, 0);
    tuile_CoorX.push_back(0);
    tuile_CoorY.push_back(0);
    classique=false;
    obj = false;
    sort = false;
    caseObjectif = -1;
    caseSortie = -1;
    // 0 : aucune, 1 : jaune, 2 : violet, 3 : orange, 4 : vert
    aucune = jaune = orange = violet = vert = false;
    nbObjectif = nbSortie = 0;

    for (int j = 0; j< 4; j++){
        objectif[j+1] = false;
        sortie[j+1] = false;
        objectifNumCase[j+1] = -1;
        sortieNum[j+1] = -1;
    }

    joueurCase[1] = 5;
    joueurCase[2] = 6;
    joueurCase[3] = 9;
    joueurCase[4] = 10;

    for (int j = 1; j < 5; j++){
        pad.placer_joueur(0, 0, Case(joueurCase[j]), (conversionCouleur(j)));
    }

    for (int i = 0; i < 16; i++){
            tabCases[i].couleurP = -1;
            tabCases[i].existeP = false;
    }

    tabCases[13].existeP = true;
    tabCases[2].existeP = true;
    tabCases[4].existeP = true;
    tabCases[11].existeP = true;
    std::cout<<"tuile créée. "<< std::endl;
    int nbP=4;
    quellesCouleurs(nbP);
    associerCouleurs(nbP);

    for (int k = 0; k < 5; k++){
        murs.push_back(k);
    }
    for (int k = 7; k < 17; k++){
        murs.push_back(k);
    }
    for (int k = 19; k < 24; k++){
        murs.push_back(k);
    }


    quelMur();
    ajouterFonctionPad(0, 0);

    ajouterAuGraphe();
    G.creerGraphe();

    murs.clear();
}

void tuile::tuileSuivante(int x, int y){
    pad.ajouter_tuile(x, y);
    tuile_CoorX.push_back(x);
    tuile_CoorY.push_back(y);
    classique = true;
    obj = false;
    sort = false;
    caseObjectif = -1;
    caseSortie = -1;
    couleurObj = -1;
    couleurSort = -1;
    // 0 : aucune, 1 : jaune, 2 : violet, 3 : orange, 4 : vert
    aucune = jaune = orange = violet = vert = false;



    for (int i = 0; i < 16; i++){
        tabCases[i].couleurP = -1;
        tabCases[i].existeP = false;
    }

    tabCases[13].existeP = true;
    tabCases[13].couleurP = 0;
    std::cout<<"tuile créée. "<< std::endl;

    // ici, on stocke le nombre de portes dans nbP, puis on le réutilise dans quellesCouleurs
    int nbP = quellePorte();
    quellesCouleurs(nbP);
    associerCouleurs(nbP);
    if (nbObjectif != 4){
        objectifP(x, y);
    }
    if (nbSortie != 4){
        sortieP(x, y);
    }

    for (int k = 0; k < 24; k++){
        murs.push_back(k);
    }

    quelMur();
    ajouterFonctionPad(x, y);

    ajouterAuGraphe();
    G.creerGraphe();

    murs.clear();

}

void tuile::quelMur(){
    unsigned int l;
    if (classique){
        l = 24;
    }
    else{
        l = 20;
        }

    Union_find toutesCases(16);

    if (!classique){
        toutesCases.fusion(5,6);
        toutesCases.fusion(6,9);
        toutesCases.fusion(9,10);
    }
    /*
    for (unsigned int j = 0; j < l; j++){
        std::cout<< murs[j] << " ";
    }*/
    for(unsigned int i = 0; i < l; i++) {
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> distr(0, l-1);
        int index = distr(eng);
        //unsigned int index = rand() % (l) ;
        int tmp = murs[l - i - 1] ;
        murs[l - i - 1] = murs[index] ;
        murs[index] = tmp ;
    }
    /*
    for (unsigned int j = 0; j < l; j++){
        std::cout<< murs[j] << " ";
    }*/
    while (!toutRelies(toutesCases)){
        Mur m(murs[0]);
        // std::cout << m[0].index() << " " << m[1].index() << " ";
        murs.erase(murs.begin());
        if(!recherche(m[0].index(), m[1].index())){
            toutesCases.fusion(m[0].index(), m[1].index());
        }
    }

}

bool tuile::toutRelies(Union_find C){
    int c = 13;
    if (tabCases[2].existeP && !(C.recherche(2, c))){
        return false;
    }
    if (tabCases[4].existeP && !(C.recherche(4, c))){
        return false;
    }
    if (tabCases[11].existeP && !(C.recherche(11, c))){
        return false;
    }
    if (obj && !(C.recherche(caseObjectif, c))){
        return false;
    }
    if (sort && !(C.recherche(caseSortie, c))){
        return false;
    }
    return true;
}

void tuile::objectifP(int x, int y){
    //if (pile_ou_face() && pile_ou_face()){
        nbObjectif++;
        obj = true;
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> distr(1, 4);
        int n = distr(eng);
        while (objectif[n] == true){
            n = distr(eng);
        }
        objectif[n] = true;
        couleurObj = n;

        std::uniform_int_distribution<int> distr2(0, 15);

        int m = distr2(eng);

        while (tabCases[m].existeP == true){
            m = distr2(eng);
        }

        objectifNumCase[n] = m;
        caseObjectif = m;
        pad.ajouter_objectif(x, y, Case(m), conversionCouleur(n));
    //}
}

void tuile::sortieP(int x, int y){
    //if (pile_ou_face() && pile_ou_face()){
        nbSortie++;
        sort = true;
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> distr(1, 4);
        int n = distr(eng);
        while (sortie[n] == true){
            n = distr(eng);
        }
        sortie[n] = true;
        couleurSort = n;
        std::uniform_int_distribution<int> distr2(0, 15);

        int m = distr2(eng);

        while ((tabCases[m].existeP == true)||(m == objectifNumCase[n])){
            m = distr2(eng);
        }

        sortieNum[n] = m;
        caseSortie = m;
        pad.ajouter_sortie(x, y, Case(m), conversionCouleur(n));
    //}
}

Couleur tuile::conversionCouleur(int n){
    switch (n){
        case 0 :{return Couleur::AUCUNE;}
        case 1 :{return Couleur::JAUNE;}
        case 2 :{return Couleur::ORANGE;}
        case 3 :{return Couleur::VIOLET;}
        case 4 :{return Couleur::VERT;}
        default :{ std::cout<< "erreur lors de la conversion de couleur" << std::endl;
                return Couleur::AUCUNE;}
    }
}

void tuile::ajouterFonctionPad(int x, int y){
    for (int i =0; i < 16; i++){
        if (tabCases[i].existeP){
            pad.ajouter_porte(x, y, Case(i), (conversionCouleur(tabCases[i].couleurP)));
        }
    }

    int l = murs.size();
    for (int j = 0; j < l; j++){
        Mur m(murs[j]);
        pad.ajouter_mur(x, y, m);
    }

}

void tuile::dessiner(){

    std::cout << pad << std::endl ;

}


int tuile::quellePorte(){
    bool porte[3];
    for (int j = 0; j < 3; j++){
        porte[j] = false;
    }
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(1, 3);
    int n = distr(eng); // compteur du nombre de portes qu'on ajoute (entre 1 et 3)
    int nbP = n;
    int i = 0; // compteur qui tourne sur toutes les portes
    while (n != 0){
        if (porte[i] == false && pile_ou_face()){
            porte[i] = true;
            n--;
        }
        if (i == 2){
            i = 0;
        }
        else{
            i++;
        }
    }
    if (porte[0] == true){
        tabCases[2].existeP = true;
    }
    if (porte[1] == true){
        tabCases[4].existeP = true;
    }
    if (porte[2] == true){
        tabCases[11].existeP = true;
    }
    return nbP;
}

void tuile::quellesCouleurs(int nbPortes){
    //bool tabCouleurs[5];
    if (classique){

        aucune = true;
        //tabCouleurs[1] = tabCouleurs[2] = tabCouleurs[3] = tabCouleurs[4] = false;

        while (nbPortes != 0){
            std::random_device rd;
            std::default_random_engine eng(rd());
            std::uniform_int_distribution<int> distr(1, 4);
            int C = distr(eng);
            if (C == 1 && !jaune){
                jaune = true;
                nbPortes--;
            }
            if (C == 2 && !orange){
                orange = true;
                nbPortes--;
            }
            if (C == 3 && !violet){
                violet = true;
                nbPortes--;
            }
            if (C == 4 && !vert){
                vert = true;
                nbPortes--;
            }
        }
    }
    else {
        jaune = orange = violet = vert = true;
    }
}

void tuile::associerCouleurs(int nbP){
    int tab[4];
    int i = 0;
    // on récupère les couleurs
    if (jaune){
        tab[i] = 1;
        i++;
    }
    if (orange){
        tab[i] = 2;
        i++;
    }
    if (violet){
        tab[i] = 3;
        i++;
    }
    if (vert){
        tab[i] = 4;
        i++;
    }

    i = 0;
    int tabC[4];
    // On mélange le tableau de couleurs 
    while(i < nbP){
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> distr(0, nbP-1);
        int tmp = distr(eng);
        if (tab[tmp] != -1){
            tabC[i] = tab[tmp];
            i++;
            tab[tmp] = -1;
        }
    }

    i = 0;

    // On associe chaque couleurs à chaque porte, dans l'ordre donnée croissant avec les coulerus dans le désordre
    if (tabCases[2].existeP){
        tabCases[2].couleurP = tabC[i];
        i++;
    }
    if (tabCases[4].existeP){
        tabCases[4].couleurP = tabC[i];

        i++;
    }
    if (tabCases[11].existeP){
        tabCases[11].couleurP = tabC[i];        
        i++;
    }
    if (tabCases[13].existeP && classique == false){
        tabCases[13].couleurP = tabC[i];
        i++;
    }
}

void tuile::ajouterAuGraphe(){
    int l = tuile_CoorX.size();
    G.setCoord(tuile_CoorX[l-1], tuile_CoorY[l-1]);
    G.setMur(murs);

    if (tabCases[2].existeP){
        G.setPortes2(tabCases[2].couleurP);
    }
    if (tabCases[4].existeP){
        G.setPortes4(tabCases[4].couleurP);
    }
    if (tabCases[11].existeP){
        G.setPortes11(tabCases[11].couleurP);
    }

    if (obj){
        G.setObj(caseObjectif, couleurObj);
    }
    if (sort){
        G.setSort(caseSortie, couleurSort);
    }
}

graphe tuile::getGraphe(){
    return G;
}

PadPlateau tuile::sauver(){
    return pad;
}

bool tuile::pile_ou_face() {
  //lancer la piece
  return m_piece(m_random) ;
}