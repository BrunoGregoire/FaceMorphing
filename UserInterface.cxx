#include "UserInterface.h"

void UserInterface::setupUi(QMainWindow *MainWindow)
{
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QStringLiteral("MainWindow"));
    MainWindow->resize(1018, 776);

    actionImport = new QAction(MainWindow);
    actionImport->setObjectName(QStringLiteral("actionImport"));
    QObject::connect(actionImport,SIGNAL(triggered()),this,SLOT(ImportButtonPressed()));

    actionImport_texture = new QAction(MainWindow);
    actionImport_texture->setObjectName(QStringLiteral("actionImport_texture"));
    QObject::connect(actionImport_texture,SIGNAL(triggered()),this,SLOT(TextureButtonPressed()));

    actionImport_all = new QAction(MainWindow);
    actionImport_all->setObjectName(QStringLiteral("actionImport_all"));
    QObject::connect(actionImport_all,SIGNAL(triggered()),this,SLOT(ImportAllButtonPressed()));

    actionLoad_keypoints = new QAction(MainWindow);
    actionLoad_keypoints->setObjectName(QStringLiteral("actionLoad_keypoints"));
    QObject::connect(actionLoad_keypoints,SIGNAL(triggered()),this,SLOT(LoadKeypointsButtonPressed()));

    actionExport_model = new QAction(MainWindow);
    actionExport_model->setObjectName(QStringLiteral("actionExport_model"));

    actionExport_blend_shapes = new QAction(MainWindow);
    actionExport_blend_shapes->setObjectName(QStringLiteral("actionExport_blend_shapes"));

    actionExport_all = new QAction(MainWindow);
    actionExport_all->setObjectName(QStringLiteral("actionExport_all"));

    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QStringLiteral("centralwidget"));

    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(5);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

    groupBox = new QGroupBox(centralwidget);
    groupBox->setObjectName(QStringLiteral("groupBox"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy);
    groupBox->setMinimumSize(QSize(50, 20));
    groupBox->setBaseSize(QSize(0, 35));
    groupBox->setLayoutDirection(Qt::RightToLeft);
    groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

    /*
    SavePointButton = new QPushButton(groupBox);
    SavePointButton->setObjectName(QStringLiteral("SavePointButton"));
    SavePointButton->setGeometry(QRect(0, 0, 51, 51));
    SavePointButton->setToolTip(QStringLiteral("Save selected key point (s)"));
    SavePointButton->setToolTipDuration(-1);
    QIcon SavePointicon;
    SavePointicon.addFile(QStringLiteral("Icons/SaveIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    SavePointButton->setIcon(SavePointicon);
    SavePointButton->setIconSize(QSize(45, 45));
    QObject::connect(SavePointButton,SIGNAL(pressed()),this,SLOT(SavePointButtonPressed()));
    */

    AllButton = new QPushButton(groupBox);
    AllButton->setObjectName(QStringLiteral("AllButton"));
    AllButton->setGeometry(QRect(0, 50, 51, 51));
    AllButton->setToolTip(QStringLiteral("Perform all the morphing based on the selected key points (a)"));
    AllButton->setToolTipDuration(-1);
    QIcon AllIcon;
    AllIcon.addFile(QStringLiteral("Icons/AllIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    AllButton->setIcon(AllIcon);
    AllButton->setIconSize(QSize(45, 45));
    QObject::connect(AllButton,SIGNAL(pressed()),this,SLOT(AllButtonPressed()));

    AlignButton = new QPushButton(groupBox);
    AlignButton->setObjectName(QStringLiteral("AlignButton"));
    AlignButton->setGeometry(QRect(0, 100, 51, 51));
    AlignButton->setToolTip(QStringLiteral("Align models (t)"));
    AlignButton->setToolTipDuration(-1);
    QIcon AlignIcon;
    AlignIcon.addFile(QStringLiteral("Icons/AlignIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    AlignButton->setIcon(AlignIcon);
    AlignButton->setIconSize(QSize(45, 45));
    QObject::connect(AlignButton,SIGNAL(pressed()),this,SLOT(AlignButtonPressed()));

    ToggleVisibilityButton = new QPushButton(groupBox);
    ToggleVisibilityButton->setObjectName(QStringLiteral("ToggleVisibilityButton"));
    ToggleVisibilityButton->setGeometry(QRect(0, 150, 51, 51));
    ToggleVisibilityButton->setToolTip(QStringLiteral("Toggle aligned model visibility (v)"));
    ToggleVisibilityButton->setToolTipDuration(-1);
    QIcon ToggleVisibilityIcon;
    ToggleVisibilityIcon.addFile(QStringLiteral("Icons/ToggleVisibilityIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    ToggleVisibilityButton->setIcon(ToggleVisibilityIcon);
    ToggleVisibilityButton->setIconSize(QSize(45, 45));
    QObject::connect(ToggleVisibilityButton,SIGNAL(pressed()),this,SLOT(ToggleVisibilityButtonPressed()));

    MorphButton = new QPushButton(groupBox);
    MorphButton->setObjectName(QStringLiteral("MorphButton"));
    MorphButton->setGeometry(QRect(0, 200, 51, 51));
    MorphButton->setToolTip(QStringLiteral("Morph the ref into the new model (m)"));
    MorphButton->setToolTipDuration(-1);
    QIcon MorphIcon;
    MorphIcon.addFile(QStringLiteral("Icons/MorphIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    MorphButton->setIcon(MorphIcon);
    MorphButton->setIconSize(QSize(45, 45));
    QObject::connect(MorphButton,SIGNAL(pressed()),this,SLOT(MorphButtonPressed()));

    DlibButton = new QPushButton(groupBox);
    DlibButton->setObjectName(QStringLiteral("DlibButton"));
    DlibButton->setGeometry(QRect(0, 250, 51, 51));
    DlibButton->setToolTip(QStringLiteral("Automatically find new landmarks thanks to face detetction/recognition (l)"));
    DlibButton->setToolTipDuration(-1);
    QIcon DlibIcon;
    DlibIcon.addFile(QStringLiteral("Icons/DlibIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    DlibButton->setIcon(DlibIcon);
    DlibButton->setIconSize(QSize(45, 45));
    QObject::connect(DlibButton,SIGNAL(pressed()),this,SLOT(DlibButtonPressed()));

    RaycastHeadButton = new QPushButton(groupBox);
    RaycastHeadButton->setObjectName(QStringLiteral("RaycastButton"));
    RaycastHeadButton->setGeometry(QRect(0, 300, 51, 51));
    RaycastHeadButton->setToolTip(QStringLiteral("Perform raycast to find new key points for the head (r)"));
    RaycastHeadButton->setToolTipDuration(-1);
    QIcon RaycastHeadIcon;
    RaycastHeadIcon.addFile(QStringLiteral("Icons/HeadIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    RaycastHeadButton->setIcon(RaycastHeadIcon);
    RaycastHeadButton->setIconSize(QSize(45, 45));
    QObject::connect(RaycastHeadButton,SIGNAL(pressed()),this,SLOT(RaycastHeadButtonPressed()));

    RaycastButton = new QPushButton(groupBox);
    RaycastButton->setObjectName(QStringLiteral("RaycastButton"));
    RaycastButton->setGeometry(QRect(0, 350, 51, 51));
    RaycastButton->setToolTip(QStringLiteral("Perform raycast to find new key points (r)"));
    RaycastButton->setToolTipDuration(-1);
    QIcon RaycastIcon;
    RaycastIcon.addFile(QStringLiteral("Icons/RaycastIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    RaycastButton->setIcon(RaycastIcon);
    RaycastButton->setIconSize(QSize(45, 45));
    QObject::connect(RaycastButton,SIGNAL(pressed()),this,SLOT(RaycastButtonPressed()));

    RetextureButton = new QPushButton(groupBox);
    RetextureButton->setObjectName(QStringLiteral("RetextureButton"));
    RetextureButton->setGeometry(QRect(0, 400, 51, 51));
    RetextureButton->setToolTip(QStringLiteral("Texture morphed model (x)"));
    RetextureButton->setToolTipDuration(-1);
    QIcon TextureIcon;
    TextureIcon.addFile(QStringLiteral("Icons/TextureIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    RetextureButton->setIcon(TextureIcon);
    RetextureButton->setIconSize(QSize(45, 45));
    QObject::connect(RetextureButton,SIGNAL(pressed()),this,SLOT(RetextureButtonPressed()));

    BlendshapsButton = new QPushButton(groupBox);
    BlendshapsButton->setObjectName(QStringLiteral("BlendshapsButton"));
    BlendshapsButton->setGeometry(QRect(0, 450, 51, 51));
    BlendshapsButton->setToolTip(QStringLiteral("Compute the blend shapes for the morphed model (b)"));
    BlendshapsButton->setToolTipDuration(-1);
    QIcon BlendshapesIcon;
    BlendshapesIcon.addFile(QStringLiteral("Icons/Blendshapes.png"), QSize(), QIcon::Normal, QIcon::Off);
    BlendshapsButton->setIcon(BlendshapesIcon);
    BlendshapsButton->setIconSize(QSize(45, 45));
    QObject::connect(BlendshapsButton,SIGNAL(pressed()),this,SLOT(BlendshapsButtonPressed()));

    horizontalLayout->addWidget(groupBox);

    RefQvtkWidget = new QVTKWidget(centralwidget);
    RefQvtkWidget->setObjectName(QStringLiteral("RefQvtkWidget"));

    horizontalLayout->addWidget(RefQvtkWidget);

    NewmodelQvtkWidget = new QVTKWidget(centralwidget);
    NewmodelQvtkWidget->setObjectName(QStringLiteral("NewmodelQvtkWidget"));

    horizontalLayout->addWidget(NewmodelQvtkWidget);

    verticalLayout->addLayout(horizontalLayout);

    gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QStringLiteral("menubar"));
    menubar->setGeometry(QRect(0, 0, 1018, 21));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QStringLiteral("menuFile"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QStringLiteral("statusbar"));
    MainWindow->setStatusBar(statusbar);

    menubar->addAction(menuFile->menuAction());
    menuFile->addAction(actionImport);
    menuFile->addAction(actionImport_texture);
    menuFile->addAction(actionImport_all);
    menuFile->addAction(actionLoad_keypoints);
    menuFile->addSeparator();
    menuFile->addAction(actionExport_model);
    menuFile->addAction(actionExport_blend_shapes);
    menuFile->addAction(actionExport_all);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
}

void UserInterface::retranslateUi(QMainWindow *MainWindow)
{
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "FaceMorphing", 0));
    actionImport->setText(QApplication::translate("MainWindow", "Import model", 0));
    actionImport_texture->setText(QApplication::translate("MainWindow", "Import texture", 0));
    actionImport_all->setText(QApplication::translate("MainWindow", "Import model and texture", 0));
    actionLoad_keypoints->setText(QApplication::translate("MainWindow", "Load keypoints", 0));
    actionExport_model->setText(QApplication::translate("MainWindow", "Export model", 0));
    actionExport_blend_shapes->setText(QApplication::translate("MainWindow", "Export blend shapes", 0));
    actionExport_all->setText(QApplication::translate("MainWindow", "Export all", 0));
    groupBox->setTitle(QString());

    menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
}

void UserInterface::ImportButtonPressed()
{
    std::string fileName = QFileDialog::getOpenFileName(centralwidget,
                                                        tr("Choose obj file"),NULL,
                                                        tr("OBJ Files (*.obj)")).toStdString();

    if(!fileName.empty())
    {
        initializer->InitNewModel(fileName);

        NewmodelQvtkWidget->SetRenderWindow(initializer->content->newmodelRenderWindow);
        NewmodelQvtkWidget->GetRenderWindow()->Render();
    }
}

void UserInterface::TextureButtonPressed()
{
    std::string fileName = QFileDialog::getOpenFileName(centralwidget,
                                                        tr("Choose jpg file"),NULL,
                                                        tr("Images (*.jpg)")).toStdString();

    if(!fileName.empty() && initializer->content->newModel->actor != NULL)
    {
        initializer->InitNewModelTexture(fileName);

        NewmodelQvtkWidget->GetRenderWindow()->Render();
    }
}

void UserInterface::ImportAllButtonPressed()
{
    ImportButtonPressed();
    TextureButtonPressed();
}

void UserInterface::LoadKeypointsButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        std::string fileName = QFileDialog::getOpenFileName(centralwidget,
                                                            tr("Choose txt file"),NULL,
                                                            tr("Text files (*.txt)")).toStdString();

        if(!fileName.empty())
        {
            interactor->LoadKeypoints(fileName);
        }
    }
}

/*
void UserInterface::SavePointButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->SaveKeypoint();
    }
}
*/

void UserInterface::AlignButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->AlignModels();
    }
}

void UserInterface::ToggleVisibilityButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->ToggleAlignedVisibility();
    }
}

void UserInterface::AllButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->DoAll();
    }
}

void UserInterface::MorphButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->MorphModels();
    }
}

void UserInterface::DlibButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->FaceDetection();
    }
}

void UserInterface::RaycastHeadButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->RaycastHead();
    }
}

void UserInterface::RaycastButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->CylinderRaycast();
    }
}

void UserInterface::RetextureButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->TextureModels();
    }
}

void UserInterface::BlendshapsButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->ComputeBlendshapes();
    }
}

