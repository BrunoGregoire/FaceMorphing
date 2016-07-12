/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * vtkMeshModel.h
 *
 * Structure managing a vtk actor
 * based on a polydata represented by a surface (mesh)
 * that can be textured
 *
 */

#ifndef VTKMESHMODEL_H
#define VTKMESHMODEL_H

#include "vtkIncludes.h"

#include "vtkModel.h"
#include "vtkLineModel.h"

class vtkMeshModel : public vtkModel
{
public:
    std::string name;

    vtkSmartPointer<vtkTexture> texture;

    vtkSmartPointer<vtkKdTreePointLocator> kdTree;
    vtkSmartPointer<vtkOBBTree> obbTree;
    vtkSmartPointer<vtkCellLocator> cellLocator;

    vtkLineModel* axis;
    vtkSmartPointer<vtkMatrix4x4> axisTransMat;
    vtkSmartPointer<vtkTransform> axisTransform;

    std::vector<std::string> bsNames;
    std::vector<vtkSmartPointer<vtkPolyData>> blendshapes;

    vtkSmartPointer<vtkDataArray> curvatures;

    vtkMeshModel();

    void ReadFromOBJ(std::string fileName);
    void ExportAsOBJ(std::string fileName);
    void ReadTexture(std::string fileName);
    void Center();
    void Texture(vtkSmartPointer<vtkTexture> _texture);
    void ScaleWith(vtkMeshModel* otherMesh);
    void BuildKdTree();
    void BuildObbTree();
    void BuildCellLocator();
    vtkMeshModel* Copy();
    void ComputeAxisTransform();
    double* TransformAlongAxis(double* point);
    void LoadBlendshapes(std::string folderPath);
    void ApplyTransMat(vtkSmartPointer<vtkMatrix4x4> transMat);
    void ApplyTransform(vtkSmartPointer<vtkAbstractTransform> transform);
    void TransformBlendshapes(vtkSmartPointer<vtkAbstractTransform> transform);
    void ExportBlendshapes(std::string folderPath);
    void TextureBlendshapes();
    void AddLink(vtkMeshModel* mesh);
    void Triangulate();
    void Smooth();
    void ComputeNormals();
    void Reorganize();
    void ComputeCurvatures();
};

#endif
