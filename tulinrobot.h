#ifndef TULINROBOT_H
#define TULINROBOT_H

#include <QObject>

class Tulinrobot : public QObject
{
    Q_OBJECT
public:
    explicit Tulinrobot(QObject *parent = nullptr);
    //!!!不需要填入apikey，原因本来使用图灵机器人api
    //! 后面改为了使用青云客api，直接写入在mainwindow的函数中
    //! http://api.qingyunke.com/api.php?key=free&appid=0&msg=" api地址

    const QString API_KEY="";
    const QString URL="";
    QString Tuling_API_URL;

signals:

};

#endif // TULINROBOT_H
