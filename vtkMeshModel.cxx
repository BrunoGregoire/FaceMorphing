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

void vtkMeshModel::ExportAsOBJ(std::string fileName)
{
    if(fileName.substr(fileName.size()-4,4)==".obj")
        fileName = fileName.substr(0,fileName.size()-4);
    else
        std::cout<<fileName.substr(fileName.size()-5,4)<<std::endl;

    vtkSmartPointer<vtkOBJExporter> objExporter = vtkOBJExporter::New();

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // Add the actors to the scene
    renderer->AddActor(actor);
    renderWindow->Render();

    objExporter->SetRenderWindow(renderWindow);
    objExporter->SetFilePrefix(fileName.c_str());
    objExporter->Write();
    renderWindow->Finalize();
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

void vtkMeshModel::LoadBlendshapes(std::string folderPath)
{
    blendshapes.clear();
    for(int i=0;i<bsNames.size();i++)
    {
        vtkSmartPointer<vtkOBJReader> objReader = vtkSmartPointer<vtkOBJReader>::New();
        std::string fileName = folderPath + bsNames[i]+".obj";
        objReader->SetFileName(fileName.c_str());
        objReader->Update();

        blendshapes.push_back(objReader->GetOutput());
    }
}

void vtkMeshModel::TransformBlendshapes(vtkSmartPointer<vtkAbstractTransform> transform)
{
    for(int i =0;i<blendshapes.size();i++)
    {
        vtkSmartPointer<vtkTransformPolyDataFilter> filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        filter->SetInputData(blendshapes[i]);
        filter->SetTransform(transform);
        filter->Update();

        blendshapes[i]->DeepCopy(filter->GetOutput());
    }
}

void vtkMeshModel::ExportBlendshapes(std::string folderPath)
{
    for(int i=0;i<blendshapes.size();i++)
    {
        std::string fileName = folderPath + bsNames[i];

        vtkSmartPointer<vtkOBJExporter> objExporter = vtkOBJExporter::New();

        vtkSmartPointer<vtkPolyDataMapper> bsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        bsMapper->SetInputData(blendshapes[i]);
        bsMapper->Update();

        vtkSmartPointer<vtkActor> bsActor = vtkSmartPointer<vtkActor>::New();
        bsActor->SetMapper(bsMapper);

        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);

        renderer->AddActor(bsActor);
        renderWindow->Render();

        objExporter->SetRenderWindow(renderWindow);
        objExporter->SetFilePrefix(fileName.c_str());
        objExporter->Write();

        renderWindow->Finalize();
    }
}

void vtkMeshModel::TextureBlendshapes()
{
    vtkDataArray* tCoords = polyData->GetPointData()->GetTCoords();

    for(int i=0;i<blendshapes.size();i++)
    {
        vtkSmartPointer<vtkFloatArray> newTCoords = vtkSmartPointer<vtkFloatArray>::New();
        newTCoords->DeepCopy(tCoords);

        blendshapes[i]->GetPointData()->SetTCoords(newTCoords);
    }
}
