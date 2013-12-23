#include <QApplication>
#include "qtquick1applicationviewer.h"
#include <QDeclarativeEngine>
#include <QtDeclarative>
#include <QCoreApplication>
//#include <QQmlComponent>
//#include <QQmlContext>
#include <QScreen>
#include <QObject>
//#include <QQuickItem>
#include <QMetaType>

#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>
#include <new>

#include "src/cmain.h"
#include "src/userinfo.h"
#include "src/ticketinfo.h"
#include "src/menuinfo.h"
#include "src/reports.h"
#include "src/settings.h"
/*
static QObject* cmain_provider(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    CMain* cmain = new CMain();
    return cmain;
}

static QObject* userinfo_provider(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    UserInfo* UInfo = new UserInfo();
    return UInfo;
}

static QObject* ticketinfo_provider(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    TicketInfo* info = new TicketInfo();
    return info;
}

static QObject* menuinfo_provider(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    MenuInfo* info = new MenuInfo();
    return info;
}

static QObject* reports_provider(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    Reports* reports = new Reports();
    return reports;
}

static QObject* settings_provider(QQmlEngine* UNUSED(engine), QJSEngine* UNUSED(scriptEngine))
{
    return new Settings();
}
*/
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    /*QRect geometry = (app.screens()[0])->geometry();
    Data::Instance().SetScreenHeight(geometry.height());
    Data::Instance().SetScreenWidth(geometry.width());
*/

    qmlRegisterType<UserInfo>("UserInfo", 1, 0, "UserInfo");//, QString("undefined") );
    QtQuick1ApplicationViewer viewer;
    qmlRegisterType<CMain>("Qt.Cmain", 1, 0, "CMain");
    qmlRegisterType<TicketInfo>("TicketInfo", 1, 0, "TicketInfo");
    qmlRegisterType<MenuInfo>("MenuInfo", 1, 0, "MenuInfo");
    qmlRegisterType<Reports>("Reports", 1, 0, "Reports");
    qmlRegisterType<Settings>("Settings", 1, 0, "Settings");
    qmlRegisterType<ListModel>("Qt.Cmain", 1, 0, "ListModel");
 /*   qmlRegisterSingletonType<CMain>("Qt.Cmain", 1, 0, "CMain", cmain_provider);
    qmlRegisterSingletonType<TicketInfo>("TicketInfo", 1, 0, "TicketInfo", ticketinfo_provider);
    qmlRegisterSingletonType<MenuInfo>("MenuInfo", 1, 0, "MenuInfo", menuinfo_provider);
    qmlRegisterSingletonType<UserInfo>("UserInfo", 1, 0, "UserInfo", userinfo_provider);
    qmlRegisterSingletonType<Reports>("Reports", 1, 0, "Reports", reports_provider);
    qmlRegisterSingletonType<Settings>("Settings", 1, 0, "Settings", settings_provider);
*/
    viewer.setMainQmlFile(QLatin1String("qml/posqt/main.qml"));
    //viewer.setClearBeforeRendering(false);
    //QSurfaceFormat format = viewer.format();
    //format.setSamples(3);//Buffers(false);
    //format.setStereo(true);
    //format.setOption(QSurfaceFormat::StereoBuffers);//Context);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    //format.setRenderableType(QSurfaceFormat::OpenGL);
    //format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    //viewer.setFormat(format);
    viewer.showFullScreen();

    return app.exec();
}
