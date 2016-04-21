/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * vtkGlyphModel.h
 *
 * Structure managing a vtk actor
 * based on a polydata represented by glyphs
 * Each point will appear as a colored square
 *
 */

#ifndef VTKGLYPHMODEL_H
#define VTKGLYPHMODEL_H

#include "vtkIncludes.h"

#include "vtkModel.h"
#include "vtkMeshModel.h"
#include "vtkVectorTextModel.h"

class vtkGlyphModel : public vtkModel
{
public:

    vtkMeshModel* mesh;

    vtkSmartPointer<vtkIdList> pointsIds;
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkVertexGlyphFilter> filter;

    double glyphSize;
    double* glyphColor;

    std::vector<vtkVectorTextModel*> idLabels;

    vtkGlyphModel(vtkMeshModel* _mesh);

    void InsertNextPoint(double* point);
    void InsertNextID(vtkIdType id);
    void InsertID(vtkIdType id,int index);
    void SetPointsIds(vtkSmartPointer<vtkIdList> newIds);
    double* GetPoint(vtkIdType id);
    void UpdatePoints();
    void UpdatePointsIDs();
    void UpdateGlyph();
    void ClearPoints();
    void SetColor(double r,double g,double b);
    int GetNumberOfPoints();
    vtkGlyphModel* Copy();
    int FindClosestPoint(double* point,double tolerance);
    void RemovePointAndID(int index);
    void ExportIDs(std::string txtPath);
    void LoadIDs(std::string txtPath);
    void UpdateIdLabels();
    void HideIdLabels();
};

#endif
