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
    vtkMeshModel* refLeftEyeModel;
    vtkMeshModel* refRightEyeModel;
    vtkMeshModel* refJawUpModel;
    vtkMeshModel* refJawDownModel;

    vtkMeshModel* newModel;
    vtkMeshModel* alignedModel;

    vtkSmartPointer<vtkIdList> customRefKeypoints;
    vtkSmartPointer<vtkIdList> customNewmodelKeypoints;

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

    vtkSmartPointer<vtkRenderWindowInteractor> refInteractor;
    vtkSmartPointer<vtkRenderWindowInteractor> newmodelInteractor;
    vtkSmartPointer<vtkInteractorStyle> style;

    vtkSmartPointer<vtkTextActor> text;

};

#endif
