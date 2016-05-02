/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * Cutter.h
 *
 * This class is used to cut the head of the new model.
 * The head with then be used for the morphing.
 * At the end we will stick the new morphed head to the body.
 *
 */

#ifndef CUTTER_H
#define CUTTER_H

#include "vtkIncludes.h"

#include "vtkMeshModel.h"

class Cutter 
{
public:

    vtkMeshModel* mesh;
    vtkMeshModel* planeModel;

    vtkSmartPointer<vtkPolyData> body;

    void InitPlane(vtkMeshModel* toCut);
    void TranslatePlane(double x,double y, double z);
    void Cut(vtkMeshModel* ref);

};

#endif
