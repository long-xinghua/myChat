#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 用.qrc里的.qss文件来个性化设计
    QFile qss(":/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug("Open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("Open .qss failed");
    }

    QString fileName = "config.ini";
    QString app_path = QCoreApplication::applicationDirPath();
    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() + fileName);    // 记录配置文件的位置，QDir::separator()就是分隔符，自动识别，在linux中为“/”，在windows中为“\”
    QSettings settings(config_path, QSettings::IniFormat);  //把config.ini中的配置读到settings中，QSettings::IniFormat指定这是个.ini文件
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;  // global中定义的gate_url_prefix（即http://localhost:8080）

    MainWindow w;
    w.show();

    return a.exec();
}
