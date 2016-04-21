/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * Texturer.h
 *
 * Apply the texture of a model to another.
 *
 */

#ifndef TEXTURER_H
#define TEXTURER_H

#include "vtkIncludes.h"

#include "vtkModel.h"
#include "vtkMeshModel.h"
#include "vtkGlyphModel.h"
#include "vtkLineModel.h"

class Texturer
{
public:


    void Texture(vtkMeshModel* source, vtkMeshModel* target);
};

#endif
