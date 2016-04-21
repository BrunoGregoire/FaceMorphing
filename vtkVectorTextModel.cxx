#include "vtkVectorTextModel.h"

vtkVectorTextModel::vtkVectorTextModel(std::string text)
{
    // Create text
    vectorText = vtkSmartPointer<vtkVectorText>::New();
    vectorText->SetText(text.c_str());
    vectorText->Update();

    polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData = vectorText->GetOutput();

    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor = vtkSmartPointer<vtkActor>::New();
}
