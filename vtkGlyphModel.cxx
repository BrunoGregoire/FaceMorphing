#include "vtkGlyphModel.h"

vtkGlyphModel::vtkGlyphModel(vtkMeshModel *_mesh)
{
    polyData = vtkSmartPointer<vtkPolyData>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor = vtkSmartPointer<vtkActor>::New();

    pointsIds = vtkSmartPointer<vtkIdList>::New();
    points = vtkSmartPointer<vtkPoints>::New();
    filter = vtkSmartPointer<vtkVertexGlyphFilter>::New();

    glyphSize = 10;
    glyphColor = new double[3];
    glyphColor[0] = 0;
    glyphColor[1] = 0;
    glyphColor[2] = 1;

    mesh = _mesh;

    SetRenderer(mesh->renderer);
    SetRenderWindow(mesh->renderWindow);

    visible = false;
}

void vtkGlyphModel::InsertNextPoint(double *point)
{
    points->InsertNextPoint(point);
}

void vtkGlyphModel::InsertNextID(vtkIdType id)
{
    pointsIds->InsertNextId(id);
    double* point = new double[3];
    mesh->polyData->GetPoint(id,point);
    InsertNextPoint(point);
}

void vtkGlyphModel::InsertID(vtkIdType id, int index)
{
    vtkSmartPointer<vtkIdList> newpointsIds = vtkSmartPointer<vtkIdList>::New();
    vtkSmartPointer<vtkPoints> newpoints = vtkSmartPointer<vtkPoints>::New();

    for(int i=0;i<index;i++)
    {
        newpoints->InsertNextPoint(points->GetPoint(i));
        newpointsIds->InsertNextId(pointsIds->GetId(i));
    }

    newpointsIds->InsertNextId(id);
    newpoints->InsertNextPoint(mesh->polyData->GetPoint(id));

    for(int i=index;i<points->GetNumberOfPoints();i++)
    {
        newpoints->InsertNextPoint(points->GetPoint(i));
        newpointsIds->InsertNextId(pointsIds->GetId(i));
    }

    points = newpoints;
    pointsIds = newpointsIds;
    UpdateGlyph();
}

void vtkGlyphModel::SetPointsIds(vtkSmartPointer<vtkIdList> newIds)
{
    points = vtkSmartPointer<vtkPoints>::New();
    pointsIds = vtkSmartPointer<vtkIdList>::New();

    for(int i=0;i<newIds->GetNumberOfIds();i++)
        InsertNextID(newIds->GetId(i));

}

double* vtkGlyphModel::GetPoint(vtkIdType id)
{
    double* point = new double[3];
    mesh->polyData->GetPoint(id,point);
    return point;
}

void vtkGlyphModel::UpdatePoints()
{
    points = vtkSmartPointer<vtkPoints>::New();

    vtkSmartPointer<vtkIdList> idsCopy = vtkSmartPointer<vtkIdList>::New();
    idsCopy->DeepCopy(pointsIds);

    pointsIds = vtkSmartPointer<vtkIdList>::New();

    for(int i=0;i<idsCopy->GetNumberOfIds();i++)
        InsertNextID(idsCopy->GetId(i));
}

void vtkGlyphModel::UpdatePointsIDs()
{
    pointsIds = vtkSmartPointer<vtkIdList>::New();

    mesh->BuildKdTree();

    for (int i=0;i<points->GetNumberOfPoints();i++)
    {
        vtkIdType id = mesh->kdTree->FindClosestPoint(points->GetPoint(i));
        pointsIds->InsertNextId(id);
    }
}

void vtkGlyphModel::UpdateGlyph()
{
    polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);

    filter->SetInputData(polyData);
    filter->Update();

    polyData = filter->GetOutput();
    Update();

    actor->GetProperty()->SetPointSize(glyphSize);
    actor->GetProperty()->SetColor(glyphColor);
}

void vtkGlyphModel::ClearPoints()
{
    pointsIds = vtkSmartPointer<vtkIdList>::New();
    points = vtkSmartPointer<vtkPoints>::New();
}

void vtkGlyphModel::SetColor(double r, double g, double b)
{
    glyphColor[0] = r;
    glyphColor[1] = g;
    glyphColor[2] = b;
}

int vtkGlyphModel::GetNumberOfPoints()
{
    return points->GetNumberOfPoints();
}

vtkGlyphModel* vtkGlyphModel::Copy()
{
    vtkGlyphModel* copy = new vtkGlyphModel(mesh);
    copy->points->DeepCopy(points);
    copy->pointsIds->DeepCopy(pointsIds);
    copy->UpdateGlyph();
    copy->glyphSize = glyphSize;
    copy->SetColor(glyphColor[0],glyphColor[1],glyphColor[2]);

    return copy;
}

int vtkGlyphModel::FindClosestPoint(double *point,double tolerance)
{
    double minDist = 9999999;
    int index;
    for(int i=0;i<points->GetNumberOfPoints();i++)
    {
        if(vtkMath::Distance2BetweenPoints(points->GetPoint(i),point)<minDist)
        {
            minDist = vtkMath::Distance2BetweenPoints(points->GetPoint(i),point);
            index = i;
        }
    }

    if(minDist<tolerance)
        return index;
    else
        return -1;
}

void vtkGlyphModel::RemovePointAndID(int index)
{
    idLabels[index]->HideModel();

    vtkSmartPointer<vtkIdList> newpointsIds = vtkSmartPointer<vtkIdList>::New();
    vtkSmartPointer<vtkPoints> newpoints = vtkSmartPointer<vtkPoints>::New();

    for(int i=0;i<points->GetNumberOfPoints();i++)
    {
        if(i!=index)
        {
            newpoints->InsertNextPoint(points->GetPoint(i));
            newpointsIds->InsertNextId(pointsIds->GetId(i));
        }
    }

    points = newpoints;
    pointsIds = newpointsIds;
    UpdateGlyph();
}

void vtkGlyphModel::ExportIDs(std::string txtPath)
{
    ofstream myfile (txtPath.c_str());
    if (myfile.is_open())
    {
      for(int i=0;i<pointsIds->GetNumberOfIds();i++)
      {
          myfile <<pointsIds->GetId(i)<<"\n";
      }
      myfile.close();
    }
    else cout << "Unable to open file";
}

void vtkGlyphModel::LoadIDs(std::string txtPath)
{
    std::string line;
    ifstream myfile (txtPath);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            InsertNextID(std::stoi(line));
        }
        myfile.close();
    }
    else cout << "Unable to open file";

    UpdateGlyph();

}

void vtkGlyphModel::UpdateIdLabels()
{
    HideIdLabels();
    idLabels.clear();
    for(int i=0;i<points->GetNumberOfPoints();i++)
    {
        std::stringstream ss;
        ss << i;
        std::string str = ss.str();
        vtkVectorTextModel* label = new vtkVectorTextModel(str);
        label->Update();
        label->SetRenderer(renderer);
        label->SetRenderWindow(renderWindow);
        label->ShowModel();
        label->actor->SetPosition(points->GetPoint(i)[0],points->GetPoint(i)[1],points->GetPoint(i)[2]+10);
        label->actor->SetScale(3,3,3);
        idLabels.push_back(label);
    }
}

void vtkGlyphModel::HideIdLabels()
{
    for(int i=0;i<idLabels.size();i++)
    {
        idLabels[i]->HideModel();
    }
}
