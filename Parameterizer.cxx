#include "Parameterizer.h"

vtkMeshModel* Parameterizer::PlanarParameterization(vtkMeshModel *mesh)
{
    std::cout << "DEBUG 0" << std::endl;
    // ** TYPEDEF **
    typedef double Coord;

    typedef itk::QuadEdgeMesh< Coord, 3 >                           MeshType;
    typedef itk::VTKPolyDataReader< MeshType >                      ReaderType;
    typedef itk::VTKPolyDataWriter< MeshType >                      WriterType;
    typedef itk::BorderQuadEdgeMeshFilter< MeshType, MeshType >     BorderTransformType;
    typedef VNLIterativeSparseSolverTraits< Coord >                 SolverTraits;

    typedef itk::ParameterizationQuadEdgeMeshFilter< MeshType, MeshType, SolverTraits >   ParametrizationType;

    std::cout << "DEBUG 1" << std::endl;
    std::string inputFileName = "inputFile.vtk";

    vtkSmartPointer<vtkPolyDataWriter> vtkWriter = vtkSmartPointer<vtkPolyDataWriter>::New();
    vtkWriter->SetFileName(inputFileName.c_str());
    vtkWriter->SetInputData(mesh->polyData);
    vtkWriter->Write();
    std::cout << "DEBUG 2" << std::endl;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFileName.c_str());
    try
    {
        reader->Update( );
    }
    catch( itk::ExceptionObject & excp )
    {
        std::cerr << "Exception thrown while reading the input file " << std::endl;
        std::cerr << excp << std::endl;
        return NULL;
    }
    std::cout << "DEBUG 2" << std::endl;

    MeshType::Pointer itkMesh = reader->GetOutput();
    std::cout << "DEBUG 2" << std::endl;

    /*
    std::cout << "DEBUG 2" << std::endl;
    // ** CONVERT VTK POLYDATA TO ITK MESH **
    vtkPolyDataToitkMesh* converter = new vtkPolyDataToitkMesh();
    converter->SetInput(mesh->polyData);
    converter->ConvertvtkToitk();
    MeshType::Pointer itkMesh = (MeshType*)converter->GetOutput();
    */

    std::cout << "DEBUG 3" << std::endl;
    // ** CHOSE< COMPUTE AND SET BORDER TRANSFORM **
    BorderTransformType::Pointer border_transform = BorderTransformType::New( );
    border_transform->SetInput( itkMesh );
    // two following line for coverage
    border_transform->SetRadius( border_transform->GetRadius() );

    std::cout << "DEBUG 4" << std::endl;
    int border = 1;
    switch( border )  // choose border type
    {
    case 0: // square shaped domain
        border_transform->SetTransformType( BorderTransformType::SQUARE_BORDER_TRANSFORM );
        break;
    case 1: // disk shaped domain
        border_transform->SetTransformType( BorderTransformType::DISK_BORDER_TRANSFORM );
        break;
    }
    std::cout << "Transform type is: " << border_transform->GetTransformType( );
    std::cout << std::endl;

    std::cout << "DEBUG 5" << std::endl;
    // ** CHOOSE AND SET BARYCENTRIC WEIGHTS **
    itk::OnesMatrixCoefficients< MeshType >                     coeff0;
    itk::InverseEuclideanDistanceMatrixCoefficients< MeshType > coeff1;
    itk::ConformalMatrixCoefficients< MeshType >                coeff2;
    itk::AuthalicMatrixCoefficients< MeshType >                 coeff3;
    itk::HarmonicMatrixCoefficients< MeshType >                 coeff4;

    ParametrizationType::Pointer param = ParametrizationType::New( );
    param->SetInput( itkMesh );
    param->SetBorderTransform( border_transform );

    int param_type = 1;
    switch( param_type )
    {
    case 0:
        param->SetCoefficientsMethod( &coeff0 );
        break;
    case 1:
        param->SetCoefficientsMethod( &coeff1 );
        break;
    case 2:
        param->SetCoefficientsMethod( &coeff2 );
        break;
    case 3:
        param->SetCoefficientsMethod( &coeff3 );
        break;
    case 4:
        param->SetCoefficientsMethod( &coeff4 );
        break;
    }
    std::cout << "DEBUG 6" << std::endl;
    // ** PROCESS **
    param->Update( );
    std::cout << "DEBUG 6 bis" << std::endl;
    MeshType::Pointer output = param->GetOutput( );

    std::cout << "DEBUG 7" << std::endl;
    // ** PRINT **
    std::cout << "BorderTransform: \n" << border_transform;
    std::cout << "Parametrization: \n" << param;

    // ** CREATE OUTPUT OBJECT **
    /*
    vtkMeshModel* outputMesh = new vtkMeshModel();
    itkMeshTovtkPolyData* outputConverter = new itkMeshTovtkPolyData();
    outputConverter->SetInput(output);
    outputConverter->ConvertitkTovtk();
    outputMesh->polyData = outputConverter->GetOutput();
    outputMesh->Update();*/

    std::cout << "DEBUG 8" << std::endl;
    // ** WRITE OUTPUT **
    std::string outputFileName = "outputFile.vtk";
    WriterType::Pointer writer = WriterType::New( );
    writer->SetInput(param->GetOutput());
    writer->SetFileName(outputFileName.c_str());
    writer->Update();

    std::cout << "DEBUG 9" << std::endl;
    vtkSmartPointer<vtkPolyDataReader> vtkReader = vtkSmartPointer<vtkPolyDataReader>::New();
    vtkReader->SetFileName(outputFileName.c_str());
    vtkReader->Update();

    std::cout << "DEBUG 10" << std::endl;
    vtkMeshModel* outputMesh = new vtkMeshModel();
    outputMesh->polyData = vtkReader->GetOutput();
    outputMesh->Update();

    return outputMesh;
}
