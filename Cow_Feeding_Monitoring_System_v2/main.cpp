#include "cow_feeding_monitoring.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cow_Feeding_Monitoring w;
    w.show();

    return a.exec();
}
