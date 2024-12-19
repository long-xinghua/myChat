#include "applyfriendpage.h"
#include "ui_applyfriendpage.h"

#include "applyfriendlist.h"
#include "tcpmgr.h"
#include <random>
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
//#include "authenfriend.h"
#include "applyfriend.h"
#include "usermgr.h"

ApplyFriendPage::ApplyFriendPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplyFriendPage)
{
    ui->setupUi(this);
    connect(ui->applyFriendList, &ApplyFriendList::sig_show_search, this, &ApplyFriendPage::sig_show_search);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp, this, &ApplyFriendPage::slot_auth_rsp);

    // 测试用，加载一些item
    loadApplyList();
}

ApplyFriendPage::~ApplyFriendPage()
{
    delete ui;
}

void ApplyFriendPage::addNewApply(std::shared_ptr<AddFriendApply> apply)
{
    //先模拟头像随机，以后头像资源增加资源服务器后再显示
    //int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int randomValue = std::rand()%100+1;
    int head_i = randomValue % heads.size();
    auto* apply_item = new ApplyFriendItem();
    auto apply_info = std::make_shared<ApplyInfo>(apply->_from_uid,
             apply->_name, apply->_desc,heads[head_i], apply->_name, 0, 0);
    apply_item->setInfo( apply_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(apply_item->sizeHint());
    // 将item设为不可用且不可选中
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
    // 需要在最上面一行添加item，所以用insertItem，不是addItem
    ui->applyFriendList->insertItem(0,item);
    ui->applyFriendList->setItemWidget(item, apply_item);
    apply_item->showAddBtn(true);
    //收到审核好友信号
    connect(apply_item, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> apply_info) {
//        auto* authFriend = new AuthenFriend(this);
//        authFriend->setModal(true);
//        authFriend->setApplyInfo(apply_info);
//        authFriend->show();
        });
}

void ApplyFriendPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ApplyFriendPage::loadApplyList()
{
    //添加好友申请，这里获取的是实际数据
    auto apply_list = UserMgr::GetInstance()->getApplyList();
    for(auto &apply: apply_list){
//        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int randomValue = std::rand()%100+1;
        int head_i = randomValue % heads.size();
        auto* apply_item = new ApplyFriendItem();
        apply->setIcon(heads[head_i]);
        apply_item->setInfo(apply);
        QListWidgetItem* item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(apply_item->sizeHint());
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
        ui->applyFriendList->insertItem(0,item);
        ui->applyFriendList->setItemWidget(item, apply_item);
        if(apply->_status){
            apply_item->showAddBtn(false);
        }else{
             apply_item->showAddBtn(true);
        }

        //收到审核好友信号
        connect(apply_item, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> apply_info) {
//            auto* authFriend = new AuthenFriend(this);
//            authFriend->setModal(true);
//            authFriend->SetApplyInfo(apply_info);
//            authFriend->show();
            });
    }

    // 模拟假数据，创建QListWidgetItem，并设置自定义的widget
    for(int i = 0; i < 13; i++){
//        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int randomValue = std::rand()%100+1;
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        auto *apply_item = new ApplyFriendItem();
        auto apply = std::make_shared<ApplyInfo>(0, names[name_i], strs[str_i],
                                    heads[head_i], names[name_i], 0, std::rand()%2);    // 最后一个为status，代表是否已经通过申请
        apply_item->setInfo(apply);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(apply_item->sizeHint());
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
        ui->applyFriendList->addItem(item);
        ui->applyFriendList->setItemWidget(item, apply_item);
        //收到审核好友信号
        connect(apply_item, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> apply_info){
//            auto *authFriend =  new AuthenFriend(this);
//            authFriend->setModal(true);
//            authFriend->SetApplyInfo(apply_info);
//            authFriend->show();
        });
    }
}

void ApplyFriendPage::slot_auth_rsp(std::shared_ptr<AuthRsp> rsp)
{
    auto uid = rsp->_uid;
    auto find_iter = _unauthItems.find(uid);
    if(find_iter == _unauthItems.end()){
        return;
    }
    find_iter->second->showAddBtn(false);

}
