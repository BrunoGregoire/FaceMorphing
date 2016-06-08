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

    actionSave_keypoints = new QAction(MainWindow);
    actionSave_keypoints->setObjectName(QStringLiteral("actionSave_keypoints"));
    QObject::connect(actionSave_keypoints,SIGNAL(triggered()),this,SLOT(SaveKeypointsButtonPressed()));

    actionExport_model = new QAction(MainWindow);
    actionExport_model->setObjectName(QStringLiteral("actionExport_model"));
    QObject::connect(actionExport_model,SIGNAL(triggered()),this,SLOT(ExportModelButtonPressed()));

    actionExport_blend_shapes = new QAction(MainWindow);
    actionExport_blend_shapes->setObjectName(QStringLiteral("actionExport_blend_shapes"));
    QObject::connect(actionExport_blend_shapes,SIGNAL(triggered()),this,SLOT(ExportBsButtonPressed()));

    actionExport_all = new QAction(MainWindow);
    actionExport_all->setObjectName(QStringLiteral("actionExport_all"));
    QObject::connect(actionExport_all,SIGNAL(triggered()),this,SLOT(ExportAllButtonPressed()));

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

    DrawButton = new QPushButton(groupBox);
    DrawButton->setObjectName(QStringLiteral("DrawButton"));
    DrawButton->setGeometry(QRect(0, 0, 51, 51));
    DrawButton->setToolTip(QStringLiteral("Draw shapes to select key points"));
    DrawButton->setToolTipDuration(-1);
    QIcon DrawIcon;
    drawButtonOn = false;
    DrawIcon.addFile(QStringLiteral("Icons/DrawIconOff.png"), QSize(), QIcon::Normal, QIcon::Off);
    DrawButton->setIcon(DrawIcon);
    DrawButton->setIconSize(QSize(45, 45));
    QObject::connect(DrawButton,SIGNAL(pressed()),this,SLOT(DrawButtonPressed()));

    FaceButton = new QPushButton(groupBox);
    FaceButton->setObjectName(QStringLiteral("DlibButton"));
    FaceButton->setGeometry(QRect(0, 50, 51, 51));
    FaceButton->setToolTip(QStringLiteral("Automatically find new landmarks thanks to face detetction/recognition (l)"));
    FaceButton->setToolTipDuration(-1);
    QIcon FaceIcon;
    FaceIcon.addFile(QStringLiteral("Icons/DlibIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    FaceButton->setIcon(FaceIcon);
    FaceButton->setIconSize(QSize(45, 45));
    QObject::connect(FaceButton,SIGNAL(pressed()),this,SLOT(DlibButtonPressed()));

    ToggleVisibilityButton = new QPushButton(groupBox);
    ToggleVisibilityButton->setObjectName(QStringLiteral("ToggleVisibilityButton"));
    ToggleVisibilityButton->setGeometry(QRect(0, 100, 51, 51));
    ToggleVisibilityButton->setToolTip(QStringLiteral("Toggle aligned model visibility (v)"));
    ToggleVisibilityButton->setToolTipDuration(-1);
    QIcon ToggleVisibilityIcon;
    ToggleVisibilityIcon.addFile(QStringLiteral("Icons/ToggleVisibilityIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    ToggleVisibilityButton->setIcon(ToggleVisibilityIcon);
    ToggleVisibilityButton->setIconSize(QSize(45, 45));
    QObject::connect(ToggleVisibilityButton,SIGNAL(pressed()),this,SLOT(ToggleVisibilityButtonPressed()));

    AlignButton = new QPushButton(groupBox);
    AlignButton->setObjectName(QStringLiteral("AlignButton"));
    AlignButton->setGeometry(QRect(0, 175, 51, 51));
    AlignButton->setToolTip(QStringLiteral("Align models (t)"));
    AlignButton->setToolTipDuration(-1);
    QIcon AlignIcon;
    AlignIcon.addFile(QStringLiteral("Icons/AlignIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
    AlignButton->setIcon(AlignIcon);
    AlignButton->setIconSize(QSize(45, 45));
    QObject::connect(AlignButton,SIGNAL(pressed()),this,SLOT(AlignButtonPressed()));

    NextIterationButton = new QPushButton(groupBox);
    NextIterationButton->setObjectName(QStringLiteral("NextIterationButton"));
    NextIterationButton->setGeometry(QRect(0, 225, 51, 51));
    NextIterationButton->setToolTip(QStringLiteral("Performs a new iteration of morphing"));
    NextIterationButton->setToolTipDuration(-1);
    QIcon NewIterationIcon;
    NewIterationIcon.addFile(QStringLiteral("Icons/NewIteration.png"), QSize(), QIcon::Normal, QIcon::Off);
    NextIterationButton->setIcon(NewIterationIcon);
    NextIterationButton->setIconSize(QSize(45, 45));
    QObject::connect(NextIterationButton,SIGNAL(pressed()),this,SLOT(NewIterationButtonPressed()));

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
    menuFile->addSeparator();
    menuFile->addAction(actionLoad_keypoints);
    menuFile->addAction(actionSave_keypoints);
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
    actionSave_keypoints->setText(QApplication::translate("MainWindow", "Save keypoints", 0));
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

void UserInterface::SaveKeypointsButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());

        std::string fileName = QFileDialog::getSaveFileName(centralwidget,
                                                            tr("Save in txt file"),NULL,
                                                            tr("Text files (*.txt)")).toStdString();

        if(!fileName.empty())
        {
            interactor->SaveKeypoints(fileName);
        }
    }
}

void UserInterface::ExportModelButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        std::string fileName = QFileDialog::getSaveFileName(centralwidget,
                                                            tr("Save model as obj"),
                                                            NULL,
                                                            tr("OBJ Files (*.obj)")).toStdString();
        if(!fileName.empty())
        {
            interactor->ExportModel(fileName);
        }
    }
}

void UserInterface::ExportBsButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        std::string folder = QFileDialog::getExistingDirectory(centralwidget,
                                                            tr("Choose folder for blend shapes"),
                                                            NULL,
                                                            NULL).toStdString();
        if(!folder.empty())
        {
            interactor->ExportBlendshapes(folder+"/");
        }
    }
}

void UserInterface::ExportAllButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        std::string folder = QFileDialog::getExistingDirectory(centralwidget,
                                                            tr("Choose folder"),
                                                            NULL,
                                                            NULL).toStdString();
        if(!folder.empty())
        {
            interactor->ExportModelAndBS(folder+"/");
        }
    }
}

void UserInterface::DrawButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->ToggleDrawMode(DrawButton);
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

void UserInterface::ToggleVisibilityButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->ToggleAlignedVisibility();
    }
}

void UserInterface::AlignButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->AlignAndMorph();
    }
}

void UserInterface::NewIterationButtonPressed()
{
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->NewIteration();
    }
}

