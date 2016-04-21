 #include "Initializer.h"

Initializer::Initializer()
{
    content = new Content();
    icp = new ICP();
    tpst = new TPST();
    texturer = new Texturer();
    raycaster = new Raycaster();
    faceDetector = new FaceDetector("Resources/shape_predictor_68_face_landmarks.dat");
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
    content->refModel->BuildKdTree();
    content->refModel->Update();
    content->refModel->ReadTexture("Models/Ref/Textures/Ref.jpg");
    content->refModel->Update();
    content->refModel->SetRenderer(content->refRenderer);
    content->refModel->SetRenderWindow(content->refRenderWindow);
    content->refModel->ShowModel();

    // INIT REF KEYPOINTS NAMES
    content->nbKeypoints = 19;
    content->keypointsNames = new std::string[content->nbKeypoints];
    content->keypointsNames[0] = "Select Left Eye Int Corner";
    content->keypointsNames[1] = "Select Left Eye Up";
    content->keypointsNames[2] = "Select Left Eye Down";
    content->keypointsNames[3] = "Select Left Eye Ext Corner";
    content->keypointsNames[4] = "Select Right Eye Int Corner";
    content->keypointsNames[5] = "Select Right Eye up";
    content->keypointsNames[6] = "Select Right Eye down";
    content->keypointsNames[7] = "Select Right Eye Ext Corner";
    content->keypointsNames[8] = "Select Nose Left Corner";
    content->keypointsNames[9] = "Select End Nose";
    content->keypointsNames[10] = "Select Nose Right Corner";
    content->keypointsNames[11] = "Select Mouth Left Corner";
    content->keypointsNames[12] = "Select Mouth Center";
    content->keypointsNames[13] = "Select Mouth Right Corner";
    content->keypointsNames[14] = "Select Start Neck";
    content->keypointsNames[15] = "Select Left Ear Down";
    content->keypointsNames[16] = "Select Left Ear Up";
    content->keypointsNames[17] = "Select Right Ear Down";
    content->keypointsNames[18] = "Select Right Ear Up";

    // INIT REF KEYPOINTS
    content->refKeypoints = new vtkGlyphModel(content->refModel);
    content->refKeypoints->LoadIDs("Resources/refKeypoints.txt");
    content->refKeypoints->ShowModel();
    content->refKeypoints->UpdateIdLabels();
    /*
    content->refKeypoints->InsertPointID(13349);
    content->refKeypoints->InsertPointID(13360);
    content->refKeypoints->InsertPointID(545);
    content->refKeypoints->InsertPointID(13327);
    content->refKeypoints->InsertPointID(27608);
    content->refKeypoints->InsertPointID(27619);
    content->refKeypoints->InsertPointID(14786);
    content->refKeypoints->InsertPointID(27565);
    content->refKeypoints->InsertPointID(10194);
    content->refKeypoints->InsertPointID(7434);
    content->refKeypoints->InsertPointID(24434);
    content->refKeypoints->InsertPointID(6390);
    content->refKeypoints->InsertPointID(5938);
    content->refKeypoints->InsertPointID(20618);
    content->refKeypoints->InsertPointID(10942);
    content->refKeypoints->InsertPointID(8642);
    content->refKeypoints->InsertPointID(533);
    content->refKeypoints->InsertPointID(22882);
    content->refKeypoints->InsertPointID(16483);
    content->refKeypoints->UpdateGlyph();
    content->refKeypoints->ShowModel();
    */

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

    newmodelImported = false;
}

void Initializer::InitNewModel(std::string fileName)
{
    // INIT NEW MODEL RENDERER AND RENDER WINDOW
    content->newmodelRenderer = vtkSmartPointer<vtkRenderer>::New();
    content->newmodelRenderer->SetBackground(.3, .3, .3);
    content->newmodelRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    content->newmodelRenderWindow->AddRenderer(content->newmodelRenderer);

    // INIT NEW MODEL
    content->newModel = new vtkMeshModel();
    content->newModel->ReadFromOBJ(fileName);
    content->newModel->ScaleWith(content->refModel);
    content->newModel->BuildKdTree();
    content->newModel->Update();
    content->newModel->SetRenderer(content->newmodelRenderer);
    content->newModel->SetRenderWindow(content->newmodelRenderWindow);
    content->newModel->ShowModel();

    // INIT NEW MODEL KEYPOINTS
    content->newmodelKeypoints = new vtkGlyphModel(content->newModel);
    content->newmodelKeypoints->SetColor(0,1,0);

    // INIT INTERACTOR
    content->interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    content->interactor->SetRenderWindow(content->newmodelRenderWindow);
    vtkSmartPointer<InteractorStyle> style = vtkSmartPointer<InteractorStyle>::New();
    style->SetContent(content);
    style->SetICP(icp);
    style->SetTPST(tpst);
    style->SetTexturer(texturer);
    style->SetRaycaster(raycaster);
    style->SetFaceDetector(faceDetector);
    content->interactor->SetInteractorStyle(style);
    content->style = style;

    newmodelImported = true;
}

void Initializer::InitNewModelTexture(std::string fileName)
{
    // INIT NEW MODEL TEXTURE
    content->newModel->ReadTexture(fileName);
    content->newModel->Update();
}
