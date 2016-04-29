/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * Content.h
 *
 * Contains all the vtk objects used for the project
 *
 */

#ifndef CONTENT_H
#define CONTENT_H

#include "vtkIncludes.h"

#include "vtkModel.h"
#include "vtkMeshModel.h"
#include "vtkGlyphModel.h"
#include "vtkLineModel.h"

class Content
{
public:
    vtkMeshModel* refModel;
    vtkMeshModel* newModel;
    vtkMeshModel* alignedModel;

    int nbKeypoints;
    std::string* areasNames;
    vtkGlyphModel* refKeypoints;
    vtkGlyphModel* newmodelKeypoints;
    vtkGlyphModel* alignedKeypoints;

    vtkLineModel* refAxis;

    vtkSmartPointer<vtkRenderer> refRenderer;
    vtkSmartPointer<vtkRenderer> newmodelRenderer;
    vtkSmartPointer<vtkRenderWindow> refRenderWindow;
    vtkSmartPointer<vtkRenderWindow> newmodelRenderWindow;

    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    vtkSmartPointer<vtkInteractorStyle> style;

    vtkSmartPointer<vtkTextActor> text;

};

#endif
