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

    webService->iniciar( 8888 );

    connect( webService, SIGNAL( signal_solicitudRecibida( QByteArray ) ),
             this, SLOT( slot_datosRecibidos( QByteArray ) ) );

    connect( ui->pbLimpiar, SIGNAL( pressed() ), ui->te, SLOT( clear() ) );

    connect( webService, SIGNAL( signal_queryItems( QUrlQuery ) ), this, SLOT( slot_procesarQueryItems( QUrlQuery ) ) );


    connect(ui->pbGrabarCamara, SIGNAL(pressed()), this,SLOT(slot_grabarCamara()));
    connect(ui->pbDetenerCamara, SIGNAL(pressed()), this,SLOT(slot_detenerCamara()));

    grabador = new Grabador( this );
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
void Principal::slot_procesarQueryItems( QUrlQuery query )  {

    if ( query.hasQueryItem( "camara" ) )  {
        qDebug() << "camara: " << query.queryItemValue( "camara" );

        if ( query.queryItemValue( "camara" ) == "1" )  {
            qWarning() << "Camara grabando";
        }
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

    if ( query.hasQueryItem( "nombre" ) )  {
        qDebug() << "nombre: " << query.queryItemValue( "nombre" );
    }

    if ( query.hasQueryItem( "ruta" ) )  {
        qDebug() << "ruta: " << query.queryItemValue( "ruta" );
    }

    if ( query.hasQueryItem( "puerto" ) )  {
        qDebug() << "puerto: " << query.queryItemValue( "puerto" );
        Config::getInstance()->setString( "puerto", query.queryItemValue( "puerto" ) );
    }


    if ( query.hasQueryItem( "servicio_upload" ) )  {
        qDebug() << "servicio_upload: " << query.queryItemValue( "servicio_upload" );
    }

}

void Principal::slot_grabarCamara()
{
    grabador->encenderCamara();
}

void Principal::slot_detenerCamara()
{

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

