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
#include "qtIncludes.h"

#include "vtkModel.h"
#include "Content.h"
#include "RefInteractorStyle.h"
#include "InteractorStyle.h"
#include "ICP.h"
#include "TPST.h"
#include "Texturer.h"
#include "Raycaster.h"
#include "FaceDetector.h"
#include "Drawer.h"
#include "Cutter.h"

class Initializer
{
public:
    Content* content;
    ICP* icp;
    TPST* tpst;
    Texturer* texturer;
    Raycaster* raycaster;
    FaceDetector* faceDetector;
    Drawer* drawer;
    Cutter* cutter;

    // Parameters
    int* raycastLevel;
    bool* eyesAndJaw;
    bool* symMode;

    bool newmodelImported;

    std::string newmodelFileName;
    std::string newmodelTextureFileName;

    Initializer();

    void Init();
    void InitNewModel(std::string fileName);
    void InitNewModelTexture(std::string fileName);

    void Reset();
};

#endif
