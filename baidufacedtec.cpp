#include "baidufacedtec.h"

BaiduFaceDtec::BaiduFaceDtec(QObject *parent) : QObject(parent)
{
    NetAccManager = new QNetworkAccessManager(this);
    https_ssl_config(NetRequest);
    //AccToken = readAccToken();
    //getAccToken();
}

//配置https访问，win32依赖libcrypto-1_1.dll和libssl-1_1.dll，放在与exe同级目录下
void BaiduFaceDtec::https_ssl_config(QNetworkRequest& NetworkRequest)
{
    QSslConfiguration config = NetworkRequest.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);
    NetworkRequest.setSslConfiguration(config);
}
//QImage转base64编码字符串
QString BaiduFaceDtec::image2base64_str(const QImage& img)
{
    QByteArray data;
    QBuffer buf(&data);
    buf.open(QIODevice::WriteOnly);
    img.save(&buf, "BMP");
    QString b64str = QString(data.toBase64());
    return b64str;
}

//base64编码字符串转QImage
QImage BaiduFaceDtec::base64_str2image(const QString & base64_str)
{
    QImage image;
    QByteArray base64_data = base64_str.toLatin1();
    image.loadFromData(QByteArray::fromBase64(base64_data));
    return image;
}

void BaiduFaceDtec::writeAccToken(const QString& token)
{
    QFile f("AccToken.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败-write");
    }
    QTextStream txtOutput(&f);
    txtOutput << token;
    f.close();
}

QString BaiduFaceDtec::readAccToken()
{
    QFile f("AccToken.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败-read");
        return "";
    }
    QTextStream txtInput(&f);
    QString lineStr;
    lineStr = txtInput.readLine();
    f.close();
    return lineStr;
}

void BaiduFaceDtec::settrustScore(QString newtrustScore)
{
    trustScore = newtrustScore;
}

QString BaiduFaceDtec::gettrustScore()
{
    return trustScore;
}


//获取token
void BaiduFaceDtec::getAccToken()
{
    //鉴权认证参考：http://ai.baidu.com/ai-doc/REFERENCE/Ck3dwjhhu
    if(AccToken.isEmpty()) {
        QStringList parlist;
        parlist.append(QString("grant_type=%1").arg("client_credentials"));
        parlist.append(QString("client_id=%1").arg(API_Key));
        parlist.append(QString("client_secret=%1").arg(Secret_Key));
        QByteArray parameters = parlist.join("&").toUtf8();

        QUrl url("https://aip.baidubce.com/oauth/2.0/token");
        NetRequest.setUrl(url);
        connect(NetAccManager,&QNetworkAccessManager::finished,this,&BaiduFaceDtec::getAccTokenReply);
        NetAccManager->post(NetRequest,parameters);
    }
}
//获取token槽
void BaiduFaceDtec::getAccTokenReply(QNetworkReply* reply)
{
    QString error = reply->errorString();
    if (!error.isEmpty() && error != "Unknown error") {
        return;
    }
    if (reply->error() != QNetworkReply::NoError) {
        //QMessageBox::warning(0,"","请求错误！");
        return;
    }
    else {
        QByteArray content = reply->readAll();
        reply->deleteLater();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(content, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)){  // 解析未发生错误
            if (doucment.isObject()){ // 文档只有一个json对象
                QJsonObject object = doucment.object();  // 转化为对象
                if (object.contains("access_token")){  // 包含指定的 key
                    QJsonValue value = object.value("access_token");  // 获取指定 key 对应的 value
                    if (value.isString()){  // 判断 value 是否为字符串
                        AccToken =  value.toString();  // 将 value 转化为字符串
                        qDebug()<< AccToken;
                        //writeAccToken(AccToken);    //写入txt文件
                    }
                }
            }
        }
    }
    disconnect(NetAccManager,&QNetworkAccessManager::finished,this,&BaiduFaceDtec::getAccTokenReply);
}
//人脸检测
void BaiduFaceDtec::FaceDetect(const QImage& image)
{
    QString img_base64 = image2base64_str(image);
    //打包请求参数
    QJsonObject post_data;;
    post_data.insert("image", img_base64);
    post_data.insert("image_type", "BASE64");
    post_data.insert("face_field", "age,beauty,gender,expression,face_shape,emotion");  //识别的属性，可自由添加
    QJsonDocument document;
    document.setObject(post_data);
    QByteArray post_param = document.toJson(QJsonDocument::Indented);

    QUrl url(FaceDetectUrl + "?access_token=" + AccToken);
    NetRequest.setUrl(url);
    NetRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json")); //接口固定
    connect(NetAccManager,&QNetworkAccessManager::finished,this,&BaiduFaceDtec::FaceDetectReply);
    NetAccManager->post(NetRequest,post_param);
}
//人脸检测槽
void BaiduFaceDtec::FaceDetectReply(QNetworkReply* reply)
{
    QString error = reply->errorString();
    if (!error.isEmpty() && error != "Unknown error")
    {
        return;
    }
    if (reply->error() != QNetworkReply::NoError)
    {
        //QMessageBox::warning(0,"","请求错误！");
        return;
    }
    else
    {
        QByteArray content = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(content, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {
            if (doucment.isObject())
            {
                QJsonObject root_object = doucment.object();
                if (root_object.take("error_code").toInt() == 0 && root_object.take("error_msg").toString() == "SUCCESS")
                {
                    QJsonObject result_object = root_object.take("result").toObject();
                    int face_num = result_object.take("face_num").toInt();
                    if(face_num != 0)
                    {
                        QJsonArray face_list = result_object.take("face_list").toArray();
                        QString result;
                        for(int index = 0; index < face_num; index++)
                        {
                            QJsonObject face_obj = face_list.at(index).toObject();
                            QJsonObject gender_obj = face_obj.take("gender").toObject();
                            if(gender_obj.take("type").toString() == "female"){
                                result.append("性别：女\n");
                            }
                            else{
                                result.append("性别：男\n");
                            }
                            result.append("年龄：" + QString::number(face_obj.take("age").toInt()) + "\n");
                            QJsonObject shape_obj = face_obj.take("face_shape").toObject();
                            QString rec = shape_obj.take("type").toString();
                            if(rec == "square"){
                                result.append("脸型：方形\n");
                            }
                            else if(rec == "triangle"){
                                result.append("脸型：三角\n");
                            }
                            else if(rec == "oval"){
                                result.append("脸型：椭圆\n");
                            }
                            else if(rec == "round"){
                                result.append("脸型：圆形\n");
                            }
                            else if(rec == "heart"){
                                result.append("脸型：心形\n");
                            }
                            result.append("颜值：" + QString::number(face_obj.take("beauty").toDouble()) + "\n");
                            QJsonObject expression_obj = face_obj.take("expression").toObject();
                            rec = expression_obj.take("type").toString();
                            if(rec == "none"){
                                result.append("表情：面无表情\n");
                            }
                            else if(rec == "smile"){
                                result.append("表情：微笑\n");
                            }
                            else if(rec == "laugh"){
                                result.append("表情：大笑\n");
                            }
                            QJsonObject emotion_obj = face_obj.take("emotion").toObject();
                            rec = emotion_obj.take("type").toString();
                            if(rec == "angry"){
                                result.append("情绪：愤怒\n");
                            }
                            else if(rec == "disgust"){
                                result.append("情绪：厌恶\n");
                            }
                            else if(rec == "fear"){
                                result.append("情绪：恐惧\n");
                            }
                            else if(rec == "happy"){
                                result.append("情绪：高兴\n");
                            }
                            else if(rec == "sad"){
                                result.append("情绪：伤心\n");
                            }
                            else if(rec == "surprise"){
                                result.append("情绪：吃惊\n");
                            }
                            else if(rec == "neutral"){
                                result.append("情绪：无\n");
                            }
                            else if(rec == "pouty"){
                                result.append("情绪：噘嘴\n");
                            }
                            else if(rec == "grimace"){
                                result.append("情绪：鬼脸\n");
                            }
                            result.append("----------face " + QString::number(index+1) +  " end----------\n");
                        }
                        emit DetectReply(result);
                    }
                }
            }
        }
        reply->deleteLater();
    }
    disconnect(NetAccManager,&QNetworkAccessManager::finished,this,&BaiduFaceDtec::FaceDetectReply);
}

//人脸对比
void BaiduFaceDtec::FaceMatch(const QImage& image1, const QImage& image2)
{
    QString img1_base64 = image2base64_str(image1);
    QString img2_base64 = image2base64_str(image2);

    QJsonObject post_data1;
    post_data1.insert("image", img1_base64);
    post_data1.insert("image_type", "BASE64");

    QJsonObject post_data2;
    post_data2.insert("image", img2_base64);
    post_data2.insert("image_type", "BASE64");

    QJsonArray img_array;
    img_array.append(post_data1);
    img_array.append(post_data2);

    QJsonDocument document;
    document.setArray(img_array);
    QByteArray post_param = document.toJson(QJsonDocument::Indented);

    QUrl url(FaceMatchUrl + "?access_token=" + AccToken);
    NetRequest.setUrl(url);
    NetRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json")); //接口固定
    connect(NetAccManager,&QNetworkAccessManager::finished,this,&BaiduFaceDtec::FaceMatchReply);
    NetAccManager->post(NetRequest,post_param);
}

//人脸对比槽
void BaiduFaceDtec::FaceMatchReply(QNetworkReply* reply)
{
    QString error = reply->errorString();
    if (!error.isEmpty() && error != "Unknown error")
    {
        return;
    }
    if (reply->error() != QNetworkReply::NoError)
    {
        //QMessageBox::warning(0,"","请求错误！");
        return;
    }
    else
    {
        QByteArray content = reply->readAll();
        qDebug() << "1:" + content;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(content, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {
            qDebug() << "2" + content;
            if (doucment.isObject())
            {
                QJsonObject root_object = doucment.object();
                if (root_object.take("error_code").toInt() == 0 && root_object.take("error_msg").toString() == "SUCCESS")
                {
                    QJsonObject result_object = root_object.take("result").toObject();
                    emit MatchReply("相似度：" + QString::number(result_object.take("score").toDouble()));
                }
            }
        } else {
            qDebug() << "3" + content;
        }
        reply->deleteLater();
    }
    disconnect(NetAccManager,&QNetworkAccessManager::finished,this,&BaiduFaceDtec::FaceMatchReply);
}

//人脸注册
void BaiduFaceDtec::FaceRegister(const QImage& image, const QString& group_id, const QString& user_id)
{
    QString img_base64 = image2base64_str(image);
    //打包请求参数
    QJsonObject post_data;;
    post_data.insert("image", img_base64);
    post_data.insert("image_type", "BASE64");
    post_data.insert("group_id", group_id);
    post_data.insert("user_id", user_id);
    QJsonDocument document;
    document.setObject(post_data);
    QByteArray post_param = document.toJson(QJsonDocument::Indented);

    QUrl url(FaceAdd + "?access_token=" + AccToken);
    NetRequest.setUrl(url);
    NetRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json")); //接口固定
    connect(NetAccManager,&QNetworkAccessManager::finished,this,&BaiduFaceDtec::FaceRegisterReply);
    NetAccManager->post(NetRequest,post_param);
}

//人脸注册槽
void BaiduFaceDtec::FaceRegisterReply(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        //QMessageBox::warning(0,"","请求错误！");
        return;
    }
    else
    {
        QByteArray content = reply->readAll();

        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(content, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {
            if (doucment.isObject())
            {
                QJsonObject root_object = doucment.object();
                if (root_object.take("error_code").toInt() == 0 && root_object.take("error_msg").toString() == "SUCCESS")
                {
                    emit RegisterReply("人脸注册成功！\n");
                }
            }
        }
        reply->deleteLater();
    }
    disconnect(NetAccManager,&QNetworkAccessManager::finished,this,&BaiduFaceDtec::FaceRegisterReply);
}

//人脸搜索识别
void BaiduFaceDtec::faceSerch(const QString& imageurl)
{
    qDebug() << "正在搜索人脸库" + imageurl;
    QFile file(imageurl);
    QImage image;
    if(file.open(QIODevice::ReadOnly))
    {
        //加载文件
        qDebug() << "文件打开成功";
        image.load(&file, "JPG");
        file.close();
    }
    else
        qDebug() << "文件打开失败" << file.errorString();
    QString img_base64 = image2base64_str(image);
    //打包请求参数
    QJsonObject post_data;;
    post_data.insert("image", img_base64);
    post_data.insert("image_type", "BASE64");
    post_data.insert("group_id_list", "faces_001");
    QJsonDocument document;
    document.setObject(post_data);
    QByteArray post_param = document.toJson(QJsonDocument::Indented);

    QUrl url(FaceSerchUrl + "?access_token=" + AccToken);
    NetRequest.setUrl(url);
    NetRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json")); //接口固定
    connect(NetAccManager,&QNetworkAccessManager::finished,this,&BaiduFaceDtec::faceSerchReply);
    NetAccManager->post(NetRequest,post_param);
}

//人脸搜索识别槽
void BaiduFaceDtec::faceSerchReply(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        //QMessageBox::warning(0,"","请求错误！");
        return;
    }
    else
    {
        qDebug() << "正在匹配";
        QByteArray content = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(content, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {
            if (doucment.isObject())
            {
                QJsonObject root_object = doucment.object();
                if (root_object.take("error_code").toInt() == 0 && root_object.take("error_msg").toString() == "SUCCESS")
                {
                    QJsonObject result_obj = root_object.take("result").toObject();
                    QJsonArray user_list = result_obj.take("user_list").toArray();
                    QString group_id = user_list.at(0).toObject().take("group_id").toString();
                    QString user_id = user_list.at(0).toObject().take("user_id").toString();
                    double score = user_list.at(0).toObject().take("score").toDouble();
                    //emit SerchReply("识别结果：\n用户组：" + group_id + "\n" + "用户名：" + user_id + "\n" + "置信度：" + QString::number(score));
                    //emit SerchReply(QString::number(score));
                    trustScore = QString::number(score);
                    emit trustScoreChanged(QString::number(score));
                    qDebug() << "识别结果：\n用户组：" + group_id + "\n" + "用户名：" + user_id + "\n" + "置信度：" + QString::number(score);
                }
            }
        }
        reply->deleteLater();
    }
    disconnect(NetAccManager,&QNetworkAccessManager::finished,this,&BaiduFaceDtec::faceSerchReply);
}
/*

//证件识别
void BaiduFaceDtec::CardScan(const QImage& image)
{
    QString img_base64 = image2base64_str(image);
}

//证件识别槽
void BaiduFaceDtec::CardScanReply(QNetworkReply* reply)
{

}*/
