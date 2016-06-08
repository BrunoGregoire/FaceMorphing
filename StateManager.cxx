#include "StateManager.h"

StateManager::StateManager(Content* _content)
{
    content = _content;

    lastRefPolydata = vtkSmartPointer<vtkPolyData>::New();

    for(int i=0;i<content->refModel->blendshapes.size();i++)
    {
        vtkSmartPointer<vtkPolyData> newPolydata = vtkSmartPointer<vtkPolyData>::New();
        lastBlendshapes.push_back(newPolydata);
    }

    lastRefIds = vtkSmartPointer<vtkIdList>::New();
    lastRefPoints = vtkSmartPointer<vtkPoints>::New();

    lastNewmodelIds = vtkSmartPointer<vtkIdList>::New();
    lastNewmodelPoints = vtkSmartPointer<vtkPoints>::New();

    lastAlignedIds = vtkSmartPointer<vtkIdList>::New();
    lastAlignedPoints = vtkSmartPointer<vtkPoints>::New();

    dirty == false;
}

void StateManager::SaveState()
{
    lastRefPolydata->DeepCopy(content->refModel->polyData);

    for(int i=0;i<content->refModel->blendshapes.size();i++)
        lastBlendshapes[i]->DeepCopy(content->refModel->blendshapes[i]);

    lastRefIds->DeepCopy(content->refKeypoints->pointsIds);
    lastRefPoints->DeepCopy(content->refKeypoints->points);

    lastNewmodelIds->DeepCopy(content->newmodelKeypoints->pointsIds);
    lastNewmodelPoints->DeepCopy(content->newmodelKeypoints->points);

    if(content->alignedKeypoints != NULL)
    {
        lastAlignedIds->DeepCopy(content->alignedKeypoints->pointsIds);
        lastAlignedPoints->DeepCopy(content->alignedKeypoints->points);
    }

    dirty = true;
}

void StateManager::RestoreState()
{
    if(dirty)
    {
        content->refModel->polyData->DeepCopy(lastRefPolydata);
        content->refModel->Update();

        for(int i=0;i<content->refModel->blendshapes.size();i++)
            content->refModel->blendshapes[i]->DeepCopy(lastBlendshapes[i]);

        content->refKeypoints->pointsIds->DeepCopy(lastRefIds);
        content->refKeypoints->points->DeepCopy(lastRefPoints);
        content->refKeypoints->UpdateGlyph();
        content->refKeypoints->UpdateIdLabels();
        if(!content->refKeypoints->visible)
            content->refKeypoints->HideIdLabels();

        content->newmodelKeypoints->pointsIds->DeepCopy(lastNewmodelIds);
        content->newmodelKeypoints->points->DeepCopy(lastNewmodelPoints);
        content->newmodelKeypoints->UpdateGlyph();
        content->newmodelKeypoints->UpdateIdLabels();
        if(!content->newmodelKeypoints->visible)
            content->newmodelKeypoints->HideIdLabels();

        if(content->alignedKeypoints != NULL)
        {
            content->alignedKeypoints->pointsIds->DeepCopy(lastAlignedIds);
            content->alignedKeypoints->points->DeepCopy(lastAlignedPoints);
            content->alignedKeypoints->UpdateGlyph();
            content->alignedKeypoints->UpdateIdLabels();
            if(!content->alignedKeypoints->visible)
                content->alignedKeypoints->HideIdLabels();
        }
    }
    dirty = false;
}
