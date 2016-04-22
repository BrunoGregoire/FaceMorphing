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

#include "vtkIncludes.h"

#include "vtk2DModel.h"
#include "vtkGlyphModel.h"


class Drawer
{
public:
    vtk2DModel* shapes[8];

    vtk2DModel* DrawShape(double* origin, double* end, int shapeIndex);
    vtk2DModel* DrawCircle(double* origin, double* end);
    vtk2DModel* DrawLine(double* origin, double* end, int nbPoints);
    void SaveArea(vtk2DModel* model,int area);
    void ProjectPoints(int area,vtkGlyphModel* glyphModel);
    void ClearShapes();

};

#endif
