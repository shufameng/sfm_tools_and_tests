#include "tatwindow.h"

#include "tatapp.h"

int main(int argc, char *argv[])
{
    TATApp qtApp(argc, argv);
    TATWindow w;
    w.show();

    return qtApp.exec();
}
