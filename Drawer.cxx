#include "Drawer.h"


vtk2DModel* Drawer::DrawShape(double* origin, double* end, int shapeIndex)
{
    if(shapeIndex == 0 || shapeIndex>3)
        return DrawCircle(origin,end);
    else if(shapeIndex == 1 || shapeIndex == 3)
        return DrawLine(origin,end,5);
    else if(shapeIndex==2)
        return DrawLine(origin,end,4);
    return NULL;
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
    if(area<0 || area>=nbShapes)
        throw std::runtime_error("area out of range in Drawer::SaveArea");

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
            int step = polyData->GetNumberOfPoints()/32;

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
        case 1: // RIGHT EYEBROW
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[1]->polyData;
            glyphModel->mesh->BuildKdTree();

            std::vector<int> points;
            std::vector<double*> positions;

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

                points.push_back(17+i);
                double* pos = new double[3];
                pos[0] = picker->GetPickPosition()[0];
                pos[1] = picker->GetPickPosition()[1];
                pos[2] = picker->GetPickPosition()[2];
                positions.push_back(pos);
            }

            ProjectSymetrics(points,positions,glyphModel);

            break;
        }
        case 2: // NOSE VERTICAL
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[2]->polyData;
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
        case 3: // NOSE HORIZONTAL
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[3]->polyData;
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
        case 4: // RIGHT EYE
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[4]->polyData;
            glyphModel->mesh->BuildKdTree();

            std::vector<int> points;
            std::vector<double*> positions;

            int step = polyData->GetNumberOfPoints()/6;

            for(int i=0;i<6;i++)
            {
                int index;

                if(i<2)
                    index = step*(5+i)-step/2;
                else
                    index = step*(i-1)-step/2;

                double* point = polyData->GetPoint(index);
                vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                picker->Pick(point[0],point[1],0,glyphModel->renderer);
                vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                glyphModel->InsertNextID(id);

                points.push_back(36+i);
                double* pos = new double[3];
                pos[0] = picker->GetPickPosition()[0];
                pos[1] = picker->GetPickPosition()[1];
                pos[2] = picker->GetPickPosition()[2];
                positions.push_back(pos);
            }

            ProjectSymetrics(points,positions,glyphModel);

            break;
        }
        case 5: // MOUTH
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[5]->polyData;
            glyphModel->mesh->BuildKdTree();

            int step = polyData->GetNumberOfPoints()/12;

            for(int i=0;i<12;i++)
            {
                int index;

                if(i<3)
                    index = step*(9+i);
                else
                    index = step*(i-3);

                double* point = polyData->GetPoint(index);
                vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                picker->Pick(point[0],point[1],0,glyphModel->renderer);
                vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                glyphModel->InsertNextID(id);
            }

            break;
        }
        case 6: // Right ear
        {
            vtkSmartPointer<vtkPolyData> polyData = shapes[6]->polyData;
            glyphModel->mesh->BuildKdTree();

            std::vector<int> points;
            std::vector<double*> positions;

            int step = polyData->GetNumberOfPoints()/4;

            for(int i=0;i<4;i++)
            {
                int index;
                index = step*i;

                if(i!=1)
                {
                    double* point = polyData->GetPoint(index);
                    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
                    picker->Pick(point[0],point[1],0,glyphModel->renderer);
                    vtkIdType id = glyphModel->mesh->kdTree->FindClosestPoint(picker->GetPickPosition());
                    glyphModel->InsertNextID(id);

                    if(i==0)
                        points.push_back(60);
                    else
                        points.push_back(59+i);
                    double* pos = new double[3];
                    pos[0] = picker->GetPickPosition()[0];
                    pos[1] = picker->GetPickPosition()[1];
                    pos[2] = picker->GetPickPosition()[2];
                    positions.push_back(pos);
                }

            }

            ProjectSymetrics(points,positions,glyphModel);

            break;
        }
    }

    glyphModel->UpdateGlyph();
    glyphModel->Update();
    glyphModel->ShowModel();
    glyphModel->Render();
}

void Drawer::ProjectSymetrics(std::vector<int> points,std::vector<double*> positions, vtkGlyphModel *glyphModel)
{
    std::vector<int> syms;
    for(int i=0;i<points.size();i++)
    {
        syms.push_back(KeypointsManager::GetSymmetric(points[i]));
        std::cout<<syms[0]<<std::endl;
    }

    if(syms[0]<syms[1])
    {
        for(int i=0;i<syms.size();i++)
        {
            double* point = positions[i];
            double symPoint[3];
            symPoint[0] = -point[0];
            symPoint[1] = point[1];
            symPoint[2] = point[2];
            vtkIdType symId = glyphModel->mesh->kdTree->FindClosestPoint(symPoint);
            glyphModel->InsertNextID(symId);
        }
    }
    else
    {
        for(int i=syms.size()-1;i>=0;i--)
        {
            double* point = positions[i];
            double symPoint[3];
            symPoint[0] = -point[0];
            symPoint[1] = point[1];
            symPoint[2] = point[2];
            vtkIdType symId = glyphModel->mesh->kdTree->FindClosestPoint(symPoint);
            glyphModel->InsertNextID(symId);
        }
    }

}
