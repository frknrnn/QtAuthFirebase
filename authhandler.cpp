#include "authhandler.h"
#include <QDebug>
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>


AuthHandler::AuthHandler(QObject *parent)
    : QObject(parent)
    , m_apiKey(QString())
{
    m_networkAccessManager = new QNetworkAccessManager( this );
    connect( this, &AuthHandler::userSignedIn, this, &AuthHandler::performAuthenticatedDatabaseCall );
}

AuthHandler::~AuthHandler()
{
    m_networkAccessManager->deleteLater();
}

void AuthHandler::setAPIKey(const QString &apiKey)
{
    m_apiKey = apiKey;
}

void AuthHandler::signUserUp(const QString &emailAddress, const QString &password)
{
    QString signUpEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
    performPost( signUpEndpoint, jsonPayload );
}

void AuthHandler::signUserIn(const QString &emailAddress, const QString &password)
{
    QString signInEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
    performPost( signInEndpoint, jsonPayload );
}

void AuthHandler::networkReplyReadyRead()
{
    QByteArray response = m_networkReply->readAll();
    parseResponse(response);

    /*
    qDebug()<< m_networkReply->readAll();
    */

    m_networkReply->deleteLater();
}

void AuthHandler::performAuthenticatedDatabaseCall()
{
    QString endPoint = "https://qtapptest-3edb8-default-rtdb.firebaseio.com/FF.json?auth=" + m_idToken;
    m_networkReply = m_networkAccessManager->get(QNetworkRequest(QUrl(endPoint)));
    connect(m_networkReply, &QNetworkReply::readyRead,this,&AuthHandler::networkReplyReadyRead);
}

void AuthHandler::performPost(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest newRequest( (QUrl( url )) );
    newRequest.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
    m_networkReply = m_networkAccessManager->post( newRequest, payload.toJson());
    connect( m_networkReply, &QNetworkReply::readyRead, this, &AuthHandler::networkReplyReadyRead );

}

void AuthHandler::parseResponse(const QByteArray &response)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(response);
    if(jsonDocument.object().contains("error")){
        qDebug()<<"Error ocured!"<<response;
    }else if(jsonDocument.object().contains("kind")){
        QString idToken = jsonDocument.object().value("idToken").toString();
        //qDebug()<<"User Id Token :"<<idToken;
        qDebug() << "User signed in successfully!";
        m_idToken = idToken;
        emit userSignedIn();
    }else{
        qDebug()<< "The response :" << response;
    }
}
