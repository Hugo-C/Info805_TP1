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

    if (argc < 6) {
        cerr << "usage : ./compression <fileInput.tri> <fileOutput.tri> <nbrSubdivisionX> <nbrSubdivisionY> <nbrSubdivisionZ>" << endl;
        return 1;
    }

    string fileInputName = argv[1];
    ifstream inFile;
    inFile.open(fileInputName);
    if (!inFile) {
        cerr << "Unable to open file" << endl;
        return 1;
    }
    int nbSubX = atoi(argv[3]);
    int nbSubY = atoi(argv[4]);
    int nbSubZ = atoi(argv[5]);
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

    cout << "number of triangles after compression : " << tsZipped.triangles.size() << " => compression ratio of " <<
         tsZipped.triangles.size() / prevTriangleCount * 100 << endl;

    string fileOutputName = argv[2];
    ofstream outFile;
    outFile.open(fileOutputName);
    tsZipped.write(outFile);

    return 0;
}