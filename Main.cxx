#include <list>
#include <string>
#include <cmath>

#include "vtkIncludes.h"
#include "qtIncludes.h"

#include "Initializer.h"
#include "UserInterface.h"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc,argv);

    Initializer* initializer = new Initializer();
    initializer->Init();

    QMainWindow* window = new QMainWindow;
    UserInterface* ui = new UserInterface;
    ui->initializer = initializer;
    ui->setupUi(window);

    window->show();

    ui->RefQvtkWidget->SetRenderWindow(initializer->content->refRenderWindow);
    ui->RefQvtkWidget->GetRenderWindow()->Render();

    return app.exec();

    return EXIT_SUCCESS;
}




