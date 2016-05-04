/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * Drawer.h
 *
 * Draw polygons on the screen.
 *
 */

#ifndef DRAWER_H
#define DRAWER_H

#include <stdexcept>

#include "vtkIncludes.h"

#include "vtk2DModel.h"
#include "vtkGlyphModel.h"
#include "KeypointsManager.h"

class Drawer
{
public:
    int nbShapes = 8;
    vtk2DModel* shapes[8];

    vtk2DModel* DrawShape(double* origin, double* end, int shapeIndex);
    vtk2DModel* DrawCircle(double* origin, double* end);
    vtk2DModel* DrawLine(double* origin, double* end, int nbPoints);
    void SaveArea(vtk2DModel* model,int area);
    void ProjectPoints(int area,vtkGlyphModel* glyphModel);
    void ProjectSymetrics(std::vector<int> points, std::vector<double*> positions, vtkGlyphModel* glyphModel);
    void ClearShapes();

};

#endif
