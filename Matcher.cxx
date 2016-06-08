#include "Matcher.h"

double* Diff(double* vect1, double* vect2)
{
    double* diff = new double[3];
    diff[0] = vect1[0] - vect2[0];
    diff[1] = vect1[1] - vect2[1];
    diff[2] = vect1[2] - vect2[2];
    return diff;
}

void Matcher::MatchPerfectly(vtkMeshModel *firstModel, vtkMeshModel *secondModel)
{
    MatchPerfectly(firstModel,secondModel,-1);
}

void Matcher::MatchPerfectly(vtkMeshModel *firstModel, vtkMeshModel *secondModel, double tolerance)
{
    firstModel->BuildCellLocator();
    secondModel->BuildCellLocator();

    vtkSmartPointer<vtkPoints> oldPoints = vtkSmartPointer<vtkPoints>::New();
    oldPoints->DeepCopy(firstModel->polyData->GetPoints());

    vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();
    newPoints->DeepCopy(firstModel->polyData->GetPoints());

    for(int i=0;i<firstModel->polyData->GetNumberOfPoints();i++)
    {
        double* point = firstModel->polyData->GetPoint(i);
        double newPoint[3];//the coordinates of the closest point will be returned here
        double closestPointDist2; //the squared distance to the closest point will be returned here
        vtkIdType cellId; //the cell id of the cell containing the closest point will be returned here
        int subId; //this is rarely used (in triangle strips only, I believe)
        secondModel->cellLocator->FindClosestPoint(point, newPoint, cellId, subId, closestPointDist2);

        if(tolerance == -1 || vtkMath::Distance2BetweenPoints(newPoint,point)<tolerance)
        {
            newPoints->SetPoint(i,newPoint[0],newPoint[1],newPoint[2]);
        }
        else
            std::cout<<">10"<<std::endl;
    }

    double** deltas = new double*[newPoints->GetNumberOfPoints()];
    for(int i=0;i<newPoints->GetNumberOfPoints();i++)
    {
        deltas[i] = Diff(newPoints->GetPoint(i),oldPoints->GetPoint(i));
    }

    for(int i=0;i<firstModel->blendshapes.size();i++)
    {
        vtkSmartPointer<vtkPoints> bsPoints =  vtkSmartPointer<vtkPoints>::New();
        bsPoints->DeepCopy(firstModel->blendshapes[i]->GetPoints());
        for(int j=0;j<bsPoints->GetNumberOfPoints();j++)
        {
            double* point = bsPoints->GetPoint(j);
            point[0] += deltas[j][0];
            point[1] += deltas[j][1];
            point[2] += deltas[j][2];
            bsPoints->SetPoint(j,point[0],point[1],point[2]);
        }
        firstModel->blendshapes[i]->SetPoints(bsPoints);
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

        for(int j=0;j<3;j++)
        {
            double* point = secondModel->polyData->GetPoint(closestPoints->GetId(j));
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

double Dist(vtkMeshModel *firstModel, vtkMeshModel *secondModel, vtkIdType firstPoint, vtkIdType secondPoint)
{
    // Formula : DIS(P1, P2)= δx|P1−P2|+(1−δ)x|(MC(P1)−MC(P2))|

    double dist = 0;
    double sigma = 0.01;

    dist += sigma*vtkMath::Distance2BetweenPoints(firstModel->polyData->GetPoint(firstPoint),
                                                  secondModel->polyData->GetPoint(secondPoint));

    //std::cout<<"Dist = "<<dist<<std::endl;

    dist += (1-sigma)*abs(firstModel->curvatures->GetTuple(firstPoint)[0] -
                        secondModel->curvatures->GetTuple(secondPoint)[0]);


    //std::cout<<"curvature = "<<abs(firstModel->curvatures->GetTuple(firstPoint)[0] -
    //                            secondModel->curvatures->GetTuple(secondPoint)[0])<<std::endl;

    return dist;
}

void Matcher::MatchWithCurvatures(vtkMeshModel *firstModel, vtkMeshModel *secondModel)
{
    firstModel->ComputeCurvatures();
    secondModel->ComputeCurvatures();

    secondModel->BuildKdTree();

    vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();

    for(int i=0;i<firstModel->polyData->GetNumberOfPoints();i++)
    {
        double newPoint[3];
        newPoint[0] = 0;
        newPoint[1] = 0;
        newPoint[2] = 0;

        vtkSmartPointer<vtkIdList> closestPoints = vtkSmartPointer<vtkIdList>::New();
        secondModel->kdTree->FindClosestNPoints(10,firstModel->polyData->GetPoint(i),closestPoints);

        vtkIdType closestId = closestPoints->GetId(0);
        double* closest = secondModel->polyData->GetPoint(closestId);
        for(int j=1;j<10;j++)
        {
            if(Dist(firstModel,secondModel,i,closestPoints->GetId(j))<Dist(firstModel,secondModel,i,closestId))
            {
                closestId = closestPoints->GetId(j);
                closest = secondModel->polyData->GetPoint(closestId);
            }
        }

        newPoint[0] = closest[0];
        newPoint[1] = closest[1];
        newPoint[2] = closest[2];

        newPoints->InsertNextPoint(newPoint);
    }

    if(firstModel->blendshapes.size()!=0)
    {
        for(int j=0;j<firstModel->blendshapes.size();j++)
        {
            for(int i=0;i<firstModel->polyData->GetNumberOfPoints();i++)
            {
                double* diff = Diff(newPoints->GetPoint(i),firstModel->polyData->GetPoint(i));
                firstModel->blendshapes[j]->GetPoint(i)[0] += diff[0];
                firstModel->blendshapes[j]->GetPoint(i)[1] += diff[1];
                firstModel->blendshapes[j]->GetPoint(i)[2] += diff[2];
            }
        }
    }

    firstModel->polyData->SetPoints(newPoints);
    firstModel->Update();
    firstModel->Render();
}
