#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QAction>
// #include <QRandomGenerator> Qt版本较低，没有这个库，用标准库里的随机数生成器代替
#include <random>
#include <ctime>
#include "chatuserwid.h"
#include <vector>
#include "loadingdialog.h"

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent), _mode(ChatUIMode::ChatMode),_b_loading(false),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);

    ui->addButton->setState("normal", "hover", "press");    // 初始化addButton的状态标签
    ui->searchEdit->setMaxLength(20);                       // 设置searchEdit中的最大文本长度

    // 创建一个搜索action，给搜索框加上搜索图标
    QAction* searchAction = new QAction(ui->searchEdit);                    // searchAction生命周期由searchEdit管理
    searchAction->setIcon(QIcon(":res/search.png"));                        // 给searchAction设置图标
    ui->searchEdit->addAction(searchAction, QLineEdit::LeadingPosition);    // 将 searchAction添加到QLineEdit的前置位置
    ui->searchEdit->setPlaceholderText(QStringLiteral("搜索"));              // 设置searchEdit的占位文本

    // 创建一个清除action，给搜索框加上清除图标，并在按下时清除搜索框内容
    QAction* clearAction = new QAction(ui->searchEdit);
    clearAction->setIcon(QIcon(":res/close_transparent.png"));                     // 先设置一个透明图标，在有文本内容时才改成不透明图标
    ui->searchEdit->addAction(clearAction, QLineEdit::TrailingPosition);    // 将clearAction放到searchEdit的末尾位置
    // 当文字有变化时将透明图标改成不透明的
    connect(ui->searchEdit, &CustomizedEdit::textChanged, [clearAction](const QString& text){
            if(!text.isEmpty()){    // 搜索框内容不为空
                clearAction->setIcon(QIcon(":/res/close_search.png"));
            }else{
                clearAction->setIcon(QIcon(":res/close_transparent.png"));
            }
    });

    connect(clearAction, &QAction::triggered, [this, clearAction](){    // 为什么lambda表达式不需要有bool类型的参数？triggered的参数类型是bool的
        ui->searchEdit->clear();    // 清除搜索框内容
        ui->searchEdit->clearFocus();   // 让搜索框失去焦点,即不处于待输入状态了
        showList(_mode);            // 隐藏搜索列表
    });

    showList(_mode);    // 默认情况下为聊天模式，隐藏搜索列表

    connect(ui->chatUserList, &ChatUserList::sig_loading_chat_user, this, &ChatDialog::slot_loading_chat_user);

    // 下面这里测试用
    addUserListTest();
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

std::vector<QString> heads = {
    ":/res/head_1.jpg",
    ":/res/head_2.jpg",
    ":/res/head_3.jpg",
    ":/res/head_4.jpg",
    ":/res/head_5.jpg",
};

std::vector<QString> names = {
    "jack",
    "tom",
    "alice",
    "jay",
    "peter",
    "doge",
    "zack",
    "james",
    "curry"
};

std::vector<QString> strs = {
    "hello",
    "im fine",
    "the legend of zelda",
    "dark souls",
    "elden ring",
    "super mario odessy"
};

void ChatDialog::addUserListTest()
{
    for(int i=0;i<13;i++){
//        std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr))); // 随机种子
//        std::uniform_int_distribution<int> distribution(0, 100); // 生成0到100的随机数
//        int randomValue = distribution(generator)*i;
        int randomValue = std::rand() % 100 + 1;

        // 得到随机的头像、名字和消息
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        // 创建一个chatUserWid
        auto* chatUserWid = new ChatUserWid();
        chatUserWid->setInfo(names[name_i], heads[head_i], strs[str_i]);
        // 创建一个QListWidgetItem对象
        QListWidgetItem* item = new QListWidgetItem;

        item->setSizeHint(chatUserWid->sizeHint());
        ui->chatUserList->addItem(item);    // 把item加到chatUserList
        ui->chatUserList->setItemWidget(item, chatUserWid); // 把item设置成自定义的widget
    }
}

void ChatDialog::showList(ChatUIMode mode, bool search)
{
    _mode = mode;
    if(search){                                 // 在多种模式下都有搜索功能
        ui->chatUserList->hide();
        ui->contactList->hide();
        ui->searchList->show();
    }
    else if(_mode == ChatUIMode::ChatMode){      // 处于聊天模式
        ui->contactList->hide();
        ui->searchList->hide();
        ui->chatUserList->show();
    }
    else if(_mode == ChatUIMode::ContactMode){   // 处于联系人模式
        ui->searchList->hide();
        ui->chatUserList->hide();
        ui->contactList->show();
    }
}

void ChatDialog::slot_loading_chat_user()
{
    if(_b_loading){     // 如果_b_loading为true说明所有信息都加载出来了，就不再加载了
        return;
    }

    _b_loading = true;
    // 创建一个加载对话框，在加载更多信息时显示正在加载中
    LoadingDialog* loadingDlg = new LoadingDialog(this);
    loadingDlg->setModal(true); // 将对话框设置成模态的，必须等对话框处理完才能进行下一步
    loadingDlg->show();
    qDebug()<<"添加更多用户信息...";
    addUserListTest();  // 这一行为测试用，实际得加载真实数据

    // 加载完毕后关闭对话框
    loadingDlg->deleteLater();



}
