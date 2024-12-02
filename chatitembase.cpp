#include "chatitembase.h"

ChatItemBase::ChatItemBase(ChatRole role, QWidget *parent) : QWidget(parent), _role(role)
{
    _nameLabel = new QLabel(this);          // 创建昵称标签
    _nameLabel->setObjectName("chatUserName");
    QFont font("Microsoft YaHei");          // 设置字体
    font.setPointSize(9);
    _nameLabel->setFont(font);
    _nameLabel->setFixedHeight(20);         // 设置固定高度

    _iconLabel = new QLabel(this);          // 创建头像标签
    _iconLabel->setObjectName("userIconLabel");
    _iconLabel->setScaledContents(true);    // 让QLabel中图像根据QLabel的大小自动缩放
    _iconLabel->setFixedSize(42,42);

    _bubble = new QWidget();    // 暂时用一个空widget占位，后面会顶掉

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setVerticalSpacing(3);      // 控件与控件之间的垂直间距
    gridLayout->setHorizontalSpacing(3);    // 控件与控件之间的垂直间距
    gridLayout->setMargin(3);

    QSpacerItem* spacer = new QSpacerItem(40,20,QSizePolicy::Expanding, QSizePolicy::Minimum);  // 创建弹簧

    // 在网格布局里排版
    gridLayout->addWidget(_nameLabel, 0,1,1,1); // 名字添加在第0行第1列，占1行1列
    gridLayout->addWidget(_bubble, 1,1,1,1);    // 气泡框加在第1行第1列，占1行1列
    if(_role == ChatRole::Self){    // 其他地方需要根据用户角色来排版
        _nameLabel->setContentsMargins(0,0,8,0);    // 昵称标签右边距为8
        _nameLabel->setAlignment(Qt::AlignRight);
        gridLayout->addWidget(_iconLabel, 0,2,2,1, Qt::AlignTop);  // 标签中的内容（即头像）要顶部对齐
        gridLayout->addItem(spacer, 1,0,1,1);
        gridLayout->setColumnStretch(0,2);  // 第0行拉伸时变化比例为2
        gridLayout->setColumnStretch(1,3);  // 第1行拉伸时变化比例为3
    }else{
        _nameLabel->setContentsMargins(8,0,0,0);    // 昵称标签左边距为8
        _nameLabel->setAlignment(Qt::AlignLeft);
        gridLayout->addWidget(_iconLabel, 0,0,2,1, Qt::AlignTop);  // 标签中的内容（即头像）要顶部对齐
        gridLayout->addItem(spacer, 1,2,1,1);
        gridLayout->setColumnStretch(0,2);  // 第0行拉伸时变化比例为2
        gridLayout->setColumnStretch(1,3);  // 第1行拉伸时变化比例为3
    }

    this->setLayout(gridLayout);

}

void ChatItemBase::setUserName(const QString &name)
{
    _nameLabel->setText(name);
}

void ChatItemBase::setUserIcon(const QPixmap &icon)
{
    _iconLabel->setPixmap(icon);
}

void ChatItemBase::setWidget(QWidget *w)
{
    QGridLayout* gridLayout = qobject_cast<QGridLayout*>(this->layout());
    gridLayout->replaceWidget(_bubble, w);  // 将占位的_bubble替换成w
    if(_bubble != nullptr) delete _bubble;
    _bubble=w;
}
