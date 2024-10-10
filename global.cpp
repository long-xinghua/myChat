#include "global.h"

std::function<void(QWidget*)> refresh = [](QWidget* w){
    w->style()->unpolish(w);    // 把控件原来的样式卸掉
    w->style()->polish(w);      // 重新装上新的样式
};

QString gate_url_prefix = "";
