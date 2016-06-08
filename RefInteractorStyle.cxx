#include "RefInteractorStyle.h"

RefInteractorStyle::RefInteractorStyle()
{
    selectionPossible = false;
    picker = vtkSmartPointer<vtkCellPicker>::New();
}

void RefInteractorStyle::SetContent(Content* _content)
{
    content = _content;
    glyph = new vtkGlyphModel(content->refModel);
    glyph->SetColor(1,1,1);
}

void RefInteractorStyle::OnLeftButtonDown()
{
    CheckSelectionPossible();
    if(selectionPossible)
    {
        int x = Interactor->GetEventPosition()[0];
        int y = Interactor->GetEventPosition()[1];
        picker->Pick(x,y,0,content->refRenderer);
        content->refModel->BuildKdTree();

        pickedId = content->refModel->kdTree->FindClosestPoint(picker->GetPickPosition());

        glyph->ClearPoints();
        glyph->InsertNextID(pickedId);
        glyph->UpdateGlyph();
        glyph->ShowModel();
        glyph->Render();
    }
    else
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void RefInteractorStyle::OnKeyPress()
{
    vtkRenderWindowInteractor *rwi = Interactor;
    std::string key = rwi->GetKeySym();

    if (key == "s")
        SaveAddKeypoint();
}

void RefInteractorStyle::CheckSelectionPossible()
{
    if(content->refKeypoints->GetNumberOfPoints()<content->newmodelKeypoints->GetNumberOfPoints())
        selectionPossible = true;
    else
        selectionPossible = false;
}

void RefInteractorStyle::SaveAddKeypoint()
{
    if(selectionPossible)
    {
        glyph->ClearPoints();
        glyph->UpdateGlyph();
        glyph->HideModel();

        content->refKeypoints->InsertNextID(pickedId);
        content->refKeypoints->UpdateGlyph();
        content->refKeypoints->UpdateIdLabels();
        content->refKeypoints->Render();

        content->customRefKeypoints->DeepCopy(content->refKeypoints->pointsIds);

        if(content->refKeypoints->GetNumberOfPoints() == content->newmodelKeypoints->GetNumberOfPoints())
            MorphModels();
    }
    CheckSelectionPossible();
}

void RefInteractorStyle::MorphModels()
{
     InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(content->style.GetPointer());
     interactor->MorphModels();
}

vtkStandardNewMacro(RefInteractorStyle);

