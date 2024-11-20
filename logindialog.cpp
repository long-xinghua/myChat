#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
#include <QPainter>
#include "httpmgr.h"
#include "tcpmgr.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 将“点击按钮”信号连接到跳转至“注册页面”信号
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginDialog::switch2reg);

    // 初始化忘记密码标签
    ui->forgetLabel->setState("normal", "normal_hover", "", "selectd", "selected_hover", "");
    connect(ui->forgetLabel, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_passwd);

    // 初始化headLabel
    initHeadLabel();

    // 初始化错误提示
    ui->errLabel->setProperty("state","normal");  // 给错误标签设置属性，把"state"属性设置成"normal"
    refresh(ui->errLabel);
    ui->errLabel->clear();

    // 将密码标签内容设置为不可见
    ui->passwdEdit->setEchoMode(QLineEdit::Password);

    // 初始化回调函数
    initHttpHandlers();

    //连接HttpMgr发过来的http完成信号(这里犯了个错，connect的第一个参数是裸指针，HttpMgr::GetInstance()只拿到了智能指针，需再调用get()得到裸指针)
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_login_mod_finish, this, &LoginDialog::slot_login_mod_finish);

    //连接tcp连接请求的信号和槽
    connect(this, &LoginDialog::sig_connect_tcp, TcpMgr::GetInstance().get(), &TcpMgr::slot_tcp_connect);
    // 连接tcpMgr发出的连接成功信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_con_success, this, &LoginDialog::slot_tcp_con_finish);
    // 连接tcpMgr发出的登录失败信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_login_failed, this, &LoginDialog::slot_login_failed);
}

LoginDialog::~LoginDialog()
{
    qDebug()<<"LoginDialog析构函数";
    delete ui;
}

void LoginDialog::initHeadLabel()
{
    // 加载图片
    QPixmap originalPixmap(":/res/link1.jpg");
    // 设置图片的自动缩放
    qDebug()<<originalPixmap.size()<<ui->headLabel->size();
    originalPixmap = originalPixmap.scaled(ui->headLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);   // KeepAspectRatio等比例缩放，SmoothTransformation平滑转移

    // 创建一个和原始图片大小一致的QPixmap，用于绘制圆角图片
    QPixmap roundedPixmap(originalPixmap.size());
    roundedPixmap.fill(Qt::transparent);    // 将图片内容设置为透明的

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform); // 更平滑

    // 用QPainterPath设置圆角
    QPainterPath path;
    path.addRoundedRect(0,0,originalPixmap.width(), originalPixmap.height(), 10, 10);   // 最后两个参数为x和y方向的圆角半径
    painter.setClipPath(path);  // 用painter画出path路径

    // 将原始图片绘制到roundedPixmap上
    painter.drawPixmap(0,0,originalPixmap);

    // 将画好的roundedPixmap添加到headLabel上
    ui->headLabel->setPixmap(roundedPixmap);
}

void LoginDialog::initHttpHandlers()
{
    _handlers.insert(ReqId::ID_LOGIN_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            if(error == 1009){  // 1009为GateServer中密码和邮箱不匹配的错误
                showTip(tr("邮箱或密码不正确"), false);
            }else{
                showTip(tr("参数错误"), false);
            }
            // 在返回之前将按钮置为可用状态
            enableBtn(true);
            return;
        }
        showTip(tr("登录成功！"), true);
        auto email = jsonObj["email"].toString();

        ServerInfo si;
        si.host = jsonObj["host"].toString();
        si.port = jsonObj["port"].toString();
        si.token = jsonObj["token"].toString();
        si.uid = jsonObj["uid"].toInt();

        _uid = si.uid;
        _token = si.token;

        qDebug()<<"host: "<<si.host<<" port: "<<si.port<<" token: "<<si.token<<" uid: "<<si.uid;
        // 发起信号连接到tcp服务器
        emit sig_connect_tcp(si);
    });
}

void LoginDialog::showTip(QString str, bool ok)
{
    ui->errLabel->setText(str); // 在错误标签上显示信息
    if(ok){ // 根据信息是否正确来更改标签qss样式
        ui->errLabel->setProperty("state","normal");
    }else{
        ui->errLabel->setProperty("state","err");
    }
    refresh(ui->errLabel);  // 刷新
}

void LoginDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;   // 把错误加入_tip_errs中，重复添加会自动覆盖
    showTip(tips, false);
}

void LoginDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);   // C++的map中没有remove方法，只有erase，用erase删除不存在的键会报错，但QMap用remove删除不存在的件不会执行任何操作，安全
    if(_tip_errs.empty()){  // 空了说明没错误了，清除标签内容
      ui->errLabel->clear();
      return;
    }

    showTip(_tip_errs.first(), false);  // 显示之前未处理的错误
}

bool LoginDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->emailEdit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }

    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool LoginDialog::checkPassValid()
{
    auto pass = ui->passwdEdit->text();

    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }

    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

void LoginDialog::enableBtn(bool enabled)
{
    ui->loginButton->setEnabled(enabled);
    ui->registerButton->setEnabled(enabled);
}

void LoginDialog::slot_forget_passwd()
{
    qDebug()<<"忘记密码槽函数";
    emit switch2reset();    // 切换到重置密码的界面上
}

void LoginDialog::on_loginButton_clicked()
{
    qDebug()<<"按下登录按钮";
    if(!checkEmailValid()){
        return;
    }

    if(!checkPassValid()){
        return;
    }

    enableBtn(false);   // 满足条件后先将按钮置为不可用防止用户重复点击

    QJsonObject jsonObj;
    jsonObj["email"] = ui->emailEdit->text();
    jsonObj["passwd"] = xorString(ui->passwdEdit->text());

    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"), jsonObj, ReqId::ID_LOGIN_USER, Modules::LOGINMOD);

}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){   // 在这里错误只可能是ERR_NETWORK
        showTip(tr("网络请求错误"), false);
        enableBtn(true);
        return;
    }

    //解析json字符串
    QJsonDocument jsonDoc = QJsonDocument::fromJson((res.toUtf8()));
    //json解析错误
    if(jsonDoc.isNull() || !jsonDoc.isObject()){
        showTip(tr("json解析错误"), false);
        enableBtn(true);
        return;
    }

    _handlers[id](jsonDoc.object()); //回调函数，根据任务id来进行处理
    // 回调函数执行完了但还没与tcp服务器成功建立连接，也没有成功登录，要继续等待连接和登录的结果，不能在这就enable按钮
    // enableBtn(true);
    return;


}

void LoginDialog::slot_tcp_con_finish(bool success)
{
    if(!success){
        showTip(tr("服务器连接失败"), false);
        enableBtn(true);
        return;
    }
    showTip(tr("服务器连接成功！正在登录..."), true);
    QJsonObject jsonObj;
    jsonObj["uid"] = _uid;
    jsonObj["token"] = _token;
    // 创建QJsonDocument对象将json对象转换成json字符串，方便传输
    QJsonDocument jsonDoc(jsonObj);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);

    // 给tcp服务器发送请求，进行逻辑上的登录操作
    emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_CHAT_LOGIN, jsonString);


}

void LoginDialog::slot_login_failed(int err)
{
    showTip(tr("出错了！登陆失败"),false);
    qDebug()<<"登陆失败，错误代码："<<err;
    enableBtn(true);
}
