#include <qapplication.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Viewer.h"
#include "Utils.h"
#include "TriangleSoupZipper.h"

using namespace std;

int main(int argc, char **argv) {

    if (argc < 5) {
        cerr << "usage : ./viewer <file.tri> <nbrSubdivisionX> <nbrSubdivisionY> <nbrSubdivisionZ>" << endl;
        return 1;
    }

    string fileName = argv[1];
    cout << fileName << endl;
    ifstream inFile;
    inFile.open(fileName);
    if (!inFile) {
        cerr << "Unable to open file" << endl;
        return 1;
    }
    int nbSubX = atoi(argv[2]);
    int nbSubY = atoi(argv[3]);
    int nbSubZ = atoi(argv[4]);
    if(nbSubX <= 0 || nbSubY <= 0 || nbSubZ <= 0){
        cerr << "Bad parameters : the number of subdivision must be greater than 0" << endl;
        return 1;
    }

    TriangleSoup tsInput;
    tsInput.read(inFile);
    inFile.close();
    cout << "number of triangles : " << tsInput.triangles.size() << endl;
    float prevTriangleCount = tsInput.triangles.size();

    // we compress tsInput into tsZipped
    TriangleSoup tsZipped;
    Index zipIndex(nbSubX, nbSubY, nbSubZ);
    TriangleSoupZipper tsz(tsInput, tsZipped, zipIndex);

    tsz.smartZip();

    cout << "number of triangles after compression : " << tsZipped.triangles.size() << " => triangle ratio of " <<
         tsZipped.triangles.size() / prevTriangleCount * 100 << endl;

    // Read command lines arguments.
    QApplication application(argc, argv);
    // Instantiate the viewer.
    Viewer viewer(&tsZipped);
    viewer.setWindowTitle("Viewer triangle soup");
    // Make the viewer window visible on screen.
    viewer.show();
    // Run main loop.
    application.exec();

    return 0;
}