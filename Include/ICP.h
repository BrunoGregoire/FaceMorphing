/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * ICP.h
 *
 * Performs the alignement of the new model to the reference
 * and then the reference to the new aligned model
 * Uses the algorithm : Iterative Closest Point Transform
 *
 */

#ifndef ICP_H
#define ICP_H

#include "vtkIncludes.h"

#include "vtkModel.h"
#include "vtkMeshModel.h"
#include "vtkGlyphModel.h"
#include "vtkLineModel.h"

class ICP
{
private:
    vtkSmartPointer<vtkMatrix4x4> ComputeTransMat(vtkGlyphModel* sourceKeypoints,vtkGlyphModel* targetKeypoints, int mode);

public:
	vtkSmartPointer<vtkIterativeClosestPointTransform> icp;
    vtkSmartPointer<vtkMatrix4x4> transMat;

    ICP();

    void Align(vtkGlyphModel* sourceKeypoints,vtkGlyphModel* targetKeypoints, int mode);
    vtkGlyphModel* AlignAndCopy(vtkGlyphModel* sourceKeypoints,vtkGlyphModel* targetKeypoints, int mode);


};

#endif
