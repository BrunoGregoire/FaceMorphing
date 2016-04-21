#include "InteractorStyle.h"

InteractorStyle::InteractorStyle()
{
    picker = vtkSmartPointer<vtkCellPicker>::New();

    icpDone = false;
    tpstReady = false;
    glyphSelected = false;
}

void InteractorStyle::SetContent(Content* _content)
{
    content = _content;
    glyph = new vtkGlyphModel(content->newModel);

    content->text = vtkSmartPointer<vtkTextActor>::New();
    content->newmodelRenderer->AddActor(content->text);

    /*
    currentKeypoint = 0;
    UpdateText();
    */
}

void InteractorStyle::SetICP(ICP *_icp)
{
    icp = _icp;
}

void InteractorStyle::SetTPST(TPST *_tpst)
{
    tpst = _tpst;
}

void InteractorStyle::SetTexturer(Texturer *_texturer)
{
    texturer = _texturer;
}

void InteractorStyle::SetRaycaster(Raycaster *_raycaster)
{
    raycaster = _raycaster;
}

void InteractorStyle::SetFaceDetector(FaceDetector *_faceDetector)
{
    faceDetector = _faceDetector;
}

/*
void InteractorStyle::OnRightButtonDown()
{
    // Get the selected point
    int x = Interactor->GetEventPosition()[0];
    int y = Interactor->GetEventPosition()[1];
    picker->Pick(x,y,0,content->newmodelRenderer);

    if(picker->GetActor() == content->newModel->actor)
    {
        double* position = picker->GetPickPosition();

        vtkIdType id = content->newModel->kdTree->FindClosestPoint(position);

        glyph->ClearPoints();
        glyph->InsertNextID(id);
        glyph->UpdateGlyph();
        glyph->ShowModel();
        glyph->Render();
    }
}
*/

void InteractorStyle::OnMiddleButtonDown()
{
    int x = Interactor->GetEventPosition()[0];
    int y = Interactor->GetEventPosition()[1];
    vtkSmartPointer<vtkCellPicker> tempPicker = vtkSmartPointer<vtkCellPicker>::New();
    tempPicker->Pick(x,y,0,content->newmodelRenderer);

    if(!glyphSelected)
    {
        glyphIndex = content->newmodelKeypoints->FindClosestPoint(tempPicker->GetPickPosition(),10);
        if(glyphIndex != -1)
        {
            content->newmodelKeypoints->RemovePointAndID(glyphIndex);
            content->newmodelKeypoints->Render();
            glyphSelected = true;
        }
    }
    else
    {
        content->newmodelKeypoints->InsertID(glyph->pointsIds->GetId(0),glyphIndex);
        content->newmodelKeypoints->UpdateIdLabels();

        glyph->ClearPoints();
        glyph->UpdateGlyph();
        glyph->ShowModel();
        glyph->Render();

        glyphSelected = false;
    }
}

void InteractorStyle::OnMouseMove()
{
    if(glyphSelected)
    {
        int x = Interactor->GetEventPosition()[0];
        int y = Interactor->GetEventPosition()[1];
        picker->Pick(x,y,0,content->newmodelRenderer);

        vtkIdType id = content->newModel->kdTree->FindClosestPoint(picker->GetPickPosition());

        glyph->ClearPoints();
        glyph->InsertNextID(id);
        glyph->UpdateGlyph();
        glyph->ShowModel();
        glyph->Render();
    }

    vtkInteractorStyleTrackballCamera::OnMouseMove();
}


void InteractorStyle::OnKeyPress()
{
    vtkRenderWindowInteractor *rwi = this->Interactor;
    std::string key = rwi->GetKeySym();

    /*
    if (key == "s")
        SaveKeypoint();
    */

    if (key == "e")
        ExportKeypoints();

    if (key == "a")
        DoAll();

    if (key == "t")
        AlignModels();

    if (key == "v")
        ToggleAlignedVisibility();

    if (key == "m")
        MorphModels();

    if (key == "x")
        TextureModels();

    if (key == "h")
        RaycastHead();

    if (key == "r")
        CylinderRaycast();

    if (key == "b")
        ComputeBlendshapes();

    if (key == "d")
        FaceDetection();

    if (key == "c")
        ResetCameras();

    if (key == "k")
        SmoothMorphedModel();

}

/*
void InteractorStyle::SaveKeypoint()
{
    if(picker->GetActor() == content->newModel->actor && currentKeypoint<content->nbKeypoints)
    {
        content->newmodelKeypoints->InsertNextID(glyph->pointsIds->GetId(0));
        content->newmodelKeypoints->UpdateGlyph();
        content->newmodelKeypoints->ShowModel();

        currentKeypoint++;
        UpdateText();
    }
}
*/

void InteractorStyle::ExportKeypoints()
{
    content->newmodelKeypoints->ExportIDs("Resources/newKeypoints.txt");
}

void InteractorStyle::LoadKeypoints(std::string path)
{
    content->newmodelKeypoints->LoadIDs(path);
    content->newmodelKeypoints->UpdateIdLabels();
    content->newmodelKeypoints->ShowModel();
    content->newmodelKeypoints->Render();
}

/*
void InteractorStyle::UpdateText()
{
    if(currentKeypoint<content->nbKeypoints)
    {
        content->text->SetInput(content->keypointsNames[currentKeypoint].c_str());
        content->text->SetPosition(0,0);
        content->text->GetTextProperty()->SetFontSize(24);
        content->text->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
    }
    else
        content->newmodelRenderer->RemoveActor(content->text);
    content->newmodelRenderWindow->Render();
}
*/

void InteractorStyle::DoAll()
{
    if(content->refKeypoints->GetNumberOfPoints() == content->newmodelKeypoints->GetNumberOfPoints())
    {
        if(!icpDone)
        {
            AlignModels();
            RaycastHead();
            MorphModels();
            tpst->basis = tpst->Basis::R;
        }

        ToggleAlignedVisibility();

        RaycastHead();
        CylinderRaycast();
        MorphModels();
        TextureModels();
    }
}

void InteractorStyle::AlignModels()
{
    if(!icpDone)
    {
        content->alignedKeypoints = icp->AlignAndCopy(content->newmodelKeypoints,content->refKeypoints,0);
        content->alignedKeypoints->SetRenderer(content->refRenderer);
        content->alignedKeypoints->SetRenderWindow(content->refRenderWindow);
        content->alignedKeypoints->ShowModel();

        content->alignedModel = content->alignedKeypoints->mesh;
        content->alignedModel->SetRenderer(content->refRenderer);
        content->alignedModel->SetRenderWindow(content->refRenderWindow);
        content->alignedModel->actor->GetProperty()->SetOpacity(0.5);
        content->alignedModel->ShowModel();

        content->newModel->polyData = content->alignedModel->polyData;
        content->newModel->Update();

        content->newmodelKeypoints->UpdatePoints();
        content->newmodelKeypoints->UpdateGlyph();

        icp->Align(content->refKeypoints,content->alignedKeypoints,1);

        content->refModel->axisTransMat = vtkSmartPointer<vtkMatrix4x4>::New();
        content->refModel->axisTransMat->DeepCopy(icp->transMat);
        content->refModel->ComputeAxisTransform();
        content->refModel->axis->ApplyTransMat(content->refModel->axisTransMat);
        content->refModel->axis->Update();

        content->alignedModel->Texture(content->newModel->texture);

        content->refKeypoints->HideModel();
        content->refKeypoints->HideIdLabels();
        content->newmodelKeypoints->HideModel();
        content->newmodelKeypoints->HideIdLabels();
        content->alignedKeypoints->HideModel();

        ResetCameras();

        icpDone = true;
        tpstReady = true;
    }
}

void InteractorStyle::ToggleAlignedVisibility()
{
    if(content->alignedModel->visible)
        content->alignedModel->HideModel();
    else
        content->alignedModel->ShowModel();
    content->alignedModel->Render();
}

void InteractorStyle::MorphModels()
{
    if(tpstReady)
    {
        tpst->Morph(content->refKeypoints,content->alignedKeypoints);

        content->refKeypoints->ClearPoints();
        content->alignedKeypoints->ClearPoints();
        content->refKeypoints->UpdateGlyph();
        content->alignedKeypoints->UpdateGlyph();

        content->refRenderWindow->Render();

        tpstReady = false;
    }
}

void InteractorStyle::TextureModels()
{
    if(icpDone)
    {
        texturer->Texture(content->alignedModel,content->refModel);
        content->refModel->Render();
    }
}

void InteractorStyle::RaycastHead()
{
    raycaster->TopRaycast(content->refKeypoints,content->alignedKeypoints);

    tpstReady = true;
}

void InteractorStyle::CylinderRaycast()
{
    raycaster->CylindricalRaycast(content->refKeypoints,content->alignedKeypoints);

    tpstReady = true;
}

void InteractorStyle::FaceDetection()
{
    content->newmodelKeypoints->ClearPoints();
    faceDetector->DetectAnProject(content->newmodelKeypoints);
    content->newmodelKeypoints->UpdateIdLabels();
    content->newmodelRenderWindow->Render();
}

void InteractorStyle::SmoothMorphedModel()
{

}

void InteractorStyle::CutNewmodel()
{

}

void InteractorStyle::ComputeBlendshapes()
{

}

void InteractorStyle::TakeScreenshot(vtkSmartPointer<vtkRenderWindow> renderWindow,std::string imgPath)
{

}

void InteractorStyle::ResetCameras()
{
    content->refRenderer->ResetCamera();
    double* fp;
    double* p;
    fp = content->refRenderer->GetActiveCamera()->GetFocalPoint();
    p = content->refRenderer->GetActiveCamera()->GetPosition();
    double dist = sqrt((p[0]-fp[0])*(p[0]-fp[0]) + (p[1]-fp[1])*(p[1]-fp[1]) + (p[2]-fp[2])*(p[2]-fp[2]) );
    content->refRenderer->GetActiveCamera()->SetPosition(fp[0], fp[1], fp[2]+dist);
    content->refRenderer->GetActiveCamera()->SetViewUp(0.0, 1.0, 0.0);
    content->refRenderer->GetActiveCamera()->Zoom(1.3);
    content->refRenderWindow->Render();

    vtkSmartPointer<vtkCamera> newCamera = vtkSmartPointer<vtkCamera>::New();
    newCamera->DeepCopy(content->refRenderer->GetActiveCamera());
    content->newmodelRenderer->SetActiveCamera(newCamera);
    content->newmodelRenderWindow->Render();
}

vtkStandardNewMacro(InteractorStyle);
