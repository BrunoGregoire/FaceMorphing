/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * InteractorStyle.h
 *
 * Manages the interactions of the user
 * Reacts to user input (mouse, keyboards etc.)
 *
 */

#ifndef INTERACTORSTYLE_H
#define INTERACTORSTYLE_H

#include "vtkIncludes.h"

#include "Content.h"
#include "vtkGlyphModel.h"
#include "ICP.h"
#include "TPST.h"
#include "Texturer.h"
#include "Raycaster.h"
#include "FaceDetector.h"
#include "Drawer.h"
#include "KeypointsManager.h"
#include "Cutter.h"

class InteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static InteractorStyle* New();
    vtkTypeMacro(InteractorStyle, vtkInteractorStyleTrackballCamera);

    Content* content;
    ICP* icp;
    bool icpDone;
    TPST* tpst;
    bool tpstReady;
    Texturer* texturer;
    Raycaster* raycaster;
    FaceDetector* faceDetector;
    Drawer* drawer;
    bool drawMode;
    Cutter* cutter;

    bool isDrawing;
    double* drawOrigin;
    double* drawEnd;

    vtkSmartPointer<vtkCellPicker> picker;
    vtkGlyphModel* glyph;
    bool glyphSelected;
    int glyphIndex;

    bool symetricMode;
    vtkGlyphModel* symGlyph;
    bool symGlyphSelected;
    int symGlyphIndex;
    double* symPosition;


    vtk2DModel* currentShape;
    int currentArea;

    InteractorStyle();

    void SetContent(Content* _content);
    void SetICP(ICP* _icp);
    void SetTPST(TPST* _tpst);
    void SetTexturer (Texturer* _texturer);
    void SetRaycaster (Raycaster* _raycaster);
    void SetFaceDetector (FaceDetector* _faceDetector);
    void SetDrawer (Drawer* _drawer);
    void SetCutter (Cutter* _cutter);

    void OnLeftButtonDown();
    void OnRightButtonDown();
    void OnMouseMove();
    void OnKeyPress();

    void ExportBlendshapes(std::string folderPath);
    void LoadKeypoints(std::string path);
    void SaveKeypoints(std::string path);
    void ExportModel(std::string path);
    void ExportModelAndBS(std::string folderPath);
    void UpdateText();
    void DoAll();
    void AlignModels();
    void ToggleAlignedVisibility();
    void MorphModels();
    void TextureModels();
    void RaycastHead();
    void RaycastHead(int precision);
    void CylinderRaycast();
    void CylinderRaycast(int precision);
    void FaceDetection();
    void ResetCameras();
    void ToggleDrawMode();
    void CancelDrawing();
    void DrawCircle();
    void InitCutPlane();
    void CutPlaneUp();
    void CutPlaneDown();
    void Cut();

};

#endif
