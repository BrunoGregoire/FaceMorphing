/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * Matcher.h
 *
 * This class exist to perfectly match
 * two models by finding the clossest vertices
 * and interpolate between their positions
 *
 */

#ifndef MATCHER_H
#define MATCHER_H

#include "vtkIncludes.h"

#include "vtkMeshModel.h"

class Matcher
{
public:

    static void MatchPerfectly(vtkMeshModel* firstModel, vtkMeshModel* secondMode, double tolerance);
    static void MatchPerfectly(vtkMeshModel* firstModel, vtkMeshModel* secondModel);
    static void MatchInterpolated(vtkMeshModel* firstModel, vtkMeshModel* secondModel);
    static void MatchWithCurvatures(vtkMeshModel* firstModel, vtkMeshModel* secondModel);
};

#endif
