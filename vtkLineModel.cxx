#include "vtkLineModel.h"

vtkLineModel::vtkLineModel(double *_origin, double *_end)
{
    polyData = vtkSmartPointer<vtkPolyData>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor = vtkSmartPointer<vtkActor>::New();

    origin = _origin;
    end = _end;

    visible = false;
}

void vtkLineModel::UpdateLine()
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints(2);
    points->Allocate(2);

    points->InsertPoint(0, origin);
    points->InsertPoint(1, end);

    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    cells->Initialize();

    vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
    line->GetPointIds()->SetId(0, 0);
    line->GetPointIds()->SetId(1, 1);

    cells->InsertNextCell(line);

    polyData = vtkPolyData::New();
    polyData->Initialize();
    polyData->SetPoints(points);
    polyData->SetLines(cells);

    Update();
}
