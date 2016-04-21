/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * Initializer.h
 *
 * Initializes the main objects of the project
 * Used at the begining to create the scene and import the models
 *
 */

#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "vtkIncludes.h"

#include "vtkModel.h"
#include "Content.h"
#include "InteractorStyle.h"
#include "ICP.h"
#include "TPST.h"
#include "Texturer.h"
#include "Raycaster.h"
#include "FaceDetector.h"

class Initializer
{
public:
    Content* content;
    ICP* icp;
    TPST* tpst;
    Texturer* texturer;
    Raycaster* raycaster;
    FaceDetector* faceDetector;

    bool newmodelImported;

    Initializer();

    void Init();
    void InitNewModel(std::string fileName);
    void InitNewModelTexture(std::string fileName);
};

#endif
