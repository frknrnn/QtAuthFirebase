#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class AuthHandler : public QObject
{
    Q_OBJECT
public:
    explicit AuthHandler(QObject *parent = nullptr);
    ~AuthHandler();
    void setAPIKey(const QString & apiKey);
    void signUserUp(const QString & emailAdress,const QString & password);
    void signUserIn(const QString & emailAdress,const QString & password);

public slots:
    void networkReplyReadyRead();
    void performAuthenticatedDatabaseCall();

signals:
    void userSignedIn();


private:
    void performPost(const QString & url, const QJsonDocument & payload);
    void parseResponse(const QByteArray & response );
    QString m_apiKey;
    QNetworkAccessManager * m_networkAccessManager;
    QNetworkReply * m_networkReply;
    QString m_idToken;

};

#endif // AUTHHANDLER_H
