/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * UserInterface.h
 *
 * Creates and manages the UI
 * Links the buttons of the UI (qt)
 * to the InteractorStyle (vtk)
 *
 */

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "qtIncludes.h"

#include "Initializer.h"

class UserInterface : QObject
{
    Q_OBJECT

public:
    Initializer* initializer;

    QAction *actionImport;
    QAction *actionImport_texture;
    QAction *actionImport_all;
    QAction *actionLoad_keypoints;
    QAction *actionSave_keypoints;
    QAction *actionExport_model;
    QAction *actionExport_blend_shapes;
    QAction *actionExport_all;

    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;

    QGroupBox *groupBox;
    bool drawButtonOn;

    QPushButton *DrawButton;
    QPushButton *FaceButton;
    QPushButton *ToggleVisibilityButton;

    QPushButton *AlignButton;
    QPushButton *NextIterationButton;

    QVTKWidget *RefQvtkWidget;
    QVTKWidget *NewmodelQvtkWidget;

    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow);
    void retranslateUi(QMainWindow *MainWindow);

public slots:

    void ImportButtonPressed();
    void TextureButtonPressed();
    void ImportAllButtonPressed();
    void LoadKeypointsButtonPressed();
    void SaveKeypointsButtonPressed();
    void ExportModelButtonPressed();
    void ExportBsButtonPressed();
    void ExportAllButtonPressed();

    void DrawButtonPressed();
    void DlibButtonPressed();
    void ToggleVisibilityButtonPressed();

    void AlignButtonPressed();
    void NewIterationButtonPressed();
};
#endif


