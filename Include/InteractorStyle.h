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

    vtkSmartPointer<vtkCellPicker> picker;
    vtkGlyphModel* glyph;

    bool glyphSelected;
    int glyphIndex;

    InteractorStyle();

    void SetContent(Content* _content);
    void SetICP(ICP* _icp);
    void SetTPST(TPST* _tpst);
    void SetTexturer (Texturer* _texturer);
    void SetRaycaster (Raycaster* _raycaster);
    void SetFaceDetector (FaceDetector* _faceDetector);

    //void OnRightButtonDown();
    void OnMiddleButtonDown();
    void OnMouseMove();
    void OnKeyPress();

    //void SaveKeypoint();
    void ExportKeypoints();
    void LoadKeypoints(std::string path);
    void UpdateText();
    void DoAll();
    void AlignModels();
    void ToggleAlignedVisibility();
    void MorphModels();
    void TextureModels();
    void ComputeBlendshapes();
    void RaycastHead();
    void CylinderRaycast();
    void FaceDetection();
    void SmoothMorphedModel();
    void CutNewmodel();
    void TakeScreenshot(vtkSmartPointer<vtkRenderWindow> renderWindow,std::string imgPath);
    void ResetCameras();
};

#endif
