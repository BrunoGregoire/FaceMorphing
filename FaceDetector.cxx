#include "FaceDetector.h"

FaceDetector::FaceDetector(std::string spPath)
{
    shapePredictorPath = spPath;

    // Init the face detector and shape predictor
    detector = dlib::get_frontal_face_detector();
    dlib::deserialize(shapePredictorPath) >> sp;

    // Ignore some points of the mouth
    nbIgnoredPoints = 8;
    toIgnore = new int [nbIgnoredPoints];
    toIgnore[0] = 60;
    toIgnore[1] = 61;
    toIgnore[2] = 62;
    toIgnore[3] = 63;
    toIgnore[4] = 64;
    toIgnore[5] = 65;
    toIgnore[6] = 66;
    toIgnore[7] = 67;
}

void FaceDetector::DetectAnProject(vtkGlyphModel *glyphModel)
{
    std::string imgPath = Screenshot(glyphModel->renderWindow);
    dlib::array2d<dlib::rgb_pixel> img;
    dlib::load_image(img, imgPath);
    imgHeight = img.nr();

    std::vector<dlib::rectangle> dets = detector(img);

    // Get the key points
    /*  0-16 : cheeks
     *  17-21 : right eyebrow
     *  22-26 : left eyebrow
     *  27-35 : nose
     *  36-41 : right eye
     *  42-47 : left eye
     *  48-67 : mouth
     */
    std::vector<dlib::full_object_detection> shapes;
    for (unsigned long j = 0; j < dets.size(); ++j)
    {
        dlib::full_object_detection shape = sp(img, dets[j]);
        shapes.push_back(shape);
    }

    std::vector<double*> screenPoints;
    screenPoints = ConvertToScreenCoordinates(shapes);
    screenPoints = FlipPoints(screenPoints);
    screenPoints = DeleteIgnoredPoints(screenPoints);

    ProjectKeypoints(glyphModel,screenPoints);

}

std::string FaceDetector::Screenshot(vtkSmartPointer<vtkRenderWindow> renderWindow)
{
    std::string imgPath = "Resources/screenshot.bmp";

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(renderWindow);
    windowToImageFilter->Update();

    vtkSmartPointer<vtkBMPWriter> writer = vtkSmartPointer<vtkBMPWriter>::New();
    writer->SetFileName(imgPath.c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();
    writer->Update();

    return imgPath;
}

std::vector<double*> FaceDetector::ConvertToScreenCoordinates(std::vector<dlib::full_object_detection> shapes)
{
    std::vector<double*> screenPoints;

    for(int j=0;j<shapes.size();j++)
    {
        dlib::full_object_detection shape = shapes[j];

        for(int i=0;i<shape.num_parts();i++)
        {
            double* newpoint = new double[3];
            newpoint[0] = shape.part(i).x();
            newpoint[1] = shape.part(i).y();
            newpoint[2] = 0;
            screenPoints.push_back(newpoint);
        }
    }

    return screenPoints;
}

std::vector<double*> FaceDetector::FlipPoints(std::vector<double*> sourcePoints)
{
    for(int i=0;i<sourcePoints.size();i++)
    {
        sourcePoints[i][1] = imgHeight - sourcePoints[i][1];
    }

    return sourcePoints;
}

std::vector<double*> FaceDetector::DeleteIgnoredPoints(std::vector<double *> sourcePoints)
{
    std::vector<double*> result = sourcePoints;

    if(sourcePoints.size()>0)
    for(int i=0;i<nbIgnoredPoints;i++)
        result.erase(result.begin()+toIgnore[i]-i);

    return result;
}

void FaceDetector::ProjectKeypoints(vtkGlyphModel *glyphModel, std::vector<double *> points)
{
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();

    for(int i=0;i<points.size();i++)
    {
        picker->Pick(points[i][0],points[i][1],0,glyphModel->renderer);
        glyphModel->InsertNextPoint(picker->GetPickPosition());
    }

    glyphModel->UpdatePointsIDs();
    glyphModel->UpdatePoints();
    glyphModel->UpdateGlyph();
    glyphModel->ShowModel();
}

