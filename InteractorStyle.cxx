#include "InteractorStyle.h"

InteractorStyle::InteractorStyle()
{
    picker = vtkSmartPointer<vtkCellPicker>::New();

    icpDone = false;
    tpstReady = false;

    glyphSelected = false;

    drawMode = false;
    isDrawing = false;

    symetricMode = true;
    symPosition = new double[3];

    currentArea = 0;
}

void InteractorStyle::SetContent(Content* _content)
{
    content = _content;
    glyph = new vtkGlyphModel(content->newModel);
    symGlyph = new vtkGlyphModel(content->newModel);

    content->text = vtkSmartPointer<vtkTextActor>::New();
    content->newmodelRenderer->AddActor(content->text);
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

void InteractorStyle::SetDrawer(Drawer *_drawer)
{
    drawer = _drawer;
}

void InteractorStyle::SetCutter(Cutter *_cutter)
{
    cutter = _cutter;
}

void InteractorStyle::OnLeftButtonDown()
{
    if(drawMode)
    {
        if(!isDrawing)
        {
            drawOrigin = new double[2];
            drawEnd = new double[2];

            drawOrigin[0] = Interactor->GetEventPosition()[0];
            drawOrigin[1] = Interactor->GetEventPosition()[1];

            currentShape = new vtk2DModel();

            isDrawing = true;

        }
        else
        {
            drawEnd[0] = Interactor->GetEventPosition()[0];
            drawEnd[1] = Interactor->GetEventPosition()[1];

            if(currentArea<drawer->nbShapes)
            {
                drawer->SaveArea(currentShape,currentArea);
                drawer->ProjectPoints(currentArea,content->newmodelKeypoints);
                currentShape->HideModel();
                content->newmodelKeypoints->UpdateIdLabels();
                content->newmodelKeypoints->Render();
                currentArea++;
                UpdateText();
            }

            if(currentArea == drawer->nbShapes)
                drawMode = false;

            isDrawing = false;
        }
    }
    else
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void InteractorStyle::OnRightButtonDown()
{
    int x = Interactor->GetEventPosition()[0];
    int y = Interactor->GetEventPosition()[1];
    vtkSmartPointer<vtkCellPicker> tempPicker = vtkSmartPointer<vtkCellPicker>::New();
    tempPicker->Pick(x,y,0,content->newmodelRenderer);

    if(!glyphSelected)
    {
        glyphIndex = content->newmodelKeypoints->FindClosestPoint(tempPicker->GetPickPosition(),20);

        if(symetricMode)
            symGlyphIndex = KeypointsManager::GetSymmetric(glyphIndex);

        if(glyphIndex != -1)
        {
            if(symGlyphIndex == -1)
            {
                content->newmodelKeypoints->RemovePointAndID(glyphIndex);
            }
            else
            {
                if(glyphIndex > symGlyphIndex)
                {
                    content->newmodelKeypoints->RemovePointAndID(glyphIndex);
                    content->newmodelKeypoints->RemovePointAndID(symGlyphIndex);
                }
                else
                {
                    content->newmodelKeypoints->RemovePointAndID(symGlyphIndex);
                    content->newmodelKeypoints->RemovePointAndID(glyphIndex);
                }
                symGlyphSelected = true;
            }

            content->newmodelKeypoints->Render();
            glyphSelected = true;
        }
        else
          vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }
    else
    {
        std::cout<<"DEBUG 0"<<std::endl;
        if(symGlyphSelected)
        {
            if(glyphIndex < symGlyphIndex)
            {
                content->newmodelKeypoints->InsertID(glyph->pointsIds->GetId(0),glyphIndex);
                content->newmodelKeypoints->InsertID(symGlyph->pointsIds->GetId(0),symGlyphIndex);
            }
            else
            {
                content->newmodelKeypoints->InsertID(symGlyph->pointsIds->GetId(0),symGlyphIndex);
                content->newmodelKeypoints->InsertID(glyph->pointsIds->GetId(0),glyphIndex);
            }
        }
        else
        {
            std::cout<<"DEBUG"<<std::endl;
            content->newmodelKeypoints->InsertID(glyph->pointsIds->GetId(0),glyphIndex);
            std::cout<<"DEBUG"<<std::endl;
        }

        std::cout<<"DEBUG"<<std::endl;
        content->newmodelKeypoints->UpdateIdLabels();

        std::cout<<"DEBUG"<<std::endl;
        glyph->ClearPoints();
        glyph->UpdateGlyph();
        glyph->ShowModel();
        glyph->Render();

        std::cout<<"DEBUG"<<std::endl;
        symGlyph->ClearPoints();
        symGlyph->UpdateGlyph();
        symGlyph->ShowModel();
        symGlyph->Render();

        std::cout<<"DEBUG"<<std::endl;
        glyphSelected = false;
        symGlyphSelected = false;
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

        if(symetricMode && symGlyphIndex != -1)
        {
            symPosition[0] = -picker->GetPickPosition()[0];
            symPosition[1] = picker->GetPickPosition()[1];
            symPosition[2] = picker->GetPickPosition()[2];

            vtkIdType symId = content->newModel->kdTree->FindClosestPoint(symPosition);

            symGlyph->ClearPoints();
            symGlyph->InsertNextID(symId);
            symGlyph->UpdateGlyph();
            symGlyph->ShowModel();
            symGlyph->Render();
        }
    }

    if(drawMode && isDrawing)
    {
        drawEnd[0] = Interactor->GetEventPosition()[0];
        drawEnd[1] = Interactor->GetEventPosition()[1];

        currentShape->HideModel();
        currentShape = drawer->DrawShape(drawOrigin,drawEnd,currentArea);
        currentShape->SetRenderer(content->newmodelRenderer);
        currentShape->SetRenderWindow(content->newmodelRenderWindow);
        currentShape->ShowModel();
        currentShape->Render();
    }

    vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void InteractorStyle::OnKeyPress()
{
    vtkRenderWindowInteractor *rwi = this->Interactor;
    std::string key = rwi->GetKeySym();

    std::cout<<key<<std::endl;

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

    if (key == "d")
        FaceDetection();

    if (key == "c")
        ResetCameras();

    if (key == "Escape")
        CancelDrawing();

    if (key == "o")
        ToggleDrawMode();

    if (key == "y")
        InitCutPlane();

    if (key == "Up")
        CutPlaneUp();

    if (key == "Down")
        CutPlaneDown();

    if (key == "Return")
        Cut();

    std::cout<<key<<std::endl;

}

void InteractorStyle::ExportBlendshapes(std::string folderPath)
{
    content->refModel->ExportBlendshapes(folderPath);
}

void InteractorStyle::LoadKeypoints(std::string path)
{
    content->newmodelKeypoints->LoadIDs(path);
    content->newmodelKeypoints->UpdateIdLabels();
    content->newmodelKeypoints->ShowModel();
    content->newmodelKeypoints->Render();

    if(content->newmodelKeypoints->GetNumberOfPoints() == 60)
        currentArea = 6;
    else if(content->newmodelKeypoints->GetNumberOfPoints() == 66)
        currentArea = 7;
}

void InteractorStyle::SaveKeypoints(std::string path)
{
    content->newmodelKeypoints->ExportIDs(path);
}

void InteractorStyle::ExportModel(std::string path)
{
    content->refModel->ExportAsOBJ(path);
}

void InteractorStyle::ExportModelAndBS(std::string folderPath)
{
    content->refModel->ExportAsOBJ(folderPath+"result");
    ExportBlendshapes(folderPath);
}

void InteractorStyle::UpdateText()
{
    if(currentArea<drawer->nbShapes)
    {
        content->text->SetInput(content->areasNames[currentArea].c_str());
        content->text->SetPosition(0,0);
        content->text->GetTextProperty()->SetFontSize(24);
        content->text->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
    }
    else
        content->newmodelRenderer->RemoveActor(content->text);

     content->newmodelRenderWindow->Render();
}

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

        RaycastHead();
        CylinderRaycast(2);
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
        content->refModel->TextureBlendshapes();
        content->refModel->Render();
    }
}

void InteractorStyle::RaycastHead()
{
    RaycastHead(1);
}

void InteractorStyle::RaycastHead(int precision)
{
    raycaster->TopRaycast(content->refKeypoints,content->alignedKeypoints,precision);

    tpstReady = true;
}

void InteractorStyle::CylinderRaycast()
{
    CylinderRaycast(1);
}

void InteractorStyle::CylinderRaycast(int precision)
{
    raycaster->CylindricalRaycast(content->refKeypoints,content->alignedKeypoints,precision);

    tpstReady = true;
}

void InteractorStyle::FaceDetection()
{
    content->newmodelKeypoints->ClearPoints();
    faceDetector->DetectAnProject(content->newmodelKeypoints);
    content->newmodelKeypoints->UpdateIdLabels();
    content->newmodelRenderWindow->Render();
    if(content->newmodelKeypoints->GetNumberOfPoints()>0)
        currentArea = 6;
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

void InteractorStyle::ToggleDrawMode()
{
    if(drawMode)
    {
        CancelDrawing();
        content->text->SetInput("");
        content->newmodelRenderWindow->Render();
        drawMode = false;
    }
    else if(currentArea<drawer->nbShapes)
    {
        UpdateText();
        drawMode = true;
    }
}

void InteractorStyle::CancelDrawing()
{
    if(drawMode && isDrawing)
    {
        isDrawing = false;
        currentShape->HideModel();
        currentShape->Render();
    }
}

void InteractorStyle::InitCutPlane()
{
    cutter->InitPlane(content->newModel);
    cutter->planeModel->Render();
}

void InteractorStyle::CutPlaneUp()
{
    cutter->TranslatePlane(0,3,0);
}

void InteractorStyle::CutPlaneDown()
{
    cutter->TranslatePlane(0,-3,0);
}

void InteractorStyle::Cut()
{
    cutter->Cut(content->refModel);
    cutter->planeModel->HideModel();
    ResetCameras();
}

vtkStandardNewMacro(InteractorStyle);
