#include <qapplication.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Viewer.h"
#include "Utils.h"

using namespace std;

int main(int argc, char **argv) {

    if (argc < 2) {
        cerr << "usage : ./visualisuer <file.tri>" << endl;
        return 1;
    }

    string fileName = argv[1];
    ifstream inFile;
    inFile.open(fileName);
    if (!inFile) {
        cerr << "Unable to open file" << endl;
        return 1;
    }

    TriangleSoup triangleSoup;
    triangleSoup.read(inFile);
    inFile.close();
    cout << "number of triangles : " << triangleSoup.triangles.size() << endl;

    // Read command lines arguments.
    QApplication application(argc, argv);
    // Instantiate the viewer.
    Viewer viewer(&triangleSoup);
    viewer.setWindowTitle("Viewer triangle soup");
    // Make the viewer window visible on screen.
    viewer.show();
    // Run main loop.
    application.exec();

    return 0;
}