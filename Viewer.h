#include <vector>
#include <QGLViewer/qglviewer.h>
#include "Utils.h"

class Viewer : public QGLViewer {
public:
    bool hasTaste;
    const TriangleSoup* ptrSoup;

    //Viewer() : QGLViewer() {}
    Viewer(const TriangleSoup *aSoup = nullptr, bool hasTaste = true)
            : QGLViewer(), hasTaste(hasTaste), ptrSoup(aSoup) {}

protected :
    /// Called at each draw of the window
    virtual void draw();

    /// Called before the first draw
    virtual void init();

    /// Called when pressing help.
    virtual QString helpString() const;

    void beautifullDraw() const;
    void regularDraw() const;
};