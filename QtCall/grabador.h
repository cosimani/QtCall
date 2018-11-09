#ifndef GRABADOR_H
#define GRABADOR_H

#include <QObject>

#include <QCamera>
#include <QCameraInfo>
#include <QList>
#include <QMediaRecorder>


class Grabador : public QObject
{
    Q_OBJECT
public:
    explicit Grabador( QObject * parent = nullptr );
    virtual ~Grabador();


    void encenderCamara( int numeroDeCamara = 0 );

private:
    QCamera * camera;
    QMediaRecorder * recorder;


private slots:
    void slot_errorGrabacionCamara(QMediaRecorder::Error error);
    void slot_cambioEstadoCamara(QCamera::Status estado);
    void slot_cambioEstadoMediaRecorder(QMediaRecorder::Status estado);
};

#endif // GRABADOR_H
