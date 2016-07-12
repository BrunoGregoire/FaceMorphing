#include "TPST.h"

TPST::TPST()
{
    tpst = vtkSmartPointer<vtkThinPlateSplineTransform>::New();
    //basis = Basis::R2LogR;
    basis = Basis::R;
}

void TPST::Morph(vtkGlyphModel *sourceKeypoints, vtkGlyphModel *targetKeypoints)
{
    vtkSmartPointer<vtkPoints> sourceLandmarks = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> targetLandmarks = vtkSmartPointer<vtkPoints>::New();

    sourceLandmarks->DeepCopy(sourceKeypoints->points);
    targetLandmarks->DeepCopy(targetKeypoints->points);

    tpst->SetSourceLandmarks(sourceLandmarks);
    tpst->SetTargetLandmarks(targetLandmarks);

    if(basis == Basis::R)
        tpst->SetBasisToR();
    else
        tpst->SetBasisToR2LogR();

    sourceKeypoints->mesh->ApplyTransform(tpst);
    sourceKeypoints->mesh->Update();
    sourceKeypoints->mesh->Render();

    sourceKeypoints->UpdatePoints();
    sourceKeypoints->UpdateGlyph();
}

