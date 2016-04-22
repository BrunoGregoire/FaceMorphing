#include "Drawer.h"


vtk2DModel* Drawer::DrawShape(double* origin, double* end, int shapeIndex)
{
    if(shapeIndex == 0 || shapeIndex>4)
        return DrawCircle(origin,end);
    else if(shapeIndex == 1 || shapeIndex == 2 || shapeIndex == 4)
        return DrawLine(origin,end,5);
    else if(shapeIndex==3)
        return DrawLine(origin,end,4);

}

vtk2DModel* Drawer::DrawCircle(double* origin, double* end)
{
    vtk2DModel* newModel = new vtk2DModel();

    double xRadius = abs(end[0]-origin[0])/2;
    double yRadius = abs(end[1]-origin[1])/2;
    double center[2];
    center[0] = (end[0]+origin[0])/2;
    center[1] = (end[1]+origin[1])/2;
    int nbSides = 5*xRadius;

    vtkSmartPointer<vtkRegularPolygonSource> polygonSource = vtkSmartPointer<vtkRegularPolygonSource>::New();
    polygonSource->GeneratePolygonOff();
    polygonSource->SetNumberOfSides(nbSides);
    polygonSource->SetRadius(xRadius);
    polygonSource->SetCenter(0, 0, 0);
    polygonSource->Update();

    newModel->polyData->DeepCopy(polygonSource->GetOutput());

    newModel->Scale(1,yRadius/xRadius,1);
    newModel->Translate(center[0],center[1],0);
    newModel->Update();

    return newModel;
}

vtk2DModel* Drawer::DrawLine(double *origin, double *end, int nbPoints)
{

    double stepX = (end[0]-origin[0])/(nbPoints-1);
    double stepY = (end[1]-origin[1])/(nbPoints-1);

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for(int i=0;i<nbPoints;i++)
    {
        double x = origin[0]+i*stepX;
        double y = origin[1]+i*stepY;
        points->InsertNextPoint(x,y,0);
    }

    vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();
    polyLine->GetPointIds()->SetNumberOfIds(nbPoints);
    for(unsigned int i = 0; i < nbPoints; i++)
        polyLine->GetPointIds()->SetId(i,i);

    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(polyLine);

    vtk2DModel* lineModel = new vtk2DModel();
    lineModel->polyData->SetPoints(points);
    lineModel->polyData->SetLines(cells);
    lineModel->Update();

    return lineModel;
}

void Drawer::SaveArea(vtk2DModel* model, int area)
{
    shapes[area] = model;
}

void Drawer::ProjectPoints(int area, vtkGlyphModel* glyphModel)
{
    switch (area)
    {
        case 0: // FACE
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[0]->polyData;
            glyphModel->mesh->BuildKdTree();

            int offset = polyData->GetNumberOfPoints()/4;
            int step = polyData->GetNumberOfPoints()/34;

            for(int i=0;i<17;i++)
            {
                int index = (16-i)*step + offset;

                double* point = polyData->GetPoint(index);
                vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                picker->Pick(point[0],point[1],0,glyphModel->renderer);
                vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                glyphModel->InsertNextID(id);
            }

            break;
        }
        case 1: // LEFT EYEBROW
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[1]->polyData;
            glyphModel->mesh->BuildKdTree();

            double* origin = polyData->GetPoint(0);
            double originX = origin[0];
            double* end = polyData->GetPoint(4);
            double endX = end[0];

            for(int i=0;i<5;i++)
            {
                int index;
                if(originX<endX)
                    index = i;
                else
                    index = 4-i;

                double* point = polyData->GetPoint(index);
                vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                picker->Pick(point[0],point[1],0,glyphModel->renderer);
                vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                glyphModel->InsertNextID(id);
            }

            break;
        }
        case 2: // RIGHT EYEBROW
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[2]->polyData;
            glyphModel->mesh->BuildKdTree();

            double* origin = polyData->GetPoint(0);
            double originX = origin[0];
            double* end = polyData->GetPoint(4);
            double endX = end[0];

            for(int i=0;i<5;i++)
            {
                int index;
                if(originX<endX)
                    index = i;
                else
                    index = 4-i;

                double* point = polyData->GetPoint(index);
                vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                picker->Pick(point[0],point[1],0,glyphModel->renderer);
                vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                glyphModel->InsertNextID(id);
            }

            break;
        }
        case 3: // NOSE VERTICAL
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[3]->polyData;
            glyphModel->mesh->BuildKdTree();

            double* origin = polyData->GetPoint(0);
            double originY = origin[1];
            double* end = polyData->GetPoint(3);
            double endY = end[1];

            for(int i=0;i<4;i++)
            {
                int index;
                if(originY>endY)
                    index = i;
                else
                    index = 3-i;

                double* point = polyData->GetPoint(index);
                vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                picker->Pick(point[0],point[1],0,glyphModel->renderer);
                vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                glyphModel->InsertNextID(id);
            }

            break;
        }
        case 4: // NOSE HORIZONTAL
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[4]->polyData;
            glyphModel->mesh->BuildKdTree();

            double* origin = polyData->GetPoint(0);
            double originX = origin[0];
            double* end = polyData->GetPoint(4);
            double endX = end[0];

            for(int i=0;i<5;i++)
            {
                int index;
                if(originX<endX)
                    index = i;
                else
                    index = 4-i;

                double* point = polyData->GetPoint(index);
                vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                picker->Pick(point[0],point[1],0,glyphModel->renderer);
                vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                glyphModel->InsertNextID(id);
            }

            break;
        }
        case 5: // RIGHT EYE
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[5]->polyData;
            glyphModel->mesh->BuildKdTree();

            int step = polyData->GetNumberOfPoints()/6;

            for(int i=0;i<6;i++)
            {
                int index;

                if(i==0)
                    index = 5*step;
                else
                    index = step*(i-1);

                double* point = polyData->GetPoint(index);
                vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                picker->Pick(point[0],point[1],0,glyphModel->renderer);
                vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                glyphModel->InsertNextID(id);
            }

            break;
        }
        case 6: // LEFT EYE
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[6]->polyData;
            glyphModel->mesh->BuildKdTree();

            int step = polyData->GetNumberOfPoints()/6;

            for(int i=0;i<6;i++)
            {
                int index;

                if(i==0)
                    index = 5*step;
                else
                    index = step*(i-1);

                double* point = polyData->GetPoint(index);
                vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                picker->Pick(point[0],point[1],0,glyphModel->renderer);
                vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                glyphModel->InsertNextID(id);
            }

            break;
        }
        case 7: // MOUTH
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[7]->polyData;
            glyphModel->mesh->BuildKdTree();

            int step = polyData->GetNumberOfPoints()/12;

            for(int i=0;i<12;i++)
            {
                int index;

                index = step*(i-1);

                double* point = polyData->GetPoint(index);
                vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                picker->Pick(point[0],point[1],0,glyphModel->renderer);
                vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                glyphModel->InsertNextID(id);
            }

        break;
            break;
        }
    }

    glyphModel->UpdateGlyph();
    glyphModel->Update();
    glyphModel->ShowModel();
    glyphModel->Render();
}
