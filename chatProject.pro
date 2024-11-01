#-------------------------------------------------
#
# Project created by QtCreator 2024-09-24T16:42:31
#
#-------------------------------------------------

QT       += core gui network    #要用到QT的网络，加入network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatProject
TEMPLATE = app
RC_ICONS = icon.ico # 将程序icon改为自己选的icon.ico文件
DESTDIR = ./bin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    logindialog.cpp \
    registerdialog.cpp \
    global.cpp \
    httpmgr.cpp \
    timerbtn.cpp \
    clickedlabel.cpp

HEADERS += \
        mainwindow.h \
    logindialog.h \
    registerdialog.h \
    global.h \
    singleton.h \
    httpmgr.h \
    timerbtn.h \
    clickedlabel.h

FORMS += \
        mainwindow.ui \
    logindialog.ui \
    registerdialog.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    config.ini

win32:CONFIG(debug, debug | release)
{
    #指定要拷贝的文件目录为工程目录下release目录下的所有dll、lib文件，例如工程目录在D:\QT\Test
    #PWD就为D:/QT/Test，DllFile = D:/QT/Test/release/*.dll
    TargetConfig = $${PWD}/config.ini
    #将输入目录中的"/"替换为"\"
    TargetConfig = $$replace(TargetConfig, /, \\)
    #将输出目录中的"/"替换为"\"
    OutputDir =  $${OUT_PWD}/$${DESTDIR}
    OutputDir = $$replace(OutputDir, /, \\)
    #执行copy命令,将config.ini拷贝到输出文件夹
    QMAKE_POST_LINK += copy /Y \"$$TargetConfig\" \"$$OutputDir\"
}
