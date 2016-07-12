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

void RefInteractorStyle::SetParameters(bool *_eyesAndJaw)
{
    eyesAndJaw = _eyesAndJaw;
}

void RefInteractorStyle::UpdateParameters()
{
    if(*eyesAndJaw && content->refModel->linkedMeshes.size()==0)
    {
        content->refModel->linkedMeshes.push_back(content->refLeftEyeModel);
        content->refModel->linkedMeshes.push_back(content->refRightEyeModel);
        content->refModel->linkedMeshes.push_back(content->refJawDownModel);
        content->refModel->linkedMeshes.push_back(content->refJawUpModel);
        for(int i=0;i<content->refModel->linkedMeshes.size();i++)
            content->refModel->linkedMeshes[i]->ShowModel();
        content->refModel->Render();
    }
    else if(!*eyesAndJaw && content->refModel->linkedMeshes.size()>0)
    {
        for(int i=0;i<content->refModel->linkedMeshes.size();i++)
            content->refModel->linkedMeshes[i]->HideModel();
        content->refModel->linkedMeshes.clear();
        content->refModel->Render();
    }
}

void RefInteractorStyle::OnLeftButtonDown()
{
    if(Interactor->GetControlKey() != 0)
    {
        StartRotate();
    }
    else
    {
        if(Interactor->GetControlKey() == 0 && State == VTKIS_ROTATE)
            EndRotate();

        if(content->newModel != NULL)
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
        }
    }
}

void RefInteractorStyle::OnKeyPress()
{
    vtkRenderWindowInteractor *rwi = Interactor;
    std::string key = rwi->GetKeySym();

    if (key == "s")
        SaveAddKeypoint();
}

void RefInteractorStyle::OnMouseMove()
{
    SetCurrentCursor();
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void RefInteractorStyle::SetCurrentCursor()
{
    if(State == VTKIS_ROTATE)
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    else if(Interactor->GetControlKey() != 0)
        QApplication::setOverrideCursor(Qt::OpenHandCursor);
    else if(content->refKeypoints->visible
            && content->newModel != NULL
            && content->refKeypoints->GetNumberOfPoints()<content->newmodelKeypoints->GetNumberOfPoints())
        QApplication::setOverrideCursor(Qt::CrossCursor);
    else
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    QApplication::processEvents();
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

