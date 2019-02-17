#include <fstream>
#include <cmath>
#include "Viewer.h"
#include "Utils.h"

using namespace std;

void Viewer::draw() {
    (hasTaste ? beautifullDraw() : regularDraw());

    string trianglesNbr = "number of triangles : " + to_string(ptrSoup->triangles.size());
    float whiteDiff[4] = {1.0, 1.0, 1.0, 1.0};
    glColor4fv(whiteDiff);
    drawText(20, 20, trianglesNbr.c_str());
}

void Viewer::beautifullDraw() const {
    float colorBronzeDiff[4] = {0.8, 0.6, 0.0, 1.0};

    float colorBronzeSpec[4] = {1.0, 1.0, 0.4, 1.0};

    // Draws triangles given by 3 vertices.
    glBegin(GL_TRIANGLES);
    glEnable(GL_COLOR_MATERIAL);  // le materiau peut changer à chaque triangle.
    // Si vous les écrivez là, ces couleurs/réglages seront partagés par tous les triangles.
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBronzeDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorBronzeSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);

    for (const auto& triangle : ptrSoup->triangles) {
        float triangleColor[4] = {dec(triangle[0][0]), dec(triangle[0][1]), dec(triangle[0][2]), 1.0f};
        glColor4fv(triangleColor);

        Vecteur n = triangle.normal();
        glNormal3f(n[0], n[1], n[2]);
        for (int i = 0; i < 3; i++) {
            Vecteur v = triangle[i];
            glVertex3f(v[0], v[1], v[2]);
        }
    }
    glDisable(GL_COLOR_MATERIAL);
    glEnd();
}

void Viewer::regularDraw() const {
    float colorPinkDiff[4] = {0.75, 0.22, 0.42, 1.0};
    float colorPinkSpec[4] = {1.0, 1.0, 0.4, 1.0};

    // Draws triangles given by 3 vertices.
    glBegin(GL_TRIANGLES);
    // Si vous les écrivez là, ces couleurs/réglages seront partagés par tous les triangles.
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorPinkDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorPinkSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);

    for (const auto& triangle : ptrSoup->triangles) {
        glColor4fv(colorPinkDiff);
        Vecteur n = triangle.normal();
        glNormal3f(n[0], n[1], n[2]);
        for (int i = 0; i < 3; i++) {
            Vecteur v = triangle[i];
            glVertex3f(v[0], v[1], v[2]);
        }
    }
    glDisable(GL_COLOR_MATERIAL);
    glEnd();

}

void Viewer::init() {
    Vecteur low;
    Vecteur up;
    ptrSoup->boundingBox(low, up);
    camera()->setSceneBoundingBox(qglviewer::Vec(low[0], low[1], low[2]), qglviewer::Vec(up[0], up[1], up[2]));
    camera()->showEntireScene();
    // Opens help window
    help();
}

QString Viewer::helpString() const {
    QString text("<h2>S i m p l e V i e w e r</h2>");
    text += "Use the mouse to move the camera around the object. ";
    text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
    text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
    text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
    text += "Simply press the function key again to restore it. Several keyFrames define a ";
    text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
    text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
    text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
    text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
    text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
    text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
    text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
    text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
    text += "Press <b>Escape</b> to exit the viewer.";
    return text;
}
