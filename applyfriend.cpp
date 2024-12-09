#include "applyfriend.h"
#include "ui_applyfriend.h"
#include <QScrollBar>
#include "usermgr.h"
#include "tcpmgr.h"

ApplyFriend::ApplyFriend(QWidget *parent) : _label_point(2,6),
    QDialog(parent),
    ui(new Ui::ApplyFriend)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    //this->setObjectName("ApplyFriend");
    this->setModal(true);
    ui->nameEdit->setPlaceholderText(tr("wmlatiao"));
    ui->labelEdit->setPlaceholderText("搜索、添加标签");
    ui->remarkEdit->setPlaceholderText("燃烧的胸毛");

    ui->labelEdit->setMaxLength(21);
    ui->labelEdit->move(2, 2);
    ui->labelEdit->setFixedHeight(20);
    ui->labelEdit->setMaxLength(10);
    ui->inputTipWidget->hide();             // 默认的输入提示框是隐藏的，输入分组后根据情况进行展示

    _tip_cur_point = QPoint(5, 5);          // 初始化标签展示列表中标签的添加位置

    _tip_data = { "同学","家人","菜鸟教程","C++ Primer","Rust 程序设计",
                             "父与子学Python","nodejs开发指南","go 语言开发指南",
                                "游戏伙伴","金融投资","微信读书","拼多多拼友", "............", "...........", "..........", ".........", "........", ".......", "......"};     // 模拟测试用，实际上要从服务器获取现有分组

    connect(ui->moreLabel, &ClickOnceLabel::clicked, this, &ApplyFriend::slotShowMoreLabel);  // 点击moreLabel显示更多现有标签
    initTipLbs();

    connect(ui->labelEdit, &CustomizedEdit::returnPressed, this, &ApplyFriend::slotLabelEnter);  //链接输入标签回车事件
    connect(ui->labelEdit, &CustomizedEdit::textChanged, this, &ApplyFriend::slotLabelTextChange);
    connect(ui->labelEdit, &CustomizedEdit::editingFinished, this, &ApplyFriend::slotLabelEditFinished);
    connect(ui->tipLabel, &ClickOnceLabel::clicked, this, &ApplyFriend::slotAddFirendLabelByClickTip);

    ui->scrollArea->horizontalScrollBar()->setHidden(true);
    ui->scrollArea->verticalScrollBar()->setHidden(true);
    ui->scrollArea->installEventFilter(this);
    ui->confirmBtn->setState("normal","hover","press");
    ui->cancelBtn->setState("normal","hover","press");
    //连接确认和取消按钮的槽函数
    connect(ui->cancelBtn, &QPushButton::clicked, this, &ApplyFriend::slotApplyCancel);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &ApplyFriend::slotApplySure);
}

ApplyFriend::~ApplyFriend()
{
    delete ui;
}

void ApplyFriend::initTipLbs()
{
    int lines = 1;
    for(int i = 0; i < _tip_data.size(); i++){  // 根据_tip_data中的数据添加标签

        auto* lb = new ClickedLabel(ui->labelList);
        lb->setState("normal", "hover", "pressed", "selected_normal",
            "selected_hover", "selected_pressed");
        lb->setObjectName("tipslb");
        lb->setText(_tip_data[i]);
        connect(lb, &ClickedLabel::clicked, this, &ApplyFriend::slotChangeFriendLabelByTip);

        QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
        int textWidth = fontMetrics.width(lb->text()); // 获取文本的宽度
        int textHeight = fontMetrics.height(); // 获取文本的高度

        if (_tip_cur_point.x() + textWidth + tip_offset > ui->labelList->width()) {
            lines++;
            // 默认只显示两行标签，要显示所有标签需要按"moreLabel"
            if (lines > 2) {
                delete lb;
                return;
            }

            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

        }

       auto next_point = _tip_cur_point;

       addTipLbs(lb, _tip_cur_point,next_point, textWidth, textHeight);

       _tip_cur_point = next_point;
    }
}

void ApplyFriend::addTipLbs(ClickedLabel* lb, QPoint cur_point, QPoint &next_point, int text_width, int text_height)
{
    lb->move(cur_point);
    lb->show();
    _add_labels.insert(lb->text(), lb);
    _add_label_keys.push_back(lb->text());
    next_point.setX(lb->pos().x()+text_width+15);
    next_point.setY(lb->pos().y());
}

bool ApplyFriend::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->scrollArea && event->type() == QEvent::Enter)
    {
        ui->scrollArea->verticalScrollBar()->setHidden(false);
    }
    else if (obj == ui->scrollArea && event->type() == QEvent::Leave)
    {
        ui->scrollArea->verticalScrollBar()->setHidden(true);
    }
    return QObject::eventFilter(obj, event);
}

void ApplyFriend::setSearchInfo(std::shared_ptr<SearchInfo> si)
{
    _si = si;
    auto applyname = UserMgr::GetInstance()->getName();
    auto bakname = si->_name;
    ui->nameEdit->setText(applyname);   // 默认名字为UserMgr中名字
    ui->remarkEdit->setText(bakname);   // 默认昵称为si中的_name
}


void ApplyFriend::addLabel(QString name)
{
    if(_friend_labels.find(name) != _friend_labels.end()){  // 如果新好友标签中已经有这个标签就直接返回
        ui->labelEdit->clear();
        return;
    }

    // 创建一个firendLabel
    auto tempLabel = new friendLabel(ui->gridWidget);
    tempLabel->setText(name);
    tempLabel->setObjectName("FirendLabel");

    auto maxWidth = ui->gridWidget->width();
    //todo... 添加宽度统计
    if(_label_point.x()+tempLabel->width()+2 > maxWidth){                 // 如果加上这个标签会超出gridWidget范围，则需要换行在开头添加
        _label_point.setY(_label_point.y() + tempLabel->height() + 6);
        _label_point.setX(2);
    }

    tempLabel->move(_label_point);  // 把tempLabel移动到_label_point的位置
    qDebug()<<"标签位置："<<tempLabel->pos();
    tempLabel->show();
    _friend_labels[tempLabel->text()] = tempLabel;      // 把该label添加到_friend_labels的map中管理
    _friend_label_keys.push_back(tempLabel->text());

    connect(tempLabel, &friendLabel::sig_close, this, &ApplyFriend::slotRemoveFriendLabel); // 连接删除该标签时的操作
    // 插入新标签后更新_label_point
    qDebug()<<"标签宽度："<<tempLabel->width();
    _label_point.setX(_label_point.x()+tempLabel->width()+2);

    // 更新标签输入框的位置
    if (_label_point.x() + MIN_APPLY_LABEL_EDIT_LEN > ui->gridWidget->width()) {
        ui->labelEdit->move(2, _label_point.y() + tempLabel->height() + 2);
    }
    else {
        ui->labelEdit->move(_label_point);
    }
    qDebug()<<"输入框位置："<<ui->labelEdit->pos();

    ui->labelEdit->clear();

    if (ui->gridWidget->height() < _label_point.y() + tempLabel->height() + 2) {
        ui->gridWidget->setFixedHeight(_label_point.y() + tempLabel->height() * 2 + 2);
    }
}

void ApplyFriend::resetLabels()
{
    auto max_width = ui->gridWidget->width();
    auto label_height = 0;
    _label_point = QPoint(2,6);     // 重置时重新设置一下_label_point
    for(auto iter = _friend_labels.begin(); iter != _friend_labels.end(); iter++){
        //todo... 添加宽度统计
        if( _label_point.x() + iter.value()->width() > max_width) {
            _label_point.setY(_label_point.y()+iter.value()->height()+6);
            _label_point.setX(2);
        }

        iter.value()->move(_label_point);
        iter.value()->show();

        _label_point.setX(_label_point.x()+iter.value()->width()+2);
        _label_point.setY(_label_point.y());
        label_height = iter.value()->height();
    }

    // 重置后需将labelEdit移动到最后面的标签位置

    if(_friend_labels.isEmpty()){
         ui->labelEdit->move(_label_point);
         return;
    }


    if(_label_point.x() + MIN_APPLY_LABEL_EDIT_LEN > ui->gridWidget->width()){
        ui->labelEdit->move(2,_label_point.y()+label_height+6);
    }else{
         ui->labelEdit->move(_label_point);
    }
}

void ApplyFriend::slotShowMoreLabel()
{
    qDebug()<< "receive more label clicked";
    ui->moreLbWidget->hide();

    ui->labelList->setFixedWidth(325);
    _tip_cur_point = QPoint(5, 5);  // 重置当前添加标签的位置
    auto next_point = _tip_cur_point;
    int textWidth;
    int textHeight;
    //重拍现有的label
    for(auto & added_key : _add_label_keys){
        auto added_lb = _add_labels[added_key];

        QFontMetrics fontMetrics(added_lb->font()); // 获取QLabel控件的字体信息
        textWidth = fontMetrics.width(added_lb->text()); // 获取文本的宽度
        textHeight = fontMetrics.height(); // 获取文本的高度

        if(_tip_cur_point.x() +textWidth + tip_offset > ui->labelList->width()){
            // 换行
            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y()+textHeight+15);
        }
        added_lb->move(_tip_cur_point);

        next_point.setX(added_lb->pos().x() + textWidth + 15);
        next_point.setY(_tip_cur_point.y());

        _tip_cur_point = next_point;

    }

    //添加未添加的
    for(int i = 0; i < _tip_data.size(); i++){
        auto iter = _add_labels.find(_tip_data[i]);
        if(iter != _add_labels.end()){  // 如果在现有标签控件中找到了就跳过
            continue;
        }

        auto* lb = new ClickedLabel(ui->labelList);
        lb->setState("normal", "hover", "pressed", "selected_normal",
            "selected_hover", "selected_pressed");
        lb->setObjectName("tipslb");
        lb->setText(_tip_data[i]);
        connect(lb, &ClickedLabel::clicked, this, &ApplyFriend::slotChangeFriendLabelByTip);

        QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
        int textWidth = fontMetrics.width(lb->text()); // 获取文本的宽度
        int textHeight = fontMetrics.height(); // 获取文本的高度

        if (_tip_cur_point.x() + textWidth + tip_offset > ui->labelList->width()) {

            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

        }

         next_point = _tip_cur_point;

        addTipLbs(lb, _tip_cur_point, next_point, textWidth, textHeight);

        _tip_cur_point = next_point;

    }

   int diff_height = next_point.y() + textHeight + tip_offset - ui->labelList->height();
   ui->labelList->setFixedHeight(next_point.y() + textHeight + tip_offset);

    //qDebug()<<"after resize ui->lb_list size is " <<  ui->lb_list->size();
    ui->scrollAreaContents->setFixedHeight(ui->scrollAreaContents->height()+diff_height);
}

void ApplyFriend::slotLabelEnter()
{
    if(ui->labelEdit->text().isEmpty()){    // 标签输入框没有内容
        return;
    }
    auto text = ui->labelEdit->text();
    addLabel(text);        // 在新好友标签框里加入这个内容
    ui->inputTipWidget->hide();             // 由于输入完成了，提示栏隐藏掉

    // 在下面已存在的标签展示区域中加入这个新标签
    // 先看看有没有叫这个名字的标签
    auto it = std::find(_tip_data.begin(), _tip_data.end(), text);
    if(it == _tip_data.end()){  // 没找到才需要新添加
        _tip_data.push_back(text);
    }

    // 判断现有标签的map中是否有该标签，有的话要变绿
    auto find = _add_labels.find(text);
    if(find != _add_labels.end()){
        find.value()->setCurState(ClickLbState::Selected);   // 展示栏里的标签置为选中状态，变绿
        return;
    }


    // 没找到就添加一个，并设置绿色选中
    auto* lb = new ClickedLabel(ui->labelList);
    lb->setState("normal", "hover", "pressed", "selected_normal",
        "selected_hover", "selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(text);
    connect(lb, &ClickedLabel::clicked, this, &ApplyFriend::slotChangeFriendLabelByTip);    // 联动新好友标签列表和展示栏标签列表
    qDebug() << "ui->lb_list->width() is " << ui->labelList->width();
    qDebug() << "_tip_cur_point.x() is " << _tip_cur_point.x();

    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.width(lb->text()); // 根据字体获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度
    qDebug() << "textWidth is " << textWidth;

    if (_tip_cur_point.x() + textWidth + tip_offset + 3 > ui->labelList->width()) {     // 如果加上这个标签超过labelList的宽度了就另起一行添加

        _tip_cur_point.setX(5);
        _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

    }

    // 开始添加标签
    auto next_point = _tip_cur_point;
    // 将lb添加到展示框中，并设置好next_point的值
    addTipLbs(lb, _tip_cur_point, next_point, textWidth, textHeight);

    _tip_cur_point = next_point;
    // 计算添加label后labelList的高度变化
    int diff_height = next_point.y() + textHeight + tip_offset - ui->labelList->height();
    ui->labelList->setFixedHeight(next_point.y() + textHeight + tip_offset);    // 根据高度变化重新设置高度

    lb->setCurState(ClickLbState::Selected);

    ui->scrollAreaContents->setFixedHeight(ui->scrollAreaContents->height() + diff_height);

}

void ApplyFriend::slotRemoveFriendLabel(QString name)
{
    qDebug() << "receive close signal";

    // 重置添加标签的位置
    _label_point.setX(2);
    _label_point.setY(6);
    // 从现有标签中找到该标签
   auto find_iter = _friend_labels.find(name);

   if(find_iter == _friend_labels.end()){
       return;
   }

   // 从索引中查找要删除的标签
   auto find_key = _friend_label_keys.end();
   for(auto iter = _friend_label_keys.begin(); iter != _friend_label_keys.end();
       iter++){
       if(*iter == name){
           find_key = iter;
           break;
       }
   }
    // 删除该标签的索引
   if(find_key != _friend_label_keys.end()){
      _friend_label_keys.erase(find_key);
   }

    // 删除该标签控件
   delete find_iter.value();
    // 从map中删除控件信息
   _friend_labels.erase(find_iter);
    // 重置标签
   resetLabels();

   // 删除新好友标签后还需调整标签展示栏中标签状态
   auto find_add = _add_labels.find(name);
   if(find_add == _add_labels.end()){
        return;
   }
    // 由于在好友标签中删除了标签，展示栏中的标签要置成未选中状态
   find_add.value()->resetNormalState();
}

void ApplyFriend::slotChangeFriendLabelByTip(QString text, ClickLbState curState)
{
    auto it = _add_labels.find(text);
    if(it == _add_labels.end()) return;
    if(curState == ClickLbState::Selected){
        // 如果在tip（即展示栏）中选中了标签，则需要把该标签也添加到新好友标签中
        addLabel(text);
        return;
    }
    if(curState == ClickLbState::Normal){
        // 取消选中该标签，在新好友标签中也删除该标签
        slotRemoveFriendLabel(text);
        return;
    }
}

void ApplyFriend::slotLabelTextChange(const QString &text)
{
    // 如果输入为空
    if (text.isEmpty()) {
        ui->tipLabel->setText("");
        ui->inputTipWidget->hide();
        return;
    }

    auto iter = std::find(_tip_data.begin(), _tip_data.end(), text);
    // 如果现有标签中匹配不上输入的文本
    if (iter == _tip_data.end()) {
        auto new_text = add_prefix + text;
        ui->tipLabel->setText(new_text);
        ui->inputTipWidget->show();
        return;
    }
    ui->tipLabel->setText(text);
    ui->inputTipWidget->show();
}

void ApplyFriend::slotLabelEditFinished()
{
    ui->inputTipWidget->hide();
}

void ApplyFriend::slotAddFirendLabelByClickTip(QString text)
{
    // 获取text中是否有add_prefix存在，即"添加标签："，如果存在说明是新标签，在新好友标签和现有标签中都要添加并选中
    int index = text.indexOf(add_prefix);
    if (index != -1) {
        text = text.mid(index + add_prefix.length());   // 截取add_prefix之后的内容
    }
    addLabel(text);

    auto find_it = std::find(_tip_data.begin(), _tip_data.end(), text);
    //找到了就只需设置状态为选中即可
    if (find_it == _tip_data.end()) {
        _tip_data.push_back(text);
    }

    //判断标签展示栏是否有该标签
    auto find_add = _add_labels.find(text);
    if (find_add != _add_labels.end()) {
        find_add.value()->setCurState(ClickLbState::Selected);
        return;
    }

    //标签展示栏找不到就增加一个, 并设置绿色选中
    auto* lb = new ClickedLabel(ui->labelList);
    lb->setState("normal", "hover", "pressed", "selected_normal",
        "selected_hover", "selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(text);
    connect(lb, &ClickedLabel::clicked, this, &ApplyFriend::slotChangeFriendLabelByTip);
    qDebug() << "ui->lb_list->width() is " << ui->labelList->width();
    qDebug() << "_tip_cur_point.x() is " << _tip_cur_point.x();

    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.width(lb->text()); // 获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度
    qDebug() << "textWidth is " << textWidth;

    if (_tip_cur_point.x() + textWidth+ tip_offset+3 > ui->labelList->width()) {

        _tip_cur_point.setX(5);
        _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

    }

    auto next_point = _tip_cur_point;

     addTipLbs(lb, _tip_cur_point, next_point, textWidth,textHeight);
    _tip_cur_point = next_point;

    int diff_height = next_point.y() + textHeight + tip_offset - ui->labelList->height();
    ui->labelList->setFixedHeight(next_point.y() + textHeight + tip_offset);

    lb->setCurState(ClickLbState::Selected);

    ui->scrollAreaContents->setFixedHeight(ui->scrollAreaContents->height()+ diff_height );
}

void ApplyFriend::slotApplySure()
{
    qDebug()<<"按下添加好友按钮";
}

void ApplyFriend::slotApplyCancel()
{
    this->hide();
    deleteLater();
}

