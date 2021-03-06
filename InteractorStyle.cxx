#include "InteractorStyle.h"

InteractorStyle::InteractorStyle()
{
    picker = vtkSmartPointer<vtkCellPicker>::New();

    icpDone = false;
    tpstReady = false;

    glyphSelected = false;

    drawButton = NULL;
    drawMode = false;
    isDrawing = false;

    addKeypointMode = false;

    symPosition = new double[3];

    currentArea = 0;

    nbIterations = 0;
}

void InteractorStyle::SetContent(Content* _content)
{
    content = _content;
    glyph = new vtkGlyphModel(content->newModel);
    symGlyph = new vtkGlyphModel(content->newModel);

    content->text = vtkSmartPointer<vtkTextActor>::New();
    content->newmodelRenderer->AddActor(content->text);

    stateManager = new StateManager(content);
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

void InteractorStyle::SetParameters(int *_raycastLevel, bool* _symMode)
{
    raycastLevel = _raycastLevel;
    symMode = _symMode;
}

void InteractorStyle::UpdateParameters()
{

}

void InteractorStyle::OnLeftButtonDown()
{
    int x = Interactor->GetEventPosition()[0];
    int y = Interactor->GetEventPosition()[1];

    if(Interactor->GetControlKey() != 0)
    {
        StartRotate();
    }
    else
    {
        if(Interactor->GetControlKey() == 0 && State == VTKIS_ROTATE)
            EndRotate();

        if(drawMode)
        {
            if(!isDrawing)
            {
                drawOrigin = new double[2];
                drawEnd = new double[2];

                drawOrigin[0] = x;
                drawOrigin[1] = y;

                currentShape = new vtk2DModel();

                isDrawing = true;

            }
            else
            {
                drawEnd[0] = x;
                drawEnd[1] = y;

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
                    DrawModeOff();

                isDrawing = false;
            }
        }
        else if(addKeypointMode)
        {
            picker->Pick(x,y,0,content->newmodelRenderer);
            content->newModel->BuildKdTree();

            pickedId = content->newModel->kdTree->FindClosestPoint(picker->GetPickPosition());

            glyph->ClearPoints();
            glyph->InsertNextID(pickedId);
            glyph->UpdateGlyph();
            glyph->ShowModel();
            glyph->Render();
        }
    }
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

        if(*symMode)
            symGlyphIndex = KeypointsManager::GetSymmetric(glyphIndex);
        else
            symGlyphIndex = -1;

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
            OnMouseMove();
        }
        else
            vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }
    else
    {
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
            content->newmodelKeypoints->InsertID(glyph->pointsIds->GetId(0),glyphIndex);
        }
        content->newmodelKeypoints->UpdateIdLabels();

        glyph->ClearPoints();
        glyph->UpdateGlyph();
        glyph->ShowModel();
        glyph->Render();

        symGlyph->ClearPoints();
        symGlyph->UpdateGlyph();
        symGlyph->ShowModel();
        symGlyph->Render();

        glyphSelected = false;
        symGlyphSelected = false;
    }
}

void InteractorStyle::OnMouseMove()
{
    SetCurrentCursor();

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

        if(*symMode && symGlyphIndex != -1)
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
    char keyCode = rwi->GetKeyCode();

    switch(keyCode)
    {
    case 't': AlignAndMorph(); break;

    case 'v': ToggleAlignedVisibility(); break;

    case 'm': MorphModels(); break;

    case 'x': TextureModels(); break;

    case 'h': RaycastHead(); break;

    case 'r': CylinderRaycast(); break;

    case 'd': FaceDetection(); break;

    case 'c': ResetCameras(); break;

    case 's': SaveAddKeypoint(); break;

    case 'y': InitCutPlane(); break;

    case 'w':
        content->refModel->Smooth();
        content->refModel->ComputeNormals();
        content->refModel->Update();
        content->refModel->Render();
    break;}

    if(key == "Escape") CancelDrawing();

    if(key == "Up") CutPlaneUp();

    if(key == "Down") CutPlaneDown();

    if(key == "Return") Cut();

    if(key == "Comma")
    {
        Matcher::MatchPerfectly(content->refModel,content->alignedModel,20);
        content->refModel->Triangulate();
        content->refModel->Update();
        content->refModel->Render();
    }
}

void InteractorStyle::SetCurrentCursor()
{
    if(State == VTKIS_ROTATE)
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    else if(Interactor->GetControlKey() != 0)
        QApplication::setOverrideCursor(Qt::OpenHandCursor);
    else if(drawMode || addKeypointMode)
        QApplication::setOverrideCursor(Qt::CrossCursor);
    else
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    QApplication::processEvents();
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
    else
        currentArea = drawer->nbShapes;
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
        content->text->SetInput("");

     content->newmodelRenderWindow->Render();
}

void InteractorStyle::AlignAndMorph()
{
    if(!icpDone && content->refKeypoints->GetNumberOfPoints() == content->newmodelKeypoints->GetNumberOfPoints())
    {
        content->customNewmodelKeypoints = vtkSmartPointer<vtkIdList>::New();
        content->customNewmodelKeypoints->DeepCopy(content->newmodelKeypoints->pointsIds);

        content->alignedKeypoints = icp->AlignAndCopy(content->newmodelKeypoints,content->refKeypoints,0);
        content->alignedKeypoints->SetRenderer(content->refRenderer);
        content->alignedKeypoints->SetRenderWindow(content->refRenderWindow);
        content->alignedKeypoints->glyphColor[0] = 0;
        content->alignedKeypoints->glyphColor[1] = 1;
        content->alignedKeypoints->glyphColor[2] = 0;
        content->alignedKeypoints->UpdateGlyph();
        content->alignedKeypoints->ShowModel();

        content->alignedModel = content->alignedKeypoints->mesh;
        content->alignedModel->SetRenderer(content->refRenderer);
        content->alignedModel->SetRenderWindow(content->refRenderWindow);
        content->alignedModel->actor->GetProperty()->SetOpacity(0.7);
        content->alignedModel->ShowModel();

        content->newModel->polyData = content->alignedModel->polyData;
        content->newModel->Update();

        content->newmodelKeypoints->ClearPoints();
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

        MorphModels();
        TextureModels();
    }
}

void InteractorStyle::NewIteration()
{
    if(icpDone)
    {
        std::cout<<"Iteration no:"<<nbIterations<<std::endl;

        if(addKeypointMode)
            AddKeypointOff();

        RaycastHead();
        CylinderRaycast(*raycastLevel);
        MorphModels();
        TextureModels();

        nbIterations++;
    }
}

void InteractorStyle::MatchPerfectly()
{
    if(icpDone && nbIterations>0)
    {
        Matcher::MatchPerfectly(content->refModel,content->alignedModel,20);
        content->refModel->Triangulate();
        content->refModel->Update();
        content->refModel->Render();

        TextureModels();
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

        if(!addKeypointMode)
        {
            content->refKeypoints->ClearPoints();
            content->alignedKeypoints->ClearPoints();
            content->newmodelKeypoints->ClearPoints();
            content->refKeypoints->UpdateGlyph();
            content->alignedKeypoints->UpdateGlyph();
            content->newmodelKeypoints->UpdateGlyph();
            content->refKeypoints->UpdateIdLabels();
            content->alignedKeypoints->UpdateIdLabels();
            content->newmodelKeypoints->UpdateIdLabels();
        }

        content->refRenderWindow->Render();
        content->newmodelRenderWindow->Render();

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

void InteractorStyle::ToggleDrawMode(QPushButton *button)
{
    if(drawButton == NULL)
        drawButton = button;

    if(drawMode)
    {
        DrawModeOff();
    }
    else if(currentArea<drawer->nbShapes)
    {
        DrawModeOn();
    }
    else if(icpDone && !addKeypointMode)
    {
        AddKeypointOn();
    }
    else if(icpDone && addKeypointMode)
    {
        AddKeypointOff();
    }

}

void InteractorStyle::DrawModeOff()
{
    if(drawButton)
    {
        QIcon icon;
        icon.addFile(QStringLiteral("Icons/DrawIconOff.png"), QSize(), QIcon::Normal, QIcon::Off);
        drawButton->setIcon(icon);
    }

    CancelDrawing();
    content->text->SetInput("");
    content->newmodelRenderWindow->Render();
    drawMode = false;
}
void InteractorStyle::DrawModeOn()
{
    if(drawButton)
    {
        QIcon icon;
        icon.addFile(QStringLiteral("Icons/DrawIconOn.png"), QSize(), QIcon::Normal, QIcon::Off);
        drawButton->setIcon(icon);
    }

    UpdateText();
    drawMode = true;
}
void InteractorStyle::AddKeypointOn()
{
    if(drawButton)
    {
        QIcon icon;
        icon.addFile(QStringLiteral("Icons/DrawIconOn.png"), QSize(), QIcon::Normal, QIcon::Off);
        drawButton->setIcon(icon);
    }

    addKeypointMode = true;

    content->refKeypoints->pointsIds->DeepCopy(content->customRefKeypoints);
    content->refKeypoints->UpdatePoints();
    content->refKeypoints->UpdateGlyph();
    content->refKeypoints->UpdateIdLabels();
    content->refKeypoints->ShowModel();
    content->refKeypoints->Render();

    content->newmodelKeypoints->pointsIds->DeepCopy(content->customNewmodelKeypoints);
    content->newmodelKeypoints->UpdatePoints();
    content->newmodelKeypoints->UpdateGlyph();
    content->newmodelKeypoints->UpdateIdLabels();
    content->newmodelKeypoints->ShowModel();
    content->newmodelKeypoints->Render();

    content->alignedKeypoints->pointsIds->DeepCopy(content->customNewmodelKeypoints);
    content->alignedKeypoints->UpdatePoints();
    content->alignedKeypoints->UpdateGlyph();
    content->alignedKeypoints->HideIdLabels();
    content->alignedKeypoints->HideModel();

    content->text->SetInput("Click to add an additional landmark");
    content->text->SetPosition(0,0);
    content->text->GetTextProperty()->SetFontSize(24);
    content->text->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
    content->newmodelRenderWindow->Render();
}
void InteractorStyle::AddKeypointOff()
{
    if(drawButton)
    {
        QIcon icon;
        icon.addFile(QStringLiteral("Icons/DrawIconOff.png"), QSize(), QIcon::Normal, QIcon::Off);
        drawButton->setIcon(icon);
    }

    addKeypointMode = false;

    glyph->HideModel();

    content->refKeypoints->ClearPoints();
    content->refKeypoints->UpdateGlyph();
    content->refKeypoints->HideIdLabels();
    content->refKeypoints->HideModel();
    content->refKeypoints->Render();

    content->newmodelKeypoints->ClearPoints();
    content->newmodelKeypoints->UpdateGlyph();
    content->newmodelKeypoints->HideIdLabels();
    content->newmodelKeypoints->HideModel();
    content->newmodelKeypoints->Render();

    content->alignedKeypoints->ClearPoints();
    content->alignedKeypoints->UpdateGlyph();
    content->alignedKeypoints->HideIdLabels();
    content->alignedKeypoints->HideModel();
    content->alignedKeypoints->Render();

    content->text->SetInput("");
    content->newmodelRenderWindow->Render();
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

void InteractorStyle::SaveAddKeypoint()
{
    glyph->ClearPoints();
    glyph->UpdateGlyph();
    glyph->HideModel();

    content->newmodelKeypoints->InsertNextID(pickedId);
    content->newmodelKeypoints->UpdateGlyph();
    content->newmodelKeypoints->ShowModel();
    content->newmodelKeypoints->UpdateIdLabels();
    content->newmodelKeypoints->Render();

    if(icpDone)
    {
        content->alignedKeypoints->InsertNextID(pickedId);
        content->alignedKeypoints->UpdateGlyph();
        content->alignedKeypoints->HideModel();
        content->alignedKeypoints->HideIdLabels();

        if(content->newmodelKeypoints->GetNumberOfPoints() == content->alignedKeypoints->GetNumberOfPoints())
            tpstReady = true;
    }

    content->customNewmodelKeypoints->DeepCopy(content->newmodelKeypoints->pointsIds);
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
