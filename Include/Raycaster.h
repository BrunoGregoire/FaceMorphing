/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * Raycaster.h
 *
 * Cast rays through models to find
 * corresponding points between them.
 *
 */

#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "vtkIncludes.h"

#include "vtkModel.h"
#include "vtkMeshModel.h"
#include "vtkGlyphModel.h"
#include "vtkLineModel.h"

class Raycaster
{
public:

    vtkSmartPointer<vtkPoints> firstIntersecPoints;
    vtkSmartPointer<vtkIdList> firstIntersecCellIds;
    vtkSmartPointer<vtkPoints> secondIntersecPoints;
    vtkSmartPointer<vtkIdList> secondIntersecCellIds;

    void TopRaycast(vtkGlyphModel* firstKeypoints, vtkGlyphModel* secondKeypoints, int precision);
    void CylindricalRaycast(vtkGlyphModel* firstKeypoints, vtkGlyphModel* secondKeypoints, int precision);
    void Cast(double *origin, double *end, vtkGlyphModel *firstKeypoints, vtkGlyphModel *secondKeypoints);
};

#endif
