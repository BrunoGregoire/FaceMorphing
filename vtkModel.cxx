#include "vtkModel.h"

vtkModel::vtkModel()
{
    polyData = vtkSmartPointer<vtkPolyData>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor = vtkSmartPointer<vtkActor>::New();

    visible = false;
}

void vtkModel::Update()
{
    mapper->SetInputData(polyData);
    mapper->Update();

    actor->SetMapper(mapper);
    actor->Modified();
}

void vtkModel::Render()
{
    renderWindow->Render();
}

void vtkModel::SetRenderer(vtkSmartPointer<vtkRenderer> _renderer)
{
    renderer = _renderer;
}

void vtkModel::SetRenderWindow(vtkSmartPointer<vtkRenderWindow> _renderWindow)
{
    renderWindow = _renderWindow;
}

void vtkModel::ShowModel()
{
    if(!visible)
    {
        visible = true;
        renderer->AddActor(actor);
    }
}

void vtkModel::HideModel()
{
    if(visible)
    {
        visible = false;
        renderer->RemoveActor(actor);
    }
}

void vtkModel::ApplyTransMat(vtkSmartPointer<vtkMatrix4x4> transMat)
{
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(transMat);
    transform->Update();

    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInputData(polyData);
    transformFilter->SetTransform(transform);
    transformFilter->Update();

    polyData = transformFilter->GetOutput();
}

void vtkModel::ApplyTransform(vtkSmartPointer<vtkAbstractTransform> transform)
{
    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInputData(polyData);
    transformFilter->SetTransform(transform);
    transformFilter->Update();

    polyData = transformFilter->GetOutput();
}

vtkSmartPointer<vtkPolyData> vtkModel::TransformPolyData(vtkSmartPointer<vtkMatrix4x4> transMat)
{
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(transMat);
    transform->Update();

    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInputData(polyData);
    transformFilter->SetTransform(transform);
    transformFilter->Update();

    return transformFilter->GetOutput();
}
