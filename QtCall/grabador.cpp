#include "grabador.h"

Grabador::Grabador(QObject *parent) : QObject(parent),
                                      camera( NULL ),
                                      recorder( NULL )

{





}

Grabador::~Grabador()
{
    recorder->stop();
    camera->stop();
}

/**
 * @brief Grabador::encenderCamara
 * @param numeroDeCamara Enciende la camara *por defecto la camara cero, es decir la primera que encuentra)
 */
void Grabador::encenderCamara( int numeroDeCamara )  {

    QList< QCameraInfo > listadoCamaras = QCameraInfo::availableCameras();

    if ( listadoCamaras.size() > 0 )  {

        if ( camera != NULL )  {
            disconnect( camera, NULL, NULL, NULL );
            delete camera;
        }

        if ( recorder != NULL )  {
            disconnect( recorder, NULL, NULL, NULL );
            delete recorder;
        }


        if ( numeroDeCamara == 0 )  camera = new QCamera( listadoCamaras.at( 0 ) );
        else if ( numeroDeCamara == 1 )  {
            if ( listadoCamaras.size() < 1 )  camera = new QCamera( listadoCamaras.at( 0 ) );
            else  camera = new QCamera( listadoCamaras.at( 1 ) );
        }
        else  camera = new QCamera( listadoCamaras.at( 0 ) );

        camera->setCaptureMode( QCamera::CaptureVideo );
        camera->start();

        connect( camera, SIGNAL( statusChanged( QCamera::Status ) ),
                 this, SLOT( slot_cambioEstadoCamara( QCamera::Status ) ) );


        recorder = new QMediaRecorder( camera );

        connect( recorder, SIGNAL( error( QMediaRecorder::Error ) ),
                 this, SLOT( slot_errorGrabacionCamara( QMediaRecorder::Error ) ) );

        connect( recorder, SIGNAL( statusChanged( QMediaRecorder::Status ) ),
                 this, SLOT( slot_cambioEstadoMediaRecorder( QMediaRecorder::Status ) ) );

        QAudioEncoderSettings audioSettings;
        audioSettings.setCodec( "audio/amr" );
        audioSettings.setQuality( QMultimedia::HighQuality );
        recorder->setAudioSettings( audioSettings );

        QVideoEncoderSettings videoSettings = recorder->videoSettings();
        videoSettings.setQuality( QMultimedia::VeryHighQuality );
        videoSettings.setCodec( "video/mpeg" );
        videoSettings.setResolution( 640, 480 );
        videoSettings.setFrameRate( 30.0 );
        recorder->setVideoSettings( videoSettings );
        recorder->setContainerFormat( "mp4" );

        recorder->setOutputLocation( QUrl::fromLocalFile( "/home/cosimani/Proyecto/2018/QtCall/grabacion" ) );
        recorder->setMuted( false );  // Si es true, entonces graba sin audio, solo video. Mutea el audio del microfono

    }

}

void Grabador::slot_errorGrabacionCamara( QMediaRecorder::Error error )
{
    qDebug() << "Errrorrrrr " << error;
}

void Grabador::slot_cambioEstadoCamara( QCamera::Status estado )
{
    qDebug() << "cambio de estado" << estado;
    if ( estado == QCamera::ActiveStatus )  {  // Si el estado es activo, entonces podemos comenzar a grabar

        if ( recorder != NULL )  {
            recorder->record();
            qDebug() << "recorder->record();";
        }
    }
}

void Grabador::slot_cambioEstadoMediaRecorder( QMediaRecorder::Status estado )
{
    qDebug() << "Estado de QMediaRecorder"  << estado;
}

