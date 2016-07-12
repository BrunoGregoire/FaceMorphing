/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * vtkModel.h
 *
 * Structure managing a vtk actor
 * based on a polydata
 * (cf vtkGlyphModel, vtkLineModel, vtkMeshModel)
 *
 */

#ifndef VTKMODEL_H
#define VTKMODEL_H

#include "vtkIncludes.h"

class vtkModel
{
public:
    bool visible;

    vtkSmartPointer<vtkPolyData> polyData;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> renderWindow;

    std::vector<vtkModel*> linkedMeshes;

    vtkModel();
    void Update();
    void Render();
    void SetRenderer(vtkSmartPointer<vtkRenderer> _renderer);
    void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> _renderWindow);
    void ShowModel();
    void HideModel();
    void ApplyTransMat(vtkSmartPointer<vtkMatrix4x4> transMat);
    void ApplyTransform(vtkSmartPointer<vtkAbstractTransform> transform);
    vtkSmartPointer<vtkPolyData> TransformPolyData(vtkSmartPointer<vtkMatrix4x4> transMat);
};

#endif
