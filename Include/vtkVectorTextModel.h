/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * vtkVectorTextModel.h
 *
 * Structure managing a vtk actor
 * containing a 3D text
 *
 */

#ifndef VTKVECTORTEXTMODEL_H
#define VTKVECTORTEXTMODEL_H

#include "vtkIncludes.h"

#include "vtkModel.h"

class vtkVectorTextModel : public vtkModel
{
public:

    vtkSmartPointer<vtkVectorText> vectorText;

    vtkVectorTextModel(std::string text);
};

#endif
