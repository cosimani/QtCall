#include "principal.h"
#include "ui_principal.h"
#include <QDebug>
#include "config.h"
#include <QMessageBox>

Principal::Principal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Principal)
{
    ui->setupUi(this);

    webService = new WebService( this );

    webService->iniciar( Config::getInstance()->getString( "puerto_tcp" ).toInt() );

    connect( webService, SIGNAL( signal_solicitudRecibida( QByteArray ) ),
             this, SLOT( slot_datosRecibidos( QByteArray ) ) );

    connect( ui->pbLimpiar, SIGNAL( pressed() ), ui->te, SLOT( clear() ) );

    connect( webService, SIGNAL( signal_queryItems( QString, QUrlQuery ) ),
             this, SLOT( slot_procesarQueryItems( QString, QUrlQuery ) ) );


    connect(ui->pbGrabarCamara, SIGNAL(pressed()), this,SLOT(slot_grabarCamara()));
    connect(ui->pbDetenerCamara, SIGNAL(pressed()), this,SLOT(slot_detenerCamara()));

    grabador = new Grabador( this );

    grabadorParlantes = new Grabador( this );
    grabadorPantalla = new Grabador( this );
    grabadorMicrofono = new Grabador( this );
    grabadorCamara = new Grabador( this );


}

Principal::~Principal()
{
    delete ui;
}

void Principal::slot_datosRecibidos( QByteArray ba )  {
    ui->te->append( ba );
}

/**
 * @brief Principal::slot_procesarQueryItems
 * @param query
 *
 * Este metodo procesa consultas como la siguiente:
 *      http://localhost:8888/index.php?camara=1&pantalla=0&audio=1&parlantes=1&nombre=usuario1234
 *          &ruta=directorio_registros&servicio_upload=192.168.0.1
 */
void Principal::slot_procesarQueryItems( QString recurso_php, QUrlQuery query )  {

    qDebug() << recurso_php << query.queryItemValue( "camara" );

    if ( recurso_php == "actualizar.php" )  {

        if ( query.hasQueryItem( "nombre_usuario" ) )  {
            qDebug() << "nombre_usuario: " << query.queryItemValue( "nombre_usuario" );
            Config::getInstance()->setString( "nombre_usuario", query.queryItemValue( "nombre_usuario" ) );
        }

        if ( query.hasQueryItem( "directorio_evidencias" ) )  {
            qDebug() << "directorio_evidencias: " << query.queryItemValue( "directorio_evidencias" );
            Config::getInstance()->setString( "directorio_evidencias", query.queryItemValue( "directorio_evidencias" ) );
        }

        if ( query.hasQueryItem( "servicio_upload" ) )  {
            qDebug() << "servicio_upload: " << query.queryItemValue( "servicio_upload" );
            Config::getInstance()->setString( "servicio_upload", query.queryItemValue( "servicio_upload" ) );
        }

    }

    if ( recurso_php == "grabaciones.php" )  {

        if ( query.queryItemValue( "camara" ) == "1" )  {
            qDebug() << "Camara grabando";
        }

        if ( query.hasQueryItem( "pantalla" ) )  {
            qDebug() << "pantalla: " << query.queryItemValue( "pantalla" );
        }

        if ( query.hasQueryItem( "audio" ) )  {
            qDebug() << "audio: " << query.queryItemValue( "audio" );
        }

        if ( query.hasQueryItem( "parlantes" ) )  {
            qDebug() << "parlantes: " << query.queryItemValue( "parlantes" );

        }

        if ( query.hasQueryItem( "camara" ) )  {
            qDebug() << "camara: " << query.queryItemValue( "camara" );

            QStringList parametros;
            parametros << "0";  // Queremos grabar desde la camara 0
            grabadorCamara->queGrabar( Grabador::CAMARA, parametros );

        }

    }

    if ( recurso_php == "detener.php" )  {

        if ( query.queryItemValue( "confirmar" ) == "1" )  {
            qDebug() << "detuvo todas las grabaciones";

            grabador->detenerGrabacion();
            grabadorPantalla->detenerGrabacion();
            grabadorCamara->detenerGrabacion();
            grabadorMicrofono->detenerGrabacion();
            grabadorParlantes->detenerGrabacion();
        }
    }

}

void Principal::slot_grabarCamara()
{
    QStringList parametros;
    parametros << "0";  // Queremos grabar desde la camara 0
    grabador->queGrabar( Grabador::CAMARA, parametros );
}

void Principal::slot_detenerCamara()
{
    grabador->detenerGrabacion();
}





//void Principal::slot_procesarImagen()  {
//    QVideoFrame frameActual = capturador->getFrameActual();

//    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat( frameActual.pixelFormat() );

//    QImage image( frameActual.bits(),
//                  frameActual.width(),
//                  frameActual.height(),
//                  frameActual.bytesPerLine(),
//                  imageFormat );


//    ui->presentador->setImage(image);
//}

