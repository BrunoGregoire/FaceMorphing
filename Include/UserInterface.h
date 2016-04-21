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
    QAction *actionExport_model;
    QAction *actionExport_blend_shapes;
    QAction *actionExport_all;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    //QPushButton *SavePointButton;
    QPushButton *AllButton;
    QPushButton *AlignButton;
    QPushButton *ToggleVisibilityButton;
    QPushButton *MorphButton;
    QPushButton *DlibButton;
    QPushButton *RaycastHeadButton;
    QPushButton *RaycastButton;
    QPushButton *RetextureButton;
    QPushButton *BlendshapsButton;
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
    //void SavePointButtonPressed();
    void AlignButtonPressed();
    void ToggleVisibilityButtonPressed();
    void AllButtonPressed();
    void MorphButtonPressed();
    void DlibButtonPressed();
    void RaycastHeadButtonPressed();
    void RaycastButtonPressed();
    void RetextureButtonPressed();
    void BlendshapsButtonPressed();
};
#endif


