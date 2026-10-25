#include <QApplication>

#include "formwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FormWindow window;
    window.show();

    return app.exec();
}
