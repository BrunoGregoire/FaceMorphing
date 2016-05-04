#include "Cutter.h"

void Cutter::InitPlane(vtkMeshModel* toCut)
{
    mesh = toCut;

    planeModel = new vtkMeshModel();
    planeModel->ReadFromOBJ("Resources/plane.obj");

    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Scale(100, 1, 100);
    transform->Translate(-1, 0, -1);

    planeModel->ApplyTransform(transform);

    planeModel->Update();

    planeModel->SetRenderer(mesh->renderer);
    planeModel->SetRenderWindow(mesh->renderWindow);

    planeModel->ShowModel();
}

void Cutter::TranslatePlane(double x, double y, double z)
{
    planeModel->actor->SetPosition(planeModel->actor->GetPosition()[0]+x,
            planeModel->actor->GetPosition()[1]+y,
            planeModel->actor->GetPosition()[2]+z);
    planeModel->Render();
}

void Cutter::Cut(vtkMeshModel* ref)
{
    vtkSmartPointer<vtkPolyData> fullNewModelPolydata = vtkSmartPointer<vtkPolyData>::New();
    fullNewModelPolydata->DeepCopy(mesh->polyData);

    vtkSmartPointer<vtkClipPolyData> clip = vtkSmartPointer<vtkClipPolyData>::New();
    clip->SetValue(0);
    clip->GenerateClippedOutputOn();
    clip->SetInputData(fullNewModelPolydata);

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0,planeModel->actor->GetPosition()[1],0);
    plane->SetNormal(0, 1, 0);
    clip->SetClipFunction (plane);
    clip->Update();

    mesh->polyData->DeepCopy(clip->GetOutput());
    mesh->ScaleWith(ref);
    mesh->Update();
    mesh->BuildKdTree();

    plane->SetNormal(0, -1, 0);
    clip->SetClipFunction(plane);
    clip->Update();

    body = vtkSmartPointer<vtkPolyData>::New();
    body->DeepCopy(clip->GetOutput());
}

