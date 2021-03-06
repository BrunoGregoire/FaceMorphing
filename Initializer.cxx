 #include "Initializer.h"

Initializer::Initializer()
{
    content = new Content();
    icp = new ICP();
    tpst = new TPST();
    texturer = new Texturer();
    raycaster = new Raycaster();
    faceDetector = new FaceDetector("Resources/shape_predictor_68_face_landmarks.dat");
    drawer = new Drawer();
    cutter = new Cutter();

    raycastLevel = new int;
    *raycastLevel = 1;
    eyesAndJaw = new bool;
    *eyesAndJaw = true;
    symMode = new bool;
    *symMode = true;
}

void Initializer::Init()
{
    // INIT REF RENDERER AND RENDER WINDOW
    content->refRenderer = vtkSmartPointer<vtkRenderer>::New();
    content->refRenderer->SetBackground(.3, .3, .3);
    content->refRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    content->refRenderWindow->AddRenderer(content->refRenderer);

    // INIT REF MODEL
    content->refModel = new vtkMeshModel();
    content->refModel->ReadFromOBJ("Models/Ref/Ref.obj");
    content->refModel->name = "RefModel";

    content->refLeftEyeModel = new vtkMeshModel();
    content->refLeftEyeModel->ReadFromOBJ("Models/Ref/EyeLeft_Mesh.obj");
    content->refLeftEyeModel->name = "EyeLeft_Mesh";

    content->refRightEyeModel = new vtkMeshModel();
    content->refRightEyeModel->ReadFromOBJ("Models/Ref/EyeRight_Mesh.obj");
    content->refRightEyeModel->name = "EyeRight_Mesh";

    content->refJawUpModel = new vtkMeshModel();
    content->refJawUpModel->ReadFromOBJ("Models/Ref/UpperTeeth_Mesh.obj");
    content->refJawUpModel->name = "UpperTeeth_Mesh";

    content->refJawDownModel = new vtkMeshModel();
    content->refJawDownModel->ReadFromOBJ("Models/Ref/Jaw_Mesh.obj");
    content->refJawDownModel->name = "Jaw_Mesh";

    content->refModel->AddLink(content->refLeftEyeModel);
    content->refModel->AddLink(content->refRightEyeModel);
    content->refModel->AddLink(content->refJawUpModel);
    content->refModel->AddLink(content->refJawDownModel);

    content->refModel->Center();
    content->refModel->BuildKdTree();
    content->refModel->Update();

    content->refModel->ReadTexture("Models/Ref/Textures/Ref.jpg");
    content->refModel->Update();

    content->refLeftEyeModel->ReadTexture("Models/Ref/Textures/RefEye.jpg");
    content->refLeftEyeModel->Update();

    content->refRightEyeModel->ReadTexture("Models/Ref/Textures/RefEye.jpg");
    content->refRightEyeModel->Update();

    content->refJawUpModel->ReadTexture("Models/Ref/Textures/Ref.jpg");
    content->refJawUpModel->Update();

    content->refJawDownModel->ReadTexture("Models/Ref/Textures/Ref.jpg");
    content->refJawDownModel->Update();

    content->refModel->SetRenderer(content->refRenderer);
    content->refModel->SetRenderWindow(content->refRenderWindow);
    content->refModel->ShowModel();

    content->refLeftEyeModel->SetRenderer(content->refRenderer);
    content->refLeftEyeModel->SetRenderWindow(content->refRenderWindow);
    content->refLeftEyeModel->ShowModel();

    content->refRightEyeModel->SetRenderer(content->refRenderer);
    content->refRightEyeModel->SetRenderWindow(content->refRenderWindow);
    content->refRightEyeModel->ShowModel();

    content->refJawUpModel->SetRenderer(content->refRenderer);
    content->refJawUpModel->SetRenderWindow(content->refRenderWindow);
    content->refJawUpModel->ShowModel();

    content->refJawDownModel->SetRenderer(content->refRenderer);
    content->refJawDownModel->SetRenderWindow(content->refRenderWindow);
    content->refJawDownModel->ShowModel();

    // INIT REF BLENDSHAPES
    content->refModel->bsNames.push_back("BrowsD_L");
    content->refModel->bsNames.push_back("BrowsD_R");
    content->refModel->bsNames.push_back("BrowsSqueeze_L");
    content->refModel->bsNames.push_back("BrowsSqueeze_R");
    content->refModel->bsNames.push_back("BrowsU_C_L");
    content->refModel->bsNames.push_back("BrowsU_C_R");
    content->refModel->bsNames.push_back("BrowsU_L");
    content->refModel->bsNames.push_back("BrowsU_R");
    content->refModel->bsNames.push_back("CheekSquint_L");
    content->refModel->bsNames.push_back("CheekSquint_R");
    content->refModel->bsNames.push_back("ChinLowerRaise");
    content->refModel->bsNames.push_back("ChinUpperRaise");
    content->refModel->bsNames.push_back("EyeBlink_L");
    content->refModel->bsNames.push_back("EyeBlink_R");
    content->refModel->bsNames.push_back("EyeDown_L");
    content->refModel->bsNames.push_back("EyeDown_R");
    content->refModel->bsNames.push_back("EyeIn_L");
    content->refModel->bsNames.push_back("EyeIn_R");
    content->refModel->bsNames.push_back("EyeOpen_L");
    content->refModel->bsNames.push_back("EyeOpen_R");
    content->refModel->bsNames.push_back("EyeOut_L");
    content->refModel->bsNames.push_back("EyeOut_R");
    content->refModel->bsNames.push_back("EyeSquint_L");
    content->refModel->bsNames.push_back("EyeSquint_R");
    content->refModel->bsNames.push_back("EyeUp_L");
    content->refModel->bsNames.push_back("EyeUp_R");
    content->refModel->bsNames.push_back("JawBackward");
    content->refModel->bsNames.push_back("JawChew");
    content->refModel->bsNames.push_back("JawFwd");
    content->refModel->bsNames.push_back("JawLeft");
    content->refModel->bsNames.push_back("JawOpen");
    content->refModel->bsNames.push_back("JawRight");
    content->refModel->bsNames.push_back("LipsFunnel_L");
    content->refModel->bsNames.push_back("LipsFunnel_R");
    content->refModel->bsNames.push_back("LipsLowerClose");
    content->refModel->bsNames.push_back("LipsLowerDown_L");
    content->refModel->bsNames.push_back("LipsLowerDown_R");
    content->refModel->bsNames.push_back("LipsLowerOpen");
    content->refModel->bsNames.push_back("LipsPucker_L");
    content->refModel->bsNames.push_back("LipsPucker_R");
    content->refModel->bsNames.push_back("LipsStretch_L");
    content->refModel->bsNames.push_back("LipsStretch_R");
    content->refModel->bsNames.push_back("LipsTogether");
    content->refModel->bsNames.push_back("LipsTowards_L");
    content->refModel->bsNames.push_back("LipsTowards_R");
    content->refModel->bsNames.push_back("LipsUpperClose");
    content->refModel->bsNames.push_back("LipsUpperOpen");
    content->refModel->bsNames.push_back("LipsUpperUp_L");
    content->refModel->bsNames.push_back("LipsUpperUp_R");
    content->refModel->bsNames.push_back("MouthDimple_L");
    content->refModel->bsNames.push_back("MouthDimple_R");
    content->refModel->bsNames.push_back("MouthDown");
    content->refModel->bsNames.push_back("MouthFrown_L");
    content->refModel->bsNames.push_back("MouthFrown_R");
    content->refModel->bsNames.push_back("MouthLeft");
    content->refModel->bsNames.push_back("MouthPress_L");
    content->refModel->bsNames.push_back("MouthPress_R");
    content->refModel->bsNames.push_back("MouthRight");
    content->refModel->bsNames.push_back("MouthSharpCornerPull_L");
    content->refModel->bsNames.push_back("MouthSharpCornerPull_R");
    content->refModel->bsNames.push_back("MouthSmile_L");
    content->refModel->bsNames.push_back("MouthSmile_R");
    content->refModel->bsNames.push_back("MouthUp");
    content->refModel->bsNames.push_back("NostrilContractor");
    content->refModel->bsNames.push_back("NostrilDepress_L");
    content->refModel->bsNames.push_back("NostrilDepress_R");
    content->refModel->bsNames.push_back("NostrilDilator");
    content->refModel->bsNames.push_back("Puff_L");
    content->refModel->bsNames.push_back("Puff_R");
    content->refModel->bsNames.push_back("Sneer_L");
    content->refModel->bsNames.push_back("Sneer_R");
    content->refModel->bsNames.push_back("Suck");

    content->refModel->LoadBlendshapes("Models/Ref/BS/");

    // TRIANGULATE THE MESH AND THE BSs
    content->refModel->Triangulate();
    content->refModel->Update();

    // INIT KEYPOINTS AREAS NAMES
    content->areasNames = new std::string[8];
    content->areasNames[0] = "Draw circle for the face";
    content->areasNames[1] = "Draw line for the right eyebrow";
    content->areasNames[2] = "Draw vertical line for the Nose";
    content->areasNames[3] = "Draw horizontal line for the bottom of the nose";
    content->areasNames[4] = "Draw circle around the right eye";
    content->areasNames[5] = "Draw circle around the mouth";
    content->areasNames[6] = "Draw circle around the right ear";
    content->areasNames[7] = "Draw line behind the head";

    // INIT REF KEYPOINTS
    content->refKeypoints = new vtkGlyphModel(content->refModel);
    content->refKeypoints->LoadIDs("Resources/refKeypoints.txt");
    content->refKeypoints->ShowModel();
    content->refKeypoints->UpdateIdLabels();

    content->customRefKeypoints = vtkSmartPointer<vtkIdList>::New();
    content->customRefKeypoints->DeepCopy(content->refKeypoints->pointsIds);

    // INIT REF AXIS
    double* origin = new double[3];
    double* end = new double[3];
    origin[0] = 0;
    origin[1] = -150;
    origin[2] = -50;
    end[0] = 0;
    end[1] = 150;
    end[2] = -50;
    content->refAxis = new vtkLineModel(origin,end);
    content->refAxis->SetRenderer(content->refRenderer);
    content->refAxis->SetRenderWindow(content->refRenderWindow);
    content->refAxis->UpdateLine();
    content->refAxis->ShowModel();
    content->refModel->axis = content->refAxis;

    // INIT INTERACTOR
    content->refInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    content->refInteractor->SetRenderWindow(content->refRenderWindow);
    vtkSmartPointer<RefInteractorStyle> refStyle = vtkSmartPointer<RefInteractorStyle>::New();
    refStyle->SetContent(content);
    refStyle->SetParameters(eyesAndJaw);
    content->refInteractor->SetInteractorStyle(refStyle);

    newmodelImported = false;
}

void Initializer::InitNewModel(std::string fileName)
{
    newmodelFileName = fileName;

    // INIT NEW MODEL RENDERER AND RENDER WINDOW
    content->newmodelRenderer = vtkSmartPointer<vtkRenderer>::New();
    content->newmodelRenderer->SetBackground(.3, .3, .3);
    content->newmodelRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    content->newmodelRenderWindow->AddRenderer(content->newmodelRenderer);


    // INIT NEW MODEL
    content->newModel = new vtkMeshModel();
    content->newModel->ReadFromOBJ(fileName);
    content->newModel->ScaleWith(content->refModel);
    content->newModel->Center();
    content->newModel->BuildKdTree();
    content->newModel->Update();
    content->newModel->SetRenderer(content->newmodelRenderer);
    content->newModel->SetRenderWindow(content->newmodelRenderWindow);
    content->newModel->ShowModel();

    // TRIANGULATE NEW MODEL
    content->newModel->Triangulate();
    content->newModel->Update();

    // INIT NEW MODEL KEYPOINTS
    content->newmodelKeypoints = new vtkGlyphModel(content->newModel);
    content->newmodelKeypoints->SetColor(0,1,0);

    // INIT INTERACTOR
    content->newmodelInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    content->newmodelInteractor->SetRenderWindow(content->newmodelRenderWindow);
    vtkSmartPointer<InteractorStyle> style = vtkSmartPointer<InteractorStyle>::New();
    style->SetContent(content);
    style->SetICP(icp);
    style->SetTPST(tpst);
    style->SetTexturer(texturer);
    style->SetRaycaster(raycaster);
    style->SetFaceDetector(faceDetector);
    style->SetDrawer(drawer);
    style->SetCutter(cutter);
    style->SetParameters(raycastLevel,symMode);
    content->newmodelInteractor->SetInteractorStyle(style);
    content->style = style;

    newmodelImported = true;
}

void Initializer::InitNewModelTexture(std::string fileName)
{
    newmodelTextureFileName = fileName;

    // INIT NEW MODEL TEXTURE
    content->newModel->ReadTexture(fileName);
    content->newModel->Update();
}
