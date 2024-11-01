#include "global.h"

std::function<void(QWidget*)> refresh = [](QWidget* w){
    w->style()->unpolish(w);    // 把控件原来的样式卸掉
    w->style()->polish(w);      // 重新装上新的样式
};

std::function<QString(QString)> xorString = [](QString input){
    QString result = input;
    int size = input.size()%255;  // 一个字符就8位(一字节)，size超过255就超过8位了
    for(int i=0;i<size;i++){
        // 将字符串里的字符转成ushort后进行异或操作，再转为QChar类型给result[i]
        result[i] = QChar(static_cast<ushort>(input[i].unicode())^static_cast<ushort>(size));
    }
    return result;
};

QString gate_url_prefix = "";
