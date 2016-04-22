#include "vtk2DModel.h"

vtk2DModel::vtk2DModel()
{
    polyData = vtkSmartPointer<vtkPolyData>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
    actor = vtkSmartPointer<vtkActor2D>::New();

    visible = false;
}

void vtk2DModel::Update()
{
    mapper->SetInputData(polyData);
    mapper->Update();

    actor->SetMapper(mapper);
    actor->Modified();
}

void vtk2DModel::Render()
{
    renderWindow->Render();
}

void vtk2DModel::SetRenderer(vtkSmartPointer<vtkRenderer> _renderer)
{
    renderer = _renderer;
}

void vtk2DModel::SetRenderWindow(vtkSmartPointer<vtkRenderWindow> _renderWindow)
{
    renderWindow = _renderWindow;
}

void vtk2DModel::ShowModel()
{
    if(!visible)
    {
        visible = true;
        renderer->AddActor(actor);
    }
}

void vtk2DModel::HideModel()
{
    if(visible)
    {
        visible = false;
        renderer->RemoveActor(actor);
    }
}

void vtk2DModel::Translate(double x, double y, double z)
{
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Translate(x,y,z);
    transform->Update();

    vtkSmartPointer<vtkTransformPolyDataFilter> filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    filter->SetInputData(polyData);
    filter->SetTransform(transform);
    filter->Update();

    polyData->DeepCopy(filter->GetOutput());
}

void vtk2DModel::Scale(double x, double y, double z)
{
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Scale(x,y,z);
    transform->Update();

    vtkSmartPointer<vtkTransformPolyDataFilter> filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    filter->SetInputData(polyData);
    filter->SetTransform(transform);
    filter->Update();

    polyData->DeepCopy(filter->GetOutput());
}
