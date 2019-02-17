#include <cmath>
#include <iostream>
#include <cassert>
#include "TriangleSoupZipper.h"

Index::Index(int i0, int i1, int i2) : idx{i0, i1, i2} {

}

Index::Index(int *indices) : idx{indices[0], indices[1], indices[2]} {

}

int Index::operator[](int i) const {
    return idx[i];
}

int& Index::operator[](int i) {
    return idx[i];
}

bool Index::operator<(const Index& other) const {
    return (idx[0] < other.idx[0])
           || ((idx[0] == other.idx[0])
               && ((idx[1] < other.idx[1])
                   || ((idx[1] == other.idx[1])
                       && (idx[2] < other.idx[2]))));
}

bool Index::operator==(const Index& other) const {
    return (idx[0] == other.idx[0])
           && ((idx[1] == other.idx[1])
               && (idx[2] == other.idx[2]));
}

bool Index::operator!=(const Index& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, Index i) {
    out << i.idx[0] << " " << i.idx[1] << " " << i.idx[2];
    return out;
}


void CellData::add(const Vecteur& v) {
    assert  (nb != 0);
    acc = v + acc;
    nb++;
}

Vecteur CellData::barycenter() const {
    if (nb == 0) {
        std::cout << "barycenter ERROR, no vector supplied" << std::endl;
        return Vecteur();
    }
    return Vecteur(acc[0] / nb, acc[1] / nb, acc[2] / nb);
}

CellData::CellData(Vecteur accInit) : acc(accInit), nb(1) {}

CellData::CellData() : acc(), nb(0) {}


TriangleSoupZipper::TriangleSoupZipper(const TriangleSoup& anInput, TriangleSoup& anOuput, Index size) :
        tsIn(anInput), tsOut(anOuput), idx(size) {
    tsIn.boundingBox(low, up);
    Vecteur v = up - low;
    cellSize[0] = v[0] / idx[0];
    cellSize[1] = v[1] / idx[1];
    cellSize[2] = v[2] / idx[2];
}

Index TriangleSoupZipper::index(const Vecteur& p) const {
    Vecteur distance = p - low;
    int nbx = static_cast<int>(distance[0] / cellSize[0]);
    int nby = static_cast<int>(distance[1] / cellSize[1]);
    int nbz = static_cast<int>(distance[2] / cellSize[2]);
    return Index(nbx, nby, nbz);
}

Vecteur TriangleSoupZipper::centroid(const Index& idx) const {
    float x = (idx[0] + 0.5f) * cellSize[0] + low[0];
    float y = (idx[1] + 0.5f) * cellSize[1] + low[1];
    float z = (idx[2] + 0.5f) * cellSize[2] + low[2];
    return Vecteur(x, y, z);
}

void TriangleSoupZipper::zip() {
    for (const auto& triangle : tsIn.triangles) {
        Index idx0 = index(triangle[0]);
        Index idx1 = index(triangle[1]);
        Index idx2 = index(triangle[2]);

        if (idx0 == idx1 || idx1 == idx2) {
            continue;  // on ne garde pas le triangle en sortie
        }

        // on ajoute le triangle et on se souvient de sa position pour le calcul du barycentre
        addNodeToIndex2Data(triangle[0], idx0);
        addNodeToIndex2Data(triangle[1], idx1);
        addNodeToIndex2Data(triangle[2], idx2);
        tsOut.triangles.emplace_back(triangle[0], triangle[1], triangle[2]);
    }
}

// Ajoute un sommet à index2Data, permettant le calcul du barycentre de l'index concerné par la suite
void TriangleSoupZipper::addNodeToIndex2Data(const Vecteur& v, const Index& idx) {
    auto it = index2data.find(idx);
    if (it == index2data.end()) {
        CellData cellData(v);
        index2data.insert(std::make_pair(idx, cellData));
    } else {
        it->second.add(v);
    }
}

// Réalise la compression d'une soupe de triangle en placant les sommet au milieu des cellules de l'index
void TriangleSoupZipper::zipVoxel() {
    for (const auto& triangle : tsIn.triangles) {
        Index idx0 = index(triangle[0]);
        Index idx1 = index(triangle[1]);
        Index idx2 = index(triangle[2]);
        if (idx0 != idx1 && idx1 != idx2) {
            tsOut.triangles.emplace_back(centroid(idx0), centroid(idx1), centroid(idx2));
        }
    }
}

void TriangleSoupZipper::smartZip() {
    index2data.clear();
    zip();
    for (auto& triangle : tsOut.triangles) {
        Index idx0 = index(triangle[0]);
        Index idx1 = index(triangle[1]);
        Index idx2 = index(triangle[2]);

        triangle[0] = index2data[idx0].barycenter();
        triangle[1] = index2data[idx1].barycenter();
        triangle[2] = index2data[idx2].barycenter();
    }
}
