#include <cmath>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Utils.h"

// return the decimal part of a float
float dec(float f) {
    return f - floor(f);
}

Vecteur::Vecteur() : xyz{0, 0, 0} {

}

Vecteur::Vecteur(float x, float y, float z) : xyz{x, y, z} {

}

float Vecteur::operator[](int i) const {
    return xyz[i];
}

float& Vecteur::operator[](int i) {
    return xyz[i];
}

Vecteur Vecteur::inf(const Vecteur& other) const {
    return Vecteur(std::min(xyz[0], other[0]), std::min(xyz[1], other[1]),
                   std::min(xyz[2], other[2]));
}

Vecteur Vecteur::sup(const Vecteur& other) const {
    return Vecteur(std::max(xyz[0], other[0]), std::max(xyz[1], other[1]),
                   std::max(xyz[2], other[2]));
}


Vecteur Vecteur::operator+(const Vecteur& v) const {
    return Vecteur(xyz[0] + v[0], xyz[1] + v[1], xyz[2] + v[2]);
}

Vecteur Vecteur::operator-(const Vecteur& v) const {
    return Vecteur(xyz[0] - v[0], xyz[1] - v[1], xyz[2] - v[2]);
}

Vecteur Vecteur::cross(const Vecteur& v) const {
    float x = xyz[1] * v[2] - xyz[2] * v[1];
    float y = xyz[2] * v[0] - xyz[0] * v[2];
    float z = xyz[0] * v[1] - xyz[1] * v[0];
    return Vecteur(x, y, z);
}

float Vecteur::magnitude() const {
    return std::sqrt((xyz[0] * xyz[0]) + (xyz[1] * xyz[1]) + (xyz[2] * xyz[2]));
}

Vecteur Vecteur::normalize() const {
    float m = magnitude();
    return Vecteur(xyz[0] / m, xyz[1] / m, xyz[2] / m);
}

std::ostream& operator<<(std::ostream& out, Vecteur v) {
    out << v[0] << " " << v[1] << " " << v[2];
    return out;
}

std::istream& operator>>(std::istream& in, Vecteur& v) {
    in >> v[0] >> v[1] >> v[2];
    return in;
}

Triangle::Triangle(Vecteur a, Vecteur b, Vecteur c) : abc{a, b, c} {}

Vecteur Triangle::operator[](int i) const {
    return abc[i];
}

Vecteur& Triangle::operator[](int i) {
    return abc[i];
}

Triangle::Triangle() : abc{Vecteur(), Vecteur(), Vecteur()} {

}

Vecteur Triangle::normal() const {
    Vecteur u = abc[1] - abc[0];
    Vecteur v = abc[2] - abc[0];
    return u.cross(v).normalize();
}

std::ostream& operator<<(std::ostream& out, Triangle t) {
    out << t[0] << " " << t[1] << " " << t[2];
    return out;
}

std::istream& operator>>(std::istream& in, Triangle& t) {
    in >> t[0] >> t[1] >> t[2];
    return in;
}


void TriangleSoup::read(std::istream& in) {
    if (!in.good()) {
        std::cerr << "ERROR" << std::endl;
        return;
    }
    std::string line;
    while (getline(in, line)) {
        if (line[0] == '#')  // on saute les commentaires
            continue;

        std::istringstream ss(line);
        Triangle triangle;
        ss >> triangle;
        triangles.push_back(triangle);
    }
}

// Calcule la boite englobante de la soupe de triangle
void TriangleSoup::boundingBox(Vecteur& low, Vecteur& up) const {
    // init des vecteurs avec les premiers vecteurs de triangle soupe
    Triangle t = triangles.at(0);
    low = t[0];
    up = t[0];

    for (const auto& triangle : triangles) {
        for (const auto vecteur : triangle.abc) {
            low = low.inf(vecteur);
            up = up.sup(vecteur);
        }
    }
}
