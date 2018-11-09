#include <QApplication>
#include "principal.h"
#include "config.h"
#include <QScreen>
#include <QWindow>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Principal w;
    w.show();

    w.resize( 1200, w.height() * 1.75 );

    // Iniciamos logger
    Logger::getInstance()->setOut("../logs");

    // Iniciamos config
    Config::getInstance();

    // Iniciamos y conectamos la DB
    Database::getInstance()->connect();

    qDebug() << Config::getInstance()->getString( "puerto_tcp" );
    qDebug() << Config::getInstance()->getString( "servicio_upload" );


    QScreen * screen = QGuiApplication::primaryScreen();
    QPixmap originalPixmap = screen->grabWindow( 0 );
    originalPixmap.save( "../aver.png" );


    return a.exec();
}
