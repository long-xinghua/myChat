#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent), _countdown(5),
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

    // 连接一下编辑栏编辑完成后的处理函数
    connect(ui->userEdit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });

    connect(ui->mailEdit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });

    connect(ui->passwdEdit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });

    connect(ui->confirmEdit, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });

    connect(ui->varifyEdit, &QLineEdit::editingFinished, this, [this](){
        checkVarifyValid();
    });

    ui->pass_visible->setCursor(Qt::PointingHandCursor);    // 让鼠标移到标签上时变成小手的样子
    ui->confirm_visible->setCursor(Qt::PointingHandCursor); // 同上

    // 加载一下显示眼睛的标签状态
    ui->pass_visible->setState("unvisible","unvisible_hover","","visible","visible_hover","");  // 在这里不用设置press和select_press，用不上
    ui->confirm_visible->setState("unvisible","unvisible_hover","","visible","visible_hover","");

    // 点击眼睛标签切换密码可见性
    connect(ui->pass_visible, &ClickedLabel::clicked, this, [this](){
        if(ui->pass_visible->GetCurState() == ClickLbState::Normal){  // 未选中状态，密码不可见
            ui->passwdEdit->setEchoMode(QLineEdit::Password);
        }else{
            ui->passwdEdit->setEchoMode(QLineEdit::Normal);
        }
    });

    connect(ui->confirm_visible, &ClickedLabel::clicked, this, [this](){
        if(ui->confirm_visible->GetCurState() == ClickLbState::Normal){  // 未选中状态，密码不可见
            ui->confirmEdit->setEchoMode(QLineEdit::Password);
        }else{
            ui->confirmEdit->setEchoMode(QLineEdit::Normal);
        }
    });

    _countdownTimer = new QTimer(this);

    connect(_countdownTimer, &QTimer::timeout, [this](){
        if(_countdown == 0){    // 到达时间，返回主界面
            _countdownTimer->stop();
            emit sigReturnToLogin();
            return;
        }
        _countdown--;
        auto str = QString("注册成功，%1 s后返回").arg(_countdown);
        ui->tipLabel->setText(str);
    });
}

RegisterDialog::~RegisterDialog()
{
    qDebug()<<"RegisterDialog析构函数";
    delete ui;
}

void RegisterDialog::on_getCode_clicked()
{
//    bool valid = checkEmailValid();
//    if(!valid) return;

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
            if(error == ErrorCodes::ERR_UserExist){
                showTip(tr("用户已存在"),false);
            }
            else{
                showTip(tr("参数错误"),false);
            }

            qDebug()<<"回包错误代码："<<error;
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送，注意查收"),true);
        qDebug()<<"回包，email is "<<email;
    });

    // 注册一个‘注册用户’操作的回包的处理逻辑
    _handlers.insert(ReqId::ID_REG_USER, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            qDebug()<<"注册用户失败，回包错误代码："<<error;
            return;
        }
        auto email = jsonObj["email"].toString();
        auto uid = jsonObj["uid"].toInt();
        showTip("用户注册成功！", true);
        qDebug()<<"注册成功，邮箱："<<email<<" uid: "<<uid;

        // 注册成功后跳转到提示页面
         changeTipPage();
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

void RegisterDialog::changeTipPage()
{
    _countdownTimer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    _countdownTimer->start(1000);   // 每隔一秒触发一次超时
}

void RegisterDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;   // 把错误加入_tip_errs中，重复添加会自动覆盖
    showTip(tips, false);
}

void RegisterDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);   // C++的map中没有remove方法，只有erase，用erase删除不存在的键会报错，但QMap用remove删除不存在的件不会执行任何操作，安全
    if(_tip_errs.empty()){  // 空了说明没错误了，清除标签内容
      ui->errLabel->clear();
      return;
    }

    showTip(_tip_errs.first(), false);  // 显示之前未处理的错误
}

bool RegisterDialog::checkUserValid()
{
    if(ui->userEdit->text() == ""){
            AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
            return false;
        }

        DelTipErr(TipErr::TIP_USER_ERR);
        return true;
}

bool RegisterDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->mailEdit->text();
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

bool RegisterDialog::checkPassValid()
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

bool RegisterDialog::checkConfirmValid()
{
    auto pass = ui->passwdEdit->text();
    auto confirm = ui->confirmEdit->text();

    if(confirm.length() < 6 || confirm.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(confirm).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_CONFIRM_ERR, tr("不能包含非法字符"));
        return false;
    }

    DelTipErr(TipErr::TIP_CONFIRM_ERR);

    if(pass != confirm){
        AddTipErr(TipErr::TIP_PWD_CONFIRM, tr("确认密码和密码不匹配"));
        return false;
    }else{
        DelTipErr(TipErr::TIP_PWD_CONFIRM);
    }
    return true;
}

bool RegisterDialog::checkVarifyValid()
{
    auto pass = ui->varifyEdit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void RegisterDialog::on_comfirmButton_clicked()
{
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
    json_obj["email"] = ui->mailEdit->text();
    json_obj["passwd"] = xorString(ui->passwdEdit->text());     // 进行一下异或操作加密后再给GateServer传过去
    json_obj["confirm"] = xorString(ui->confirmEdit->text());   // 同上
    json_obj["verifyCode"] = ui->varifyEdit->text();

    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_register"), json_obj, ReqId::ID_REG_USER, Modules::REGISTERMOD);


}

void RegisterDialog::on_returnButton_clicked()
{
    _countdownTimer->stop();
    emit sigReturnToLogin();
}
