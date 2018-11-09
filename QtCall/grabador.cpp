#include "grabador.h"
#include <QThread>
#include "config.h"

Grabador::Grabador(QObject *parent) : QObject(parent),
                                      camera( NULL ),
                                      recorder( NULL ),
                                      queGraba( NADA )

{


}

Grabador::~Grabador()
{
    if ( recorder != NULL )  {
        recorder->stop();
        QThread::msleep( 100 );  // Esperamos unos 100 ms para que se detenga el recorder
    }
    if ( camera != NULL )  {
        camera->stop();
        QThread::msleep( 100 );  // Esperamos unos 100 ms para que se detenga la camera
    }
}

void Grabador::queGrabar( Grabador::QueGraba queGraba , QStringList parametrosConfiguracion )
{
    // Retorna desconfigurando todo
    if ( queGraba == NADA )  {
        this->queGraba = NADA;
        return;
    }

    if ( queGraba == CAMARA )  {
        this->queGraba = CAMARA;

        int numeroDeCamara = 0;

        if ( parametrosConfiguracion.size() == 0 )  {
            numeroDeCamara = 0;
        }
        else if ( parametrosConfiguracion.size() == 1 )  {
            QString sNumeroCamara = parametrosConfiguracion.at( 0 );

            // Si toInt() no lo puede convertir a int, entonces devuelve 0, por lo tanto, esta todo bien.
            numeroDeCamara = sNumeroCamara.toInt();
        }

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

            QString directorioParaAlmacenar = Config::getInstance()->getString( "directorio_evidencias" );

            // Entra cuando tiene una barra de directorio al final. Si lo tiene, se lo quita.
            // Es decir, si en el archivo de configuracion esta lo siguiente:
            //                   directorio_evidencias := /home/cosimani/Proyecto/QtCall/registros/
            // lo deja como:     directorio_evidencias := /home/cosimani/Proyecto/QtCall/registros
            if ( directorioParaAlmacenar.lastIndexOf( "/" ) == directorioParaAlmacenar.size() - 1 )  {

                directorioParaAlmacenar = directorioParaAlmacenar.remove( directorioParaAlmacenar.size() - 1, 1 );

                // Lo corrije en el archivo
                Config::getInstance()->setString( "directorio_evidencias", directorioParaAlmacenar );
            }

            QDir dirDirectorioParaAlmacenar( Config::getInstance()->getString( "directorio_evidencias" ) );

            // Si el directorio indicado en la variable directorio_evidencias no existe, entonces aca se
            // crea el directorio incluyendo todos los directorios anteriores
            if ( ! dirDirectorioParaAlmacenar.exists() )  {

                bool exito = dirDirectorioParaAlmacenar.mkpath( Config::getInstance()->getString( "directorio_evidencias" ) );

                qDebug() << "se creo" << Config::getInstance()->getString( "directorio_evidencias" ) << exito;

            }

            QString nombre_archivo = Config::getInstance()->getString( "directorio_evidencias" ) + "/";
            nombre_archivo += Config::getInstance()->getString( "nombre_usuario" );


            recorder->setOutputLocation( QUrl::fromLocalFile( nombre_archivo ) );
//            recorder->setOutputLocation( QUrl::fromLocalFile( "./grabacion" ) );
            recorder->setMuted( false );  // Si es true, entonces graba sin audio, solo video. Mutea el audio del microfono

//            qDebug() << QDir::currentPath();

        }

    }
}


void Grabador::detenerGrabacion()
{
    if ( queGraba == CAMARA )  {
        if ( recorder != NULL )  {
            recorder->stop();
            QThread::msleep( 100 );  // Esperamos unos 100 ms para que se detenga el recorder
        }
        if ( camera != NULL )  {
            camera->stop();
            QThread::msleep( 100 );  // Esperamos unos 100 ms para que se detenga la camera
        }
    }

}

//bool Grabador::iniciarGrabacion()
//{
//    // Si aun no se configuro correctamente que se desea grabar, entocnes claramente no iniciara ninguna grabacion
//    if ( queGraba == NADA )
//        return false;

//    if ( queGraba == CAMARA )  {
//        if ( camera->status() == QCamera::ActiveStatus )  {
//            recorder->record();
//        }
//    }

//    return true;
//}

void Grabador::slot_errorGrabacionCamara( QMediaRecorder::Error error )
{
    qDebug() << "Errrorrrrr " << error;
}

void Grabador::slot_cambioEstadoCamara( QCamera::Status estado )
{
    qDebug() << "cambio de estado" << estado;
    if ( estado == QCamera::ActiveStatus )  {  // Si el estado es activo, entonces podemos comenzar a grabar

        if ( queGraba == CAMARA )  {
            if ( recorder != NULL )  {
                recorder->record();
                qDebug() << "recorder->record();";
            }
        }

    }
}

void Grabador::slot_cambioEstadoMediaRecorder( QMediaRecorder::Status estado )
{
    qDebug() << "Estado de QMediaRecorder"  << estado;
}

