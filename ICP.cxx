#include "ICP.h"

ICP::ICP()
{
    icp = vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
}

vtkSmartPointer<vtkMatrix4x4> ICP::ComputeTransMat(vtkGlyphModel* sourceKeypoints,vtkGlyphModel* targetKeypoints, int mode)
{
    icp = vtkSmartPointer<vtkIterativeClosestPointTransform>::New();

    vtkSmartPointer<vtkPolyData> source = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();

    vtkSmartPointer<vtkVertexGlyphFilter> sourceFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vtkSmartPointer<vtkVertexGlyphFilter> targetFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();

    source->SetPoints(sourceKeypoints->points);
    target->SetPoints(targetKeypoints->points);

    if(source->GetNumberOfPoints() != target->GetNumberOfPoints())
        std::cout<<"ERROR ICP : "<<source->GetNumberOfPoints()<<" source points but "<<target->GetNumberOfPoints()<<" target points"<<std::endl;

    sourceFilter->SetInputData(source);
    sourceFilter->Update();
    source = sourceFilter->GetOutput();

    targetFilter->SetInputData(target);
    targetFilter->Update();
    target = targetFilter->GetOutput();

    icp->SetSource(source);
    icp->SetTarget(target);
    if(mode == 0)
        icp->GetLandmarkTransform()->SetModeToSimilarity();
    else
        icp->GetLandmarkTransform()->SetModeToAffine();
    icp->SetMaximumNumberOfIterations(50);
    icp->StartByMatchingCentroidsOn();
    icp->Modified();
    icp->Update();
    return icp->GetMatrix();
}

void ICP::Align(vtkGlyphModel *sourceKeypoints, vtkGlyphModel *targetKeypoints, int mode)
{
    transMat = ComputeTransMat(sourceKeypoints,targetKeypoints,mode);

    sourceKeypoints->mesh->ApplyTransMat(transMat);
    sourceKeypoints->mesh->Update();

    sourceKeypoints->UpdatePoints();
    sourceKeypoints->UpdateGlyph();
    sourceKeypoints->Render();
}

vtkGlyphModel* ICP::AlignAndCopy(vtkGlyphModel* sourceKeypoints,vtkGlyphModel* targetKeypoints, int mode)
{
    transMat = ComputeTransMat(sourceKeypoints,targetKeypoints,mode);

    vtkMeshModel* sourceMeshCopy = sourceKeypoints->mesh->Copy();
    sourceMeshCopy->ApplyTransMat(transMat);
    sourceMeshCopy->Update();

    vtkGlyphModel* newKeypoints = new vtkGlyphModel(sourceMeshCopy);
    newKeypoints->SetPointsIds(sourceKeypoints->pointsIds);
    newKeypoints->UpdateGlyph();

    return newKeypoints;
}





























