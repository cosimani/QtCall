#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QWidget>

#include "webservice.h"
#include "grabador.h"

#include <QCamera>
#include <QCameraInfo>
#include <QList>
#include "capturador.h"
#include <QTimer>
#include <QMediaRecorder>


namespace Ui {
class Principal;
}

class Principal : public QWidget
{
    Q_OBJECT

public:
    explicit Principal(QWidget *parent = 0);
    ~Principal();

private:
    Ui::Principal *ui;

    WebService * webService;

    Grabador * grabador;

private slots:
    void slot_datosRecibidos( QByteArray ba );
    void slot_procesarQueryItems( QUrlQuery query );

    void slot_grabarCamara();
    void slot_detenerCamara();

};

#endif // PRINCIPAL_H
