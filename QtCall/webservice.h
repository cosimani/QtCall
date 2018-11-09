#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include <QObject>
#include <QList>
#include <QByteArray>
#include <QStringList>
#include <QUrlQuery>

class QTcpServer;
class QTcpSocket;


class WebService : public QObject  {
    Q_OBJECT

public:
    explicit WebService( QObject * parent = 0 );
    ~WebService();

    bool iniciar( int puerto );

    bool isEscuchando();

private:
    QTcpServer * tcpServer;
    QList< QTcpSocket * > listTcpSockets;

private slots:
    void onNewConnection();
    void processTextMessage( QString message );
    void slot_leerDatos();
    void processBinaryMessage( QByteArray message );
    void socketDisconnected();


signals:
    void signal_solicitudRecibida( QByteArray );
    void signal_queryItems( QUrlQuery );
};

#endif //ECHOSERVER_H
