/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * Parameterizer.h
 *
 * This class performs parameterization of a mesh
 * thanks to ITK
 *
 */

#ifndef PARAMETERIZER_H
#define PARAMETERIZER_H

#include "itkVTKPolyDataReader.h"
#include "itkVTKPolyDataWriter.h"
#include "VNLIterativeSparseSolverTraits.h"
#include "itkParameterizationQuadEdgeMeshFilter.h"

#include "vtkPolyDataToitkMesh.h"
#include "itkMeshTovtkPolyData.h"

#include "vtkIncludes.h"

#include "vtkMeshModel.h"


class Parameterizer
{
public:
    static vtkMeshModel* PlanarParameterization(vtkMeshModel* mesh);
};

#endif
