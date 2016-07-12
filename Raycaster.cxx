#include "Raycaster.h"

void Raycaster::TopRaycast(vtkGlyphModel *firstKeypoints, vtkGlyphModel *secondKeypoints, int precision)
{
    firstIntersecPoints = vtkSmartPointer<vtkPoints>::New();
    firstIntersecCellIds = vtkSmartPointer<vtkIdList>::New();
    secondIntersecPoints = vtkSmartPointer<vtkPoints>::New();
    secondIntersecCellIds = vtkSmartPointer<vtkIdList>::New();

    firstKeypoints->mesh->BuildObbTree();
    secondKeypoints->mesh->BuildObbTree();

    double* origin = new double[3];
    double* end = new double[3];

    for(int j=0;j<(8*precision);j++)
    {
        double angle_x = 3.1416*(((double)j)/(8*precision));

        for(int i=0;i<(8*precision);i++)
        {
            double angle_y = 3.1416*(((double)i)/(8*precision));

            origin[0] = 300*cos(angle_x)*sin(angle_y);
            origin[1] = 300*sin(angle_x)*sin(angle_y)+80;
            origin[2] = 300*cos(angle_y)-50;

            end[0] = 0;
            end[1] = 80;
            end[2] = -50;

            if(firstKeypoints->mesh->axis != NULL)
            {
                origin = firstKeypoints->mesh->TransformAlongAxis(origin);
                end = firstKeypoints->mesh->TransformAlongAxis(end);
            }
            else if(secondKeypoints->mesh->axis != NULL)
            {
                origin = secondKeypoints->mesh->TransformAlongAxis(origin);
                end = secondKeypoints->mesh->TransformAlongAxis(end);
            }

            Cast(origin,end,firstKeypoints,secondKeypoints);
        }
    }
}

void Raycaster::CylindricalRaycast(vtkGlyphModel *firstKeypoints, vtkGlyphModel *secondKeypoints, int precision)
{
    firstIntersecPoints = vtkSmartPointer<vtkPoints>::New();
    firstIntersecCellIds = vtkSmartPointer<vtkIdList>::New();
    secondIntersecPoints = vtkSmartPointer<vtkPoints>::New();
    secondIntersecCellIds = vtkSmartPointer<vtkIdList>::New();

    firstKeypoints->mesh->BuildObbTree();
    secondKeypoints->mesh->BuildObbTree();

    int randOffset = rand()%10;

    double* origin = new double[3];
    double* end = new double[3];

    for(int j=0;j<(16*precision);j++)
    {
        double angle = (2*3.1416)*(((double)j)/(16*precision));
        double x = 300*cos(angle);
        double z = 300*sin(angle);

        for(int y=-150;y<80;y+=20/precision)
        {
            origin[0]=x;
            origin[1]=y+randOffset;
            origin[2]=z-50;

            end[0]=0;
            end[1]=y+randOffset;
            end[2]=-50;

            if(firstKeypoints->mesh->axis != NULL)
            {
                origin = firstKeypoints->mesh->TransformAlongAxis(origin);
                end = firstKeypoints->mesh->TransformAlongAxis(end);
            }
            else if(secondKeypoints->mesh->axis != NULL)
            {
                origin = secondKeypoints->mesh->TransformAlongAxis(origin);
                end = secondKeypoints->mesh->TransformAlongAxis(end);
            }

            Cast(origin,end,firstKeypoints,secondKeypoints);
        }
    }

}

void Raycaster::Cast(double *origin, double *end, vtkGlyphModel *firstKeypoints, vtkGlyphModel *secondKeypoints)
{
    firstKeypoints->mesh->obbTree->IntersectWithLine(origin,end,firstIntersecPoints,firstIntersecCellIds);
    secondKeypoints->mesh->obbTree->IntersectWithLine(origin,end,secondIntersecPoints,secondIntersecCellIds);

    if(firstIntersecPoints->GetNumberOfPoints()>0 && secondIntersecPoints->GetNumberOfPoints()>0)
    {
        firstKeypoints->InsertNextPoint(firstIntersecPoints->GetPoint(0));
        secondKeypoints->InsertNextPoint(secondIntersecPoints->GetPoint(0));
    }


    // Draw ray
    /*
    vtkLineModel* ray = new vtkLineModel(origin,end);
    ray->UpdateLine();
    ray->SetRenderer(firstKeypoints->renderer);
    ray->SetRenderWindow(firstKeypoints->renderWindow);
    ray->Update();
    ray->ShowModel();
    ray->Render();
    */

}

