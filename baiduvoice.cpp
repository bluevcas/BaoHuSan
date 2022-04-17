#include "baiduvoice.h"

BaiduVoice::BaiduVoice(QObject *parent): QObject{parent}
{

}

//程序初始化函数
void BaiduVoice::appInit()
{
    QObject::connect(this, SIGNAL(successGotToken(QString)),
                      this,  SLOT(setInitStatus(QString)));
    TokenInit();
    getMacAddress();
    audioInit();
    //setInitStatus("true");
}

void BaiduVoice::audioInit()
{
    const auto &&availableDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if(!availableDevices.isEmpty())
    {
        SpeechCurrentDevice = availableDevices.first();
        QAudioFormat format;
        format.setSampleRate(8000);
        format.setChannelCount(1);
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::SignedInt);
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setCodec("audio/pcm");
        audio_input = new QAudioInput(SpeechCurrentDevice, format);
    }
    media_player =new QMediaPlayer(this);
    qDebug()<< "24: Success audioInit";
}

void BaiduVoice::TokenInit()
{
    //if(JudgeTokenTime()) return;
    QUrl url = QUrl(Get_Token_URL +
                  "?grant_type=client_credentials"+
                  "&client_id="+ API_KEY +
                  "&client_secret=" + Secret_Key);
    qDebug() << "51: " + url.toString();
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request(url);
    //    QJsonObject append;
    connect(manager,&QNetworkAccessManager::finished,this,&BaiduVoice::get_Token_slot);
    manager->get(request);
}

void BaiduVoice::get_Token_slot(QNetworkReply *reply)
{
    QByteArray JsonStr=reply->readAll();
    QJsonObject acceptedData(QJsonDocument::fromJson(JsonStr).object());
    if(acceptedData.contains("access_token"))
    {
        QJsonValue json_value = acceptedData.take("access_token");
        Access_Token = json_value.toString();
        qDebug()<< "success get token:" << Access_Token;
        //发射成功获取Access_Token信号
        emit successGotToken(Access_Token);
    }
}

BaiduVoice::~BaiduVoice()
{

}

//更改voice_get_ans函数，用于发射信号
void BaiduVoice::changeBaiduAudioAns(QString str)
{
    voice_get_ans = str;
    qDebug()<<"识别声音结果 "<<voice_get_ans;
    emit changeV2T();
}

bool BaiduVoice::JudgeTokenTime()
{
    //https://blog.csdn.net/qq_18286031/article/details/78538769 时间互转连接
    QSettings *setting = new QSettings("GetTokenTime.ini",QSettings::IniFormat);
    //当前时间
    QDateTime current_date_time = QDateTime::currentDateTime();
    QDateTime file_date_time;
    //读取的文件时间
    QString file_date_str=setting->value("/TIME/last_time").toString();
    file_date_time=QDateTime::fromString(file_date_str,"yyyy-MM-dd");
    //    qDebug()<<"file_date_time"<<file_date_time;
    uint stime = file_date_time.toTime_t();
    uint etime = current_date_time.toTime_t();
    int ndaysec = 24*60*60;
    int Day= (etime - stime)/(ndaysec) + ((etime - stime)%(ndaysec)+(ndaysec-1))/(ndaysec) - 1;
    //Token 有效期内
    if(Day<30)    return true;
    else
    {
        //写时间
        setting->beginGroup("TIME");
        setting->setValue("last_time",current_date_time.toString("yyyy-MM-dd"));
        setting->endGroup();
        return false;
    }
}

//更改UI_ANS_String函数，用于发射信号
void BaiduVoice::setUIString(QString str)
{
    UI_ANS_String = str;
    //ui->OutputTextEdit->setText(str);
    qDebug() << "118" + str;
    emit changeUIANS();
}

//获取本机MAC地址
void BaiduVoice::getMacAddress()
{

    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            MAC_cuid = nets[i].hardwareAddress();
            break;
        }
    }
    MAC_cuid.replace(":","-");
    qDebug()<<"自动获取的MAC地址 "<<MAC_cuid;
}

//获得图灵机器人的回答后再播放这个回答
void BaiduVoice::baidu_TextToVoice_replyFinish()
{
    QStringList url;
    url.append("http://tsn.baidu.com/text2audio?");
    url.append("&lan=zh");
    //本机MAC地址
    url.append("&cuid=" + MAC_cuid);
    url.append("&ctp=1");
    url.append("&tok=" + Access_Token);
    url.append("&pit=5");
    url.append("&per=4");
    url.append("&tex=");
    url.append(QUrl::toPercentEncoding(UI_ANS_String));
    media_player->setMedia(QUrl::fromLocalFile(url.join("")));
    qDebug() << "172: " + url.join("");
    media_player->play();
}

//当声音识别成文本后，发射信号到此函数，然后将文本发送给青云客
void BaiduVoice::voice_post_Tuling_slot()
{
    qDebug()<<"successes get voice_post_Tuling_slot";
    QString Url="http://api.qingyunke.com/api.php?key=free&appid=0&msg="+voice_get_ans;
    QUrl url;
    url.setUrl(Url);
    QNetworkRequest request(url);
    QNetworkAccessManager *manager=new QNetworkAccessManager();
    //处理获得文本，连接到voice_Tuling_replyFinish函数
    connect(manager,&QNetworkAccessManager::finished,this,&BaiduVoice::voice_Tuling_replyFinish);
    manager->get(request);
}

//直接聊天
void BaiduVoice::onSendBtnclicked(QString text)
{

    QString Url="http://api.qingyunke.com/api.php?key=free&appid=0&msg=" + text;
    QUrl url;
    url.setUrl(Url);
    QNetworkRequest request(url);
    QNetworkAccessManager *manager=new QNetworkAccessManager();
    connect(manager,&QNetworkAccessManager::finished,this,&BaiduVoice::Tuling_replyFinish);
    manager->get(request);
}

//解析图灵API返回的json数据
void BaiduVoice::Tuling_replyFinish(QNetworkReply *reply)
{

    QString data=reply->readAll();//读取回话信息
    qDebug()<<"这是返回的结果 "+data;
    QJsonParseError json_error;
    //一定要toUtf8 否则会出错！
    QJsonDocument json=QJsonDocument::fromJson(data.toUtf8(),&json_error);
    //判断有没有错
    if(json_error.error==QJsonParseError::NoError)
    {
        if(json.isObject())
        {
            QJsonObject jsonObj=json.object();
            //取到text内容
            if(jsonObj.contains("content"))
            {
                qDebug()<<"成功获取text";
                QJsonValue text_value=jsonObj.take("content");
                if(text_value.isString())
                {
                    tuling_get_ans=text_value.toVariant().toString();
                    tuling_get_ans.replace("br","\n");
                    qDebug()<<"!!!返回的ans:"<<tuling_get_ans;
                }
            }
        }
    }
    reply->deleteLater();
    //ui->OutputTextEdit->setText(tuling_get_ans);
    qDebug() <<"219" + tuling_get_ans;
}

//走语音识别的流程 这是将语音识别文本发送给图灵机器人，然后解析json
void BaiduVoice::voice_Tuling_replyFinish(QNetworkReply *reply)
{
    qDebug()<<"测试走到voice_Tuling_replyFinish...";
    QString data=reply->readAll();//读取回话信息
    qDebug()<<"这是返回的结果 "+data;
    QString tstr;
    QJsonParseError json_error;
    //一定要toUtf8 否则会出错！
    QJsonDocument json=QJsonDocument::fromJson(data.toUtf8(),&json_error);
    //判断有没有错
    if(json_error.error==QJsonParseError::NoError)
    {
        if(json.isObject())
        {
            QJsonObject jsonObj=json.object();
            //取到text内容
            if(jsonObj.contains("content"))
            {
                qDebug()<<"成功获取text";
                QJsonValue text_value=jsonObj.take("content");
                if(text_value.isString())
                {
                    tstr=text_value.toVariant().toString();
                    qDebug()<<"!!!返回的ans:"<<tuling_get_ans;
                }
            }
        }
    }
    reply->deleteLater();
    //    获得返回文本后
    setUIString(tstr);
}


//当按下录音按钮后 开始录音
void BaiduVoice::voiceBtnpressed()
{
    JsonBuffer = new QBuffer;
    JsonBuffer->open(QIODevice::ReadWrite);
    qDebug()<<"录音设备访问成功...";
    audio_input->start(JsonBuffer);
    qDebug()<<"录音设备运行成功...";
}

//松开按钮后，结束录音，发送请求
void BaiduVoice::voiceBtnreleased()
{
    qDebug()<<"结束录音哦~~~";
    audio_input->stop();
    const auto &sendData = JsonBuffer->data();
    JsonBuffer->deleteLater();
    QUrl theurl = QUrl(VOP_URL);
    QNetworkRequest request(theurl);
    request.setRawHeader("Content-Type", "application/json");
    qDebug()<< "jason_speech:request设置头成功 ...";
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager,&QNetworkAccessManager::finished,this,&BaiduVoice::Baidu_VoiceToText_replyFinish);

    QJsonObject append;
    //设置json请求格式
    append["dev_pid"] = 1537;
    append["format"] = "pcm";
    append["rate"] = 8000;
    append["channel"] = 1;
    append["token"] = Access_Token;
    append["lan"] = "zh";
    append["cuid"] = MAC_cuid;
    append["speech"] = QString(sendData.toBase64());
    append["len"] = sendData.size();
    //发送HTTP post请求
    //qDebug() << append;
    manager->post(request,QJsonDocument(append).toJson());
    //qDebug() << QJsonDocument(append);
}

//声音转成文本
void BaiduVoice::Baidu_VoiceToText_replyFinish(QNetworkReply *reply)
{
    qDebug()<<"进到槽函数了哦~~";
    QByteArray JsonStr = reply->readAll();
    //qDebug()<< "返回信息 "<<JsonStr;
    QJsonObject acceptedData(QJsonDocument::fromJson(JsonStr).object());

    if(acceptedData.contains("err_no"))
    {
        QJsonValue version_value = acceptedData.take("err_no");
        if(version_value.isDouble())
        {
            int version = version_value.toVariant().toInt();
            if(version == 3300){
                //                textEdit -> append("错误代码:3300-输入参数不正确,请仔细核对文档及参照demo，核对输入参数");//重新写入文本
            }else if(version == 3301){
                //                textEdit -> append("错误代码:3301-音频质量过差,请上传清晰的音频");//重新写入文本
                //QMessageBox::information(NULL,tr("识别失败"),tr("请等待一秒后开始录音，确认录音质量良好"));
            }else if(version == 3302){
                //                textEdit -> append("错误代码:3302-鉴权失败,token字段校验失败。请用appkey 和 app secret生成");//重新写入文本
            }else if(version == 3303){
                //                textEdit -> append("错误代码:3303-转pcm失败,如下2个原因 1. wav amr音频转码失败，即音频有问题。2. 服务端问题，请将api返回结果反馈至论坛或者QQ群");//重新写入文本
            }else if(version == 3304){
                //                textEdit -> append("错误代码:3304-用户的请求QPS超限,请降低识别api请求频率 （qps以appId计算，移动端如果共用则累计）");//重新写入文本
            }else if(version == 3305){
                //                textEdit -> append("错误代码:3305-用户的日pv（日请求量）超限,请“申请提高配额”，如果暂未通过，请降低日请求量");//重新写入文本
            }else if(version == 3307){
                //                textEdit -> append("错误代码:3307-语音服务器后端识别出错问题,请将api返回结果反馈至论坛或者QQ群");//重新写入文本
            }else if(version == 3308){
                //                textEdit -> append("错误代码:3308-音频过长,音频时长不超过60s，请将音频时长截取为60s以下");//重新写入文本
            }else if(version == 3309){
                //                textEdit -> append("错误代码:3309-音频数据问题,服务端无法将音频转为pcm格式，可能是长度问题，音频格式问题等。 请将输入的音频时长截取为60s以下，并核对下音频的编码，是否是8K或者16K， 16bits，单声道。");//重新写入文本
            }else if(version == 3310){
                //                textEdit -> append("错误代码:3310-输入的音频文件过大,语音文件共有3种输入方式： json 里的speech 参数（base64后）； 直接post 二进制数据，及callback参数里url。 分别对应三种情况：json超过10M；直接post的语音文件超过10M；callback里回调url的音频文件超过10M");//重新写入文本
            }else if(version == 3311){
                //                textEdit -> append("错误代码:3311-采样率rate参数不在选项里,目前rate参数仅提供8000,16000两种，填写4000即会有此错误");//重新写入文本
            }else if(version == 3312){
                //                textEdit -> append("错误代码:3312-音频格式format参数不在选项里,目前格式仅仅支持pcm，wav或amr，如填写mp3即会有此错误");//重新写入文本
            }
            //            return;
        }
    }
    if(!acceptedData["result"].isNull())
    {
        QString message=acceptedData["result"].toArray()[0].toString();
        //ui->InputTextEdit->setText(message);
        //qDebug() << message;
        changeBaiduAudioAns(message);
    }
}

QString BaiduVoice::getInitStatus()
{
    return initStatus;
}

void BaiduVoice::setInitStatus(QString newInitStatus)
{
    if ("" != newInitStatus)
        initStatus = "true";
    emit initStatusChanged(initStatus);
}
