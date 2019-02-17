#ifndef TP1_TRIANGLESOUPZIPPER_H
#define TP1_TRIANGLESOUPZIPPER_H

#include <map>
#include "Utils.h"

/// Définit un index sur 3 entiers. Toutes les opérations usuelles
/// sont surchargées (accès, comparaisons, égalité).
struct Index {
    int idx[3];

    Index() = default;

    Index(int i0, int i1, int i2);

    explicit Index(int indices[]);

    int operator[](int i) const;

    int& operator[](int i);

    bool operator<(const Index& other) const;

    bool operator==(const Index& other) const;

    bool operator!=(const Index& other) const;
};

std::ostream& operator<<(std::ostream& out, Index i);


// Structure pour calculer le barycentre d'un ensemble de points.
struct CellData {
    Vecteur acc;
    int nb;

    CellData();

    explicit CellData(Vecteur accInit);

    // Ajoute le point v à l'accumulateur.
    void add(const Vecteur& v);

    // Retourne le barycentre de tous les points ajoutés.
    Vecteur barycenter() const;
};


struct TriangleSoupZipper {

    TriangleSoup tsIn;
    TriangleSoup& tsOut;
    Index idx;

    // Stocke pour chaque cellule son barycentre.
    std::map<Index, CellData> index2data;

    Vecteur cellSize;
    Vecteur low;
    Vecteur up;

    // Construit le zipper avec une soupe de triangle en entrée
    // anInput, une soupe de triangle en sortie anOutput, et un index size
    // qui est le nombre de cellules de la boîte découpée selon les 3 directions.
    TriangleSoupZipper(const TriangleSoup& anInput, TriangleSoup& anOuput, Index size);

    /// @return l'index de la cellule dans laquelle tombe p.
    Index index(const Vecteur& p) const;

    /// @return le centroïde de la cellule d'index idx (son "centre").
    Vecteur centroid(const Index& idx) const;

    void smartZip();

    void zip();

    void zipVoxel();

    void addNodeToIndex2Data(const Vecteur& v, const Index& idx);
};


#endif //TP1_TRIANGLESOUPZIPPER_H
