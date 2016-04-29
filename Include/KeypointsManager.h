/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * KeypointsManager.h
 *
 */

#ifndef KEYPOINTSMANAGER_H
#define KEYPOINTSMANAGER_H

#include "vtkIncludes.h"

class KeypointsManager
{
public:
    static int GetSymmetric(int keypoint);
    //int GetArea(int keypoint);
    //std::vector<int> GetKeypoints (int shape);
};

#endif
