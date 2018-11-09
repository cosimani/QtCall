#include "capturador.h"

Capturador::Capturador(QObject *parent) : QAbstractVideoSurface(parent)  {

}

QList<QVideoFrame::PixelFormat>
Capturador::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const  {
    if (handleType == QAbstractVideoBuffer::NoHandle)  {
        return QList<QVideoFrame::PixelFormat>()
                    << QVideoFrame::Format_RGB32
                    << QVideoFrame::Format_ARGB32;
    }
    else  {
        return QList<QVideoFrame::PixelFormat>();
    }
}

bool Capturador::present(const QVideoFrame &frame)  {
    frameActual = frame;

    frameActual.map(QAbstractVideoBuffer::ReadOnly);

    return true;  // Con la idea de devolver true si este frame fue usado
}
