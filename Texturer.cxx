#include "Texturer.h"

void Texturer::Texture(vtkMeshModel *source, vtkMeshModel *target)
{
    source->BuildKdTree();

    vtkSmartPointer<vtkPoints> targetPoints = vtkSmartPointer<vtkPoints>::New();
    targetPoints = target->polyData->GetPoints();

    vtkDataArray* sourceTCoords = source->polyData->GetPointData()->GetTCoords();

    vtkSmartPointer<vtkFloatArray> targetTCoords = vtkSmartPointer<vtkFloatArray>::New();
    targetTCoords->SetNumberOfComponents(2);
    targetTCoords->SetName("TextureCoordinates");

    for (int i = 0; i < targetPoints->GetNumberOfPoints(); i++)
    {
        double p[3];
        targetPoints->GetPoint(i, p);
        vtkIdType id = source->kdTree->FindClosestPoint(p);

        float coord[2];
        coord[0] = sourceTCoords->GetTuple(id)[0];
        coord[1] = sourceTCoords->GetTuple(id)[1];

        targetTCoords->InsertNextTuple(coord);
    }

    target->polyData->GetPointData()->SetTCoords(targetTCoords);
    target->Texture(source->texture);
}
