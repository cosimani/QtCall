#include "webservice.h"
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDateTime>

WebService::WebService( QObject * parent ) : QObject( parent ), tcpServer( new QTcpServer( this ) )
{

}

WebService::~WebService()
{
    tcpServer->close();
    qDeleteAll( listTcpSockets.begin(), listTcpSockets.end() );
}

bool WebService::iniciar( int puerto )
{
    bool exito = tcpServer->listen( QHostAddress::Any, puerto );
    if ( exito )  {
        qDebug() << "WebService escuchando en puerto" << puerto;

        connect( tcpServer, SIGNAL( newConnection() ), this, SLOT( onNewConnection() ) );

        return true;
    }

    return false;
}

bool WebService::isEscuchando()  {
    return tcpServer->isListening();
}

void WebService::onNewConnection()  {
    QTcpSocket * tcpSocket = tcpServer->nextPendingConnection();

    connect( tcpSocket, SIGNAL( readyRead() ), this, SLOT( slot_leerDatos() ) );
//    connect( pSocket, &QTcpSocket::textMessageReceived, this, &WebService::processTextMessage );
//    connect( pSocket, &QTcpSocket::binaryMessageReceived, this, &WebService::processBinaryMessage );
    connect( tcpSocket, SIGNAL( disconnected() ), this, SLOT( socketDisconnected() ) );

    listTcpSockets << tcpSocket;
}

void WebService::processTextMessage( QString message )  {
    QTcpSocket * tcpSocket = qobject_cast< QTcpSocket * >( sender() );
    qDebug() << "Message received:" << message;

    if ( tcpSocket )  {
        tcpSocket->write( message.toStdString().c_str() );
//        pClient->sendTextMessage( message );
    }
}

/**
 * @brief WebService::slot_leerDatos
 * - Este metodo atiende solo las peticiones a /index.php
 * - No tiene en cuenta nada que se envie en la cabecera HTTP
 * - Solo peticiones GET
 * - Igualmente emite la senal signal_solicitudRecibida con todos los datos recibidos
 */
void WebService::slot_leerDatos()  {
    QTcpSocket * tcpSocket = qobject_cast< QTcpSocket * >( sender() );

    QByteArray ba = tcpSocket->readAll();

    // Emite en esta senal todo lo recibido en cada peticion, sin procesar
    emit signal_solicitudRecibida( ba );

    QStringList tokens = QString( ba ).split( QRegExp( "\\s+" ) );  // Separa con espacios y lineas nuevas

    // Algunas validaciones por las dudas
    if ( tokens.size() >= 2 )  {

        QString token1 = tokens.at( 0 );
        QString token2 = tokens.at( 1 );
        QString token3 = tokens.at( 2 );

        if ( token1 == "GET" && token3.startsWith( "HTTP" ) )  {

            QString recurso_actualizar_php = "actualizar.php";
            QString recurso_grabaciones_php = "grabaciones.php";
            QString recurso_detener_php = "detener.php";

            // Entra a este if cuando se recibion una consulta correcta con actualizar.php
            if ( token2.contains( recurso_actualizar_php, Qt::CaseInsensitive ) )  {

                // Si entro a este if, entonces token2 es algo como lo siguiente:
                //      /actualizar.php?q=6&aver=9
                //      /actualizar.php?consulta=usuarios&key=1234

                QStringList recurso_y_variables = token2.split( "?" );

                // Entra si la consulta tiene una o mas variables
                if ( recurso_y_variables.size() >= 2 )  {

                    // query contendra todas las variables para se interpretada con metodos de QUrlQuery
                    QUrlQuery query( recurso_y_variables.at( 1 ) );

                    qDebug() << "Variables de la solicitud" << query.queryItems();

                    // Aca se emiten todas las varibles recibidas en la consulta
                    emit signal_queryItems( recurso_actualizar_php, query );

                }

            }

            // Entra a este if cuando se recibion una consulta correcta con grabaciones.php
            if ( token2.contains( recurso_grabaciones_php, Qt::CaseInsensitive ) )  {

                // Si entro a este if, entonces token2 es algo como lo siguiente:
                //      /grabaciones.php?q=6&aver=9
                //      /grabaciones.php?consulta=usuarios&key=1234

                QStringList recurso_y_variables = token2.split( "?" );

                // Entra si la consulta tiene una o mas variables
                if ( recurso_y_variables.size() >= 2 )  {

                    // query contendra todas las variables para se interpretada con metodos de QUrlQuery
                    QUrlQuery query( recurso_y_variables.at( 1 ) );

                    qDebug() << "Variables de la solicitud" << query.queryItems();

                    // Aca se emiten todas las varibles recibidas en la consulta
                    emit signal_queryItems( recurso_grabaciones_php, query );

                }

            }

            // Entra a este if cuando se recibio una consulta correcta con detener.php
            if ( token2.contains( recurso_detener_php, Qt::CaseInsensitive ) )  {

                // Si entro a este if, entonces token2 es algo como lo siguiente:
                //      /detener.php?confirmar=1

                QStringList recurso_y_variables = token2.split( "?" );

                // Entra si la consulta tiene una o mas variables
                if ( recurso_y_variables.size() >= 2 )  {

                    // query contendra todas las variables para se interpretada con metodos de QUrlQuery
                    QUrlQuery query( recurso_y_variables.at( 1 ) );

                    qDebug() << "Variables de la solicitud" << query.queryItems();

                    // Aca se emiten todas las varibles recibidas en la consulta
                    emit signal_queryItems( recurso_detener_php, query );

                }

            }


        }
    }


    if ( tcpSocket )  {
        tcpSocket->write( "HTTP/1.0 200 OK\r\n" );
        tcpSocket->write( "Content-Type: text/html; charset=\"utf-8\"\r\n" );
        tcpSocket->write( "\r\n" );
        tcpSocket->write( "OK - " );
        tcpSocket->write( QDateTime::currentDateTime().toString().toStdString().c_str() );
        tcpSocket->write( "\n" );
        tcpSocket->flush();
        tcpSocket->close();
    }
}

void WebService::processBinaryMessage( QByteArray message )  {
    QTcpSocket * tcpSocket = qobject_cast< QTcpSocket * >( sender() );
    qDebug() << "Binary Message received:" << message;
//    if ( pClient ) {
//        pClient->sendBinaryMessage( message );
//    }
}

void WebService::socketDisconnected()
{
    QTcpSocket * tcpSocket = qobject_cast< QTcpSocket * >( sender() );
    qDebug() << "socketDisconnected:" << tcpSocket;

    if ( tcpSocket ) {
        listTcpSockets.removeAll( tcpSocket );
        tcpSocket->deleteLater();
    }
}
