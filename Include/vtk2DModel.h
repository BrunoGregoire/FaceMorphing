/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * vtk2DModel.h
 *
 * Structure managing a vtk actor 2D
 *
 */

#ifndef VTK2DMODEL_H
#define VTK2DMODEL_H

#include "vtkIncludes.h"

class vtk2DModel
{
public:
    bool visible;

    vtkSmartPointer<vtkPolyData> polyData;
    vtkSmartPointer<vtkPolyDataMapper2D> mapper;
    vtkSmartPointer<vtkActor2D> actor;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> renderWindow;

    vtk2DModel();
    void Update();
    void Render();
    void SetRenderer(vtkSmartPointer<vtkRenderer> _renderer);
    void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> _renderWindow);
    void ShowModel();
    void HideModel();
    void Translate(double x,double y,double z);
    void Scale(double x,double y,double z);

};

#endif
