/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * StateManager.h
 *
 * Manages the undo/redo
 *
 */

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "vtkIncludes.h"

#include "Content.h"
#include "vtkMeshModel.h"
#include "vtkGlyphModel.h"

class StateManager
{
public:
    Content* content;

    vtkSmartPointer<vtkPolyData> lastRefPolydata;
    std::vector<vtkSmartPointer<vtkPolyData>> lastBlendshapes;

    vtkSmartPointer<vtkIdList> lastRefIds;
    vtkSmartPointer<vtkPoints> lastRefPoints;

    vtkSmartPointer<vtkIdList> lastNewmodelIds;
    vtkSmartPointer<vtkPoints> lastNewmodelPoints;

    vtkSmartPointer<vtkIdList> lastAlignedIds;
    vtkSmartPointer<vtkPoints> lastAlignedPoints;

    bool dirty;

    StateManager(Content* _content);

    void SaveState();
    void RestoreState();

};

#endif
