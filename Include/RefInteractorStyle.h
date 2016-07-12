/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * RefInteractorStyle.h
 *
 * Manages the interactions of the user for the reference window
 * Reacts to user input (mouse, keyboards etc.)
 *
 */

#ifndef REFINTERACTORSTYLE_H
#define REFINTERACTORSTYLE_H

#include "vtkIncludes.h"
#include "qtIncludes.h"

#include "Content.h"
#include "InteractorStyle.h"

class RefInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static RefInteractorStyle* New();
    vtkTypeMacro(RefInteractorStyle, vtkInteractorStyleTrackballCamera);

    Content* content;

    bool* eyesAndJaw;

    bool selectionPossible;
    vtkSmartPointer<vtkCellPicker> picker;
    vtkIdType pickedId;
    vtkGlyphModel* glyph;

    RefInteractorStyle();

    void SetContent(Content* _content);
    void SetParameters(bool* _eyesAndJaw);

    void UpdateParameters();

    void OnLeftButtonDown();
    void OnKeyPress();
    void OnMouseMove();

    void SetCurrentCursor();

    void CheckSelectionPossible();
    void SaveAddKeypoint();
    void MorphModels();

};

#endif
