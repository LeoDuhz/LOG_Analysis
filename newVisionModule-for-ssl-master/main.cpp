#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "interaction.h"
#include "parammanager.h"
#include "cvision.h"
//#include "pluginmanager.h"
#include "field.h"
#include "actionmodule.h"
void qmlRegister(){
    qmlRegisterType<Interaction>("Li", 1, 0, "Interaction");
    qmlRegisterType<Field>("Li", 1, 0, "Field");
}
int main(int argc, char *argv[])
{
   // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication w(argc, argv);
    qmlRegister();
//    CactionModule::instance();
//    CactionModule::instance()->connectRadio(0, 8);
//    while(true) {
//        CactionModule::instance()->crazy(0,5,CVector(20,0),0,0,0,0);
//    }
    Datastore dataget;
//    Rec_loader log_loader;
    CvisionModule::instance();
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("main.qml"));
    engine.load(url);
    CvisionModule::instance()->SetBallType(true);
    dataget.link(&(*CvisionModule::instance()),"imgdata");
    std::cout<<"link successfully"<<std::endl;
//    std::thread ta([&]{log_loader.run();});
    std::thread tb([&]{CvisionModule::instance()->run();});
    qDebug() << "send crazy!!!";

    auto res = w.exec();
    //tb.join();
    return res;
}

//int main(){
//    A a;
//    B b;
//    a.link(&b,"mes");
//    a.start_detach();
//    b.start_detach();
//    while(true){
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    }
//}
