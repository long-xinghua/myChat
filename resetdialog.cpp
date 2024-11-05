#include "resetdialog.h"
#include "ui_resetdialog.h"
#include "httpmgr.h"

ResetDialog::ResetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetDialog)
{
    ui->setupUi(this);
    // 密码编辑框内容不可见
    ui->passwdEdit->setEchoMode(QLineEdit::Password);
    ui->errTip->setProperty("state","normal");  // 给错误标签设置属性，把"state"属性设置成"normal"
    refresh(ui->errTip);
    

    ui->errTip->clear();
    
    connect(ui->userEdit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });

    connect(ui->emailEdit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });

    connect(ui->passwdEdit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });


    connect(ui->verifyEdit, &QLineEdit::editingFinished, this, [this](){
         checkVarifyValid();
    });

    //连接reset相关信号和注册处理回调
    initHttpHandlers();
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reset_mod_finish, this,
            &ResetDialog::slot_reset_mod_finish);
    
}

ResetDialog::~ResetDialog()
{
    qDebug()<<"ResetDialog析构函数";
    delete ui;
}


void ResetDialog::on_verifyButton_clicked()
{
    if(!checkEmailValid()){
        return;
    }
    auto email = ui->emailEdit->text();
    QJsonObject json_obj;
    json_obj["email"] = email;
    // 给服务器发送post请求
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_varifyCode"), json_obj, ReqId::ID_GET_VARIFY_CODE, Modules::RESETMOD);
    showTip(tr("正在发送验证码"),true);
}

void ResetDialog::on_sureButton_clicked()
{
    // 在发送http请求之前检查输入信息是否有误
    bool valid = checkUserValid();
    if(!valid){
        return;
    }
    valid = checkEmailValid();
    if(!valid){
        return;
    }
    valid = checkPassValid();
    if(!valid){
        return;
    }
    valid = checkVarifyValid();
    if(!valid){
        return;
    }
    // 给GateServer发送post请求，判断验证码是否正确（GateServer去redis中查找相应邮箱对应的验证码）
    QJsonObject json_obj;
    json_obj["user"] = ui->userEdit->text();
    json_obj["email"] = ui->emailEdit->text();
    json_obj["passwd"] = xorString(ui->passwdEdit->text());     // 进行一下异或操作加密后再给GateServer传过去
    json_obj["verifyCode"] = ui->verifyEdit->text();

    // 向GateServer发起reset_passwd的请求
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/reset_passwd"), json_obj, ReqId::ID_RESET_PWD, Modules::RESETMOD);

}

void ResetDialog::on_returnButton_clicked()
{
    // 返回登录界面
    emit switch2Login();
}

void ResetDialog::slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err)
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

bool ResetDialog::checkUserValid()
{
    if(ui->userEdit->text() == ""){
        AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool ResetDialog::checkEmailValid()
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

bool ResetDialog::checkPassValid()
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

bool ResetDialog::checkVarifyValid()
{
    auto pass = ui->verifyEdit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void ResetDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;   // 把错误加入_tip_errs中，重复添加会自动覆盖
    showTip(tips, false);
}

void ResetDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);   // C++的map中没有remove方法，只有erase，用erase删除不存在的键会报错，但QMap用remove删除不存在的件不会执行任何操作，安全
    if(_tip_errs.empty()){  // 空了说明没错误了，清除标签内容
      ui->errTip->clear();
      return;
    }

    showTip(_tip_errs.first(), false);  // 显示之前未处理的错误
}

void ResetDialog::showTip(QString str, bool ok)
{
    ui->errTip->setText(str); // 在错误标签上显示信息
    if(ok){ // 根据信息是否正确来更改标签qss样式
        ui->errTip->setProperty("state","normal");
    }else{
        ui->errTip->setProperty("state","err");
    }
    refresh(ui->errTip);  // 刷新
}

void ResetDialog::initHttpHandlers()
{
    // 注册获取验证码回包的逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){   // 说明出现错误,防御式编程，有错就返回
            showTip(tr("参数错误"),false);
            qDebug()<<"回包错误代码："<<error;
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送，注意查收"),true);
        qDebug()<<"回包，email is "<<email;
    });

    // 注册一个处理‘重置密码’操作的回包的处理逻辑
    _handlers.insert(ReqId::ID_RESET_PWD, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            if(error == 1007){   // 1007是GateServer中设置的EmailNoMatch错误，找不到用户
                showTip(tr("用户名或邮箱错误"),false);
            }
            else{
                showTip(tr("参数错误"),false);
            }

            qDebug()<<"重置密码失败，回包错误代码："<<error;
            return;
        }
        auto email = jsonObj["email"].toString();
        auto uid = jsonObj["uid"].toInt();
        showTip("密码重置成功！点击返回登录", true);
        qDebug()<<"密码重置成功，邮箱："<<email<<" uid: "<<uid;
    });
}
