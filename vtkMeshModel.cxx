#include "vtkMeshModel.h"

vtkMeshModel::vtkMeshModel()
{
    polyData = vtkSmartPointer<vtkPolyData>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor = vtkSmartPointer<vtkActor>::New();

    visible = false;
}

void vtkMeshModel::ReadFromOBJ(std::string fileName)
{
    vtkSmartPointer<vtkOBJReader> objReader = vtkSmartPointer<vtkOBJReader>::New();
    objReader->SetFileName(fileName.c_str());
    objReader->Update();

    polyData = objReader->GetOutput();

    Update();
}

void vtkMeshModel::ReadTexture(std::string fileName)
{
    // Import jpg
    vtkSmartPointer<vtkJPEGReader> jpgReader = vtkSmartPointer<vtkJPEGReader>::New();
    jpgReader->SetFileName(fileName.c_str());
    jpgReader->Update();

    // Apply texture
    texture = vtkSmartPointer<vtkTexture>::New();
    texture->SetInputData(jpgReader->GetOutput());
    texture->InterpolateOn();
    texture->Update();

    // Apply texture
    actor->SetTexture(texture);
}

void vtkMeshModel::Texture(vtkSmartPointer<vtkTexture> _texture)
{
    texture = vtkSmartPointer<vtkTexture>::New();
    texture->SetInputData(_texture->GetInput());
    texture->InterpolateOn();
    texture->Update();

    actor->SetTexture(texture);
}

void vtkMeshModel::ScaleWith(vtkMeshModel *otherMesh)
{
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    double scale = otherMesh->polyData->GetBounds()[1]/polyData->GetBounds()[1];
    transform->Scale(scale, scale, scale);

    vtkSmartPointer<vtkTransformFilter> filter = vtkSmartPointer<vtkTransformFilter>::New();
    filter->SetInputData(polyData);
    filter->SetTransform(transform);
    filter->Update();

    polyData = filter->GetPolyDataOutput();
}

void vtkMeshModel::BuildKdTree()
{
    kdTree = vtkSmartPointer<vtkKdTreePointLocator>::New();
    kdTree->SetDataSet(polyData);
    kdTree->BuildLocator();
}

void vtkMeshModel::BuildObbTree()
{
    obbTree = vtkSmartPointer<vtkOBBTree>::New();
    obbTree->SetDataSet(polyData);
    obbTree->BuildLocator();
}

vtkMeshModel* vtkMeshModel::Copy()
{
    vtkMeshModel* copy = new vtkMeshModel();
    copy->polyData->DeepCopy(polyData);
    copy->Update();
    copy->SetRenderer(renderer);
    copy->SetRenderWindow(renderWindow);

    return copy;
}

void vtkMeshModel::ComputeAxisTransform()
{
    axisTransform = vtkSmartPointer<vtkTransform>::New();
    axisTransform->SetMatrix(axisTransMat);
    axisTransform->Update();
}

double* vtkMeshModel::TransformAlongAxis(double *point)
{
    double x = point[0];
    double y = point[1];
    double z = point[2];

    double* transformedPoint = new double[3];
    transformedPoint[0] = axisTransform->TransformPoint(x,y,z)[0];
    transformedPoint[1] = axisTransform->TransformPoint(x,y,z)[1];
    transformedPoint[2] = axisTransform->TransformPoint(x,y,z)[2];
    return transformedPoint;
}
