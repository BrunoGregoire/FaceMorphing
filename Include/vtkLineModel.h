/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * vtkLineModel.h
 *
 * Structure managing a vtk actor
 * based on a polydata represented by a line
 * defined by two points
 *
 */

#ifndef VTKLINEMODEL_H
#define VTKLINEMODEL_H

#include "vtkIncludes.h"

#include "vtkModel.h"

class vtkLineModel : public vtkModel
{
public:
    double* origin;
    double* end;

    vtkLineModel(double* _origin,double* _end);

    void UpdateLine();
};

#endif
