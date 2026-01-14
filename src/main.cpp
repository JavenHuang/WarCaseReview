#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("战例复盘分析系统");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("WarCaseReview");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}

