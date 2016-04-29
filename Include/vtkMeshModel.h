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
    vtkSmartPointer<vtkTexture> texture;
    vtkSmartPointer<vtkKdTreePointLocator> kdTree;

    vtkLineModel* axis;
    vtkSmartPointer<vtkMatrix4x4> axisTransMat;
    vtkSmartPointer<vtkTransform> axisTransform;

    vtkSmartPointer<vtkOBBTree> obbTree;

    std::vector<std::string> bsNames;
    std::vector<vtkSmartPointer<vtkPolyData>> blendshapes;

    vtkMeshModel();

    void ReadFromOBJ(std::string fileName);
    void ExportAsOBJ(std::string fileName);
    void ReadTexture(std::string fileName);
    void Center();
    void Texture(vtkSmartPointer<vtkTexture> _texture);
    void ScaleWith(vtkMeshModel* otherMesh);
    void BuildKdTree();
    void BuildObbTree();
    vtkMeshModel* Copy();
    void ComputeAxisTransform();
    double* TransformAlongAxis(double* point);
    void LoadBlendshapes(std::string folderPath);
    void TransformBlendshapes(vtkSmartPointer<vtkAbstractTransform> transform);
    void ExportBlendshapes(std::string folderPath);
    void TextureBlendshapes();
};

#endif
