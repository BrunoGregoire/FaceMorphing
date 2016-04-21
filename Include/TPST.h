/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * TPST.h
 *
 * Performs the morphing of a model into another,
 * based on landmarks previously marked on each model.
 * Uses the algorithm:
 * Thin Plate Spline Transform
 *
 */

#ifndef TPST_H
#define TPST_H

#include "vtkIncludes.h"

#include "vtkModel.h"
#include "vtkMeshModel.h"
#include "vtkGlyphModel.h"
#include "vtkLineModel.h"

class TPST
{
public:
    enum Basis{R,R2LogR};
    Basis basis;

    vtkSmartPointer<vtkThinPlateSplineTransform> tpst;

    TPST();

    void Morph(vtkGlyphModel* sourceKeypoints,vtkGlyphModel* targetKeypoints);
};

#endif
