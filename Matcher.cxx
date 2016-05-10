#include "Matcher.h"

void Matcher::MatchPerfecty(vtkMeshModel *firstModel, vtkMeshModel *secondModel)
{
    int* prefered = new int[firstModel->polyData->GetNumberOfPoints()];

    secondModel->BuildKdTree();

    for(int i=0;i<firstModel->polyData->GetNumberOfPoints();i++)
    {
        prefered[i] = secondModel->kdTree->FindClosestPoint(firstModel->polyData->GetPoint(i));
    }

    vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();
    for(int i=0;i<firstModel->polyData->GetNumberOfPoints();i++)
    {
        newPoints->InsertNextPoint(secondModel->polyData->GetPoint(prefered[i]));
    }

    firstModel->polyData->SetPoints(newPoints);
    firstModel->Update();
    firstModel->Render();
}

void Matcher::MatchInterpolated(vtkMeshModel *firstModel, vtkMeshModel *secondModel)
{
    secondModel->BuildKdTree();


    vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();

    for(int i=0;i<firstModel->polyData->GetNumberOfPoints();i++)
    {
        double newPoint[3];
        newPoint[0] = 0;
        newPoint[1] = 0;
        newPoint[2] = 0;

        vtkSmartPointer<vtkIdList> closestPoints = vtkSmartPointer<vtkIdList>::New();
        secondModel->kdTree->FindClosestNPoints(3,firstModel->polyData->GetPoint(i),closestPoints);

        for(int i=0;i<3;i++)
        {
            double* point = secondModel->polyData->GetPoint(closestPoints->GetId(i));
            newPoint[0] += point[0];
            newPoint[1] += point[1];
            newPoint[2] += point[2];
        }

        newPoint[0] = newPoint[0]/3;
        newPoint[1] = newPoint[1]/3;
        newPoint[2] = newPoint[2]/3;

        newPoints->InsertNextPoint(newPoint);
    }

    firstModel->polyData->SetPoints(newPoints);
    firstModel->Update();
    firstModel->Render();

}
