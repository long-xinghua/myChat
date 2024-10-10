#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->passwdEdit->setEchoMode(QLineEdit::Password);   // 输入模式切换成密码模式，在输入时就看不到输入内容了
    ui->confirmEdit->setEchoMode(QLineEdit::Password);
    ui->errLabel->setProperty("state","normal"); // 给错误标签设置属性，把"state"属性设置成"normal"
    ui->errLabel->setText(" ");
    refresh(ui->errLabel);  // 刷新一下标签
    qDebug()<<"start";
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);    //在这里要是HttpMgr还没有实例化的对象就会进行初始化
    qDebug()<<"end";
    initHttpHndlers();  // 注册各个回调函数
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_getCode_clicked()
{
    auto email = ui->mailEdit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");   //用于匹配邮箱格式的正则表达式
    bool match = regex.match(email).hasMatch();
    if(match){
        // 匹配成功，发送http验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        // 给服务器发送post请求
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_varifyCode"), json_obj, ReqId::ID_GET_VARIFY_CODE, Modules::REGISTERMOD);
        showTip(tr("验证码已发送"),true);
    }else{
        showTip(tr("邮箱地址不正确"),false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        showTip(tr("网络请求错误"),false);
        return;
    }
    // 解析JSON字符串，res转成QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());  //先把QString类型的res转成QByteArray，再传给fromJson来解析，解析结果给到json文件jsonDoc
    if(jsonDoc.isNull()){   //json文件为空，解析失败
        showTip(tr("json解析失败"),false);
        return;
    }
    if(!jsonDoc.isObject()){    //json文件不能转成json对象，说明解析错误
        showTip(tr("json解析错误"),false);
        return;
    }

    _handlers[id](jsonDoc.object()); //回调函数，根据任务id来进行处理
    return;
}

void RegisterDialog::initHttpHndlers()
{
    // 注册获取验证码回包的逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){   // 说明出现错误,防御式编程，有错就返回
            showTip(tr("参数错误"),false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送，注意查收"),true);
        qDebug()<<"email is "<<email;
    });
}

void RegisterDialog::showTip(QString str, bool ok)
{
    ui->errLabel->setText(str); // 在错误标签上显示信息
    if(ok){ // 根据信息是否正确来更改标签qss样式
        ui->errLabel->setProperty("state","normal");
    }else{
        ui->errLabel->setProperty("state","err");
    }
    refresh(ui->errLabel);  // 刷新
}
