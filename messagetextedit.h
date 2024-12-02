#ifndef MESSAGETEXTEDIT_H
#define MESSAGETEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMimeType>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QPainter>
#include <QVector>
#include "global.h"

/***********************************************
    * @file     messagetextedit.h
    * @brief    自定义消息输入框的类，预留一些外部接口实现更复杂的功能,借鉴的别人的代码
    *
    * @author   wmlatiao
    * @date     2024/11/28
    * @history
***********************************************/

class MessageTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MessageTextEdit(QWidget *parent = nullptr);

    ~MessageTextEdit();

    QVector<MsgInfo> getMsgList();

    void insertFileFromUrl(const QStringList &urls);    // 使用url插入文件
signals:
    void send();

protected:
    void dragEnterEvent(QDragEnterEvent *event);    // 把文件拽进来的处理
    void dropEvent(QDropEvent *event);              // 把文件丢在这里的处理
    void keyPressEvent(QKeyEvent *e);               // 按下回车的处理

private:
    void insertImages(const QString &url);
    void insertTextFile(const QString &url);                    // 插入文本
    bool canInsertFromMimeData(const QMimeData *source) const;  // 判断多媒体数据能不能插入到textEdit里
    void insertFromMimeData(const QMimeData *source);           // 插入多媒体资源

private:
    bool isImage(QString url);//判断文件是否为图片
    void insertMsgList(QVector<MsgInfo> &list,QString flag, QString text, QPixmap pix); // 把消息插入消息列表

    QStringList getUrl(QString text);
    QPixmap getFileIconPixmap(const QString &url);//获取文件图标及大小信息，并转化成图片
    QString getFileSize(qint64 size);//获取文件大小

private slots:
    void textEditChanged();

private:
    QVector<MsgInfo> mMsgList;      // 管理消息的列表
    QVector<MsgInfo> mGetMsgList;   // 获取消息的列表
};

#endif // MESSAGETEXTEDIT_H
