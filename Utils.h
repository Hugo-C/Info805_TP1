#ifndef TP1_UTILS_H
#define TP1_UTILS_H

#include <ostream>
#include <istream>
#include <vector>

float dec(float f);

struct Vecteur {
    float xyz[3];  // les composantes
    Vecteur();  // constructeur
    Vecteur(float x, float y, float z);  // constructeur
    Vecteur operator+(const Vecteur& v) const;

    Vecteur operator-(const Vecteur& v) const;

    Vecteur cross(const Vecteur& v) const;

    float magnitude() const;

    Vecteur normalize() const;

    // Retourne le vecteur dont les composantes sont les minima des
    // composantes de soi-même et de other.
    Vecteur inf(const Vecteur& other) const;

    // Retourne le vecteur dont les composantes sont les maxima des
    // composantes de soi-même et de other.
    Vecteur sup(const Vecteur& other) const;

    float operator[](int i) const;       // accesseur en lecture
    float& operator[](int i);            // accesseur en ecriture
};

std::ostream& operator<<(std::ostream& out, Vecteur v);

std::istream& operator>>(std::istream& in, Vecteur& v);

struct Triangle {
    Vecteur abc[3];  // les composantes
    Triangle();  // constructeur
    Triangle(Vecteur a, Vecteur b, Vecteur c);  // constructeur
    Vecteur normal() const;

    Vecteur operator[](int i) const;              // accesseur en lecture
    Vecteur& operator[](int i);                   // accesseur en ecriture
};

std::ostream& operator<<(std::ostream& out, Triangle t);

std::istream& operator>>(std::istream& in, Triangle& t);

struct TriangleSoup {
    std::vector<Triangle> triangles; // les triangles
    TriangleSoup() = default;

    void read(std::istream& in);

    void boundingBox(Vecteur& low, Vecteur& up) const;
};

#endif //TP1_UTILS_H
