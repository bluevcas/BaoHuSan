#ifndef BAIDUVOICE_H
#define BAIDUVOICE_H

//语音识别 语音合成两大功能
//先语音识别，再语音合成
#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QUrl>
#include <QtNetwork>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QAudioInput>
#include <QMediaPlayer>
#include <QTime>
#include <QSettings>

//#include "tulingrobot.h"

class BaiduVoice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString initStatus READ getInitStatus WRITE setInitStatus NOTIFY initStatusChanged)

public:
    explicit BaiduVoice(QObject *parent = nullptr);

    //QJsonObject append;
//    QJsonObject VOP_append;
    //TulingRobot *p_TulingRobot;
    //baiduVoice *p_BaiduVoice;
    QString TulingURL;
    QMediaPlayer* media_player;
    QAudioDeviceInfo SpeechCurrentDevice;
    //QAudioInput* audio_input=NULL;
    QBuffer* JsonBuffer = NULL;
    void setUIString(QString str);
    Q_INVOKABLE void audioInit();
    void TokenInit();
    Q_INVOKABLE void getMacAddress();
    void changeBaiduAudioAns(QString str);
    bool JudgeTokenTime();
    //声音识别初始化函数
    Q_INVOKABLE void appInit();
    ~BaiduVoice();
    QString getInitStatus();

signals:
    void changeV2T();
    void changeUIANS();
    //void appInitDown();
    void initStatusChanged(QString initStatus);
    //Access_Token获取成功信号
    void successGotToken(QString token);
public slots:
    //直接文字识别
    void Tuling_replyFinish(QNetworkReply* reply);
    //语音识别过程获得回答
    void voice_Tuling_replyFinish(QNetworkReply* reply);
    //识别声音获得文字
    void Baidu_VoiceToText_replyFinish(QNetworkReply* reply);
    void get_Token_slot(QNetworkReply* reply);
    //文本转语音槽函数
    Q_INVOKABLE void baidu_TextToVoice_replyFinish();
    //获得声音后
    void voice_post_Tuling_slot();
//    void get_Answer_Set_UI();
    void onSendBtnclicked(QString text);
    void setInitStatus(QString newInitStatus);
    Q_INVOKABLE void voiceBtnpressed();
    Q_INVOKABLE void voiceBtnreleased();
private slots:

private:
    //程序初始化状态

    //显示文本
    QString tuling_get_ans;
    QString voice_get_ans;
    QString UI_ANS_String = "你好, 我是保护伞";
    //Ui::MainWindow *ui;
    QNetworkAccessManager* NetAccManager;
    QNetworkRequest NetRequest;
    QAudioInput* audio_input;
    //这里要填入你的百度语音apikey secretkey
    const QString API_KEY="Ge3SDRnjOKpQ3uqeG89RrpaO";
    const QString Secret_Key="yoRkkWICyEjWDqebPTyhyVf1EMiGrdLx";
    QString Access_Token = "";
    //audio to text
    const QString VOP_URL="http://vop.baidu.com/server_api";
    //Text to audio
    const QString TSN_URL="http://tsn.baidu.com/text2audio";
    QString Get_Token_URL="https://aip.baidubce.com/oauth/2.0/token";
    QString MAC_cuid = "";
    QString initStatus = "false";
};

#endif // BAIDUVOICE_H
