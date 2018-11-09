#ifndef GRABADOR_H
#define GRABADOR_H

#include <QObject>

#include <QCamera>
#include <QCameraInfo>
#include <QList>
#include <QMediaRecorder>
#include <QDir>


/**
 * @brief The Grabador class Permite grabar de a una cosa por vez. Es decir, si se desean grabar varias cosas, se debera
 * crear un objeto de esta clase para cada cosa que se quiera grabar.
 */
class Grabador : public QObject
{
    Q_OBJECT
public:
    explicit Grabador( QObject * parent = nullptr );
    virtual ~Grabador();

    enum QueGraba { NADA, CAMARA, PANTALLA, PARLANTES, MICROFONO };

    void queGrabar( QueGraba queGraba, QStringList parametrosConfiguracion );

    void detenerGrabacion();
//    bool iniciarGrabacion();


private:
    QCamera * camera;
    QMediaRecorder * recorder;
    QueGraba queGraba;

    /**
     * @brief parametrosConfiguracion Se usara para almacenar la configuracion de lo que se graba. Por ejemplo>
     * Si se graba CAMARA :     parametrosConfiguracion = nroCamara
     * Si se graba PARLANTES :  parametrosConfiguracion =
     * Si se graba MICROFONO :  parametrosConfiguracion =
     * Si se graba PANTALLA :   parametrosConfiguracion =
     */
    QStringList parametrosConfiguracion;


private slots:
    void slot_errorGrabacionCamara(QMediaRecorder::Error error);
    void slot_cambioEstadoCamara(QCamera::Status estado);
    void slot_cambioEstadoMediaRecorder(QMediaRecorder::Status estado);
};

#endif // GRABADOR_H
