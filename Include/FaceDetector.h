/* Project FaceMorphing by Bruno GREGOIRE
 * Master Thesis / Politecnico di Torino
 *
 * FaceDetector.h
 *
 * Detects faces in a vtk render window.
 * Automaticaly find keypoints and
 * project them on the face.
 *
 */

#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include "vtkIncludes.h"
#include "qtIncludes.h"

#include "vtkModel.h"
#include "vtkMeshModel.h"
#include "vtkGlyphModel.h"
#include "vtkLineModel.h"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/geometry/vector.h>

class FaceDetector
{
public:
    std::string shapePredictorPath;

    dlib::frontal_face_detector detector;
    dlib::shape_predictor sp;

    double imgHeight;

    int nbIgnoredPoints;
    int* toIgnore;

    FaceDetector(std::string spPath);

    void DetectAnProject(vtkGlyphModel* glyphModel);
    std::string Screenshot(vtkSmartPointer<vtkRenderWindow> renderWindow);
    std::vector<double*> ConvertToScreenCoordinates(std::vector<dlib::full_object_detection> shapes);
    std::vector<double*> FlipPoints(std::vector<double*> sourcePoints);
    std::vector<double*> DeleteIgnoredPoints(std::vector<double*> sourcePoints);
    void ProjectKeypoints(vtkGlyphModel* glyphModel,std::vector<double*> points);
};

#endif
