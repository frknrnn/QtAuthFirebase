#include <QCoreApplication>
#include "authhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AuthHandler authHandler;
    authHandler.setAPIKey("");
    authHandler.signUserIn("test@email.com","Pass123");

    return a.exec();
}
