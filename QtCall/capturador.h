#ifndef CAPTURADOR
#define CAPTURADOR

#include <QAbstractVideoSurface>
#include <QAbstractVideoBuffer>
#include <QVideoSurfaceFormat>
#include <QList>
#include <QVideoFrame>

class Capturador : public QAbstractVideoSurface  {
        Q_OBJECT

public:
        Capturador(QObject *parent = 0);

        QList<QVideoFrame::PixelFormat> supportedPixelFormats(
                        QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

        bool present(const QVideoFrame &frame);

        QVideoFrame getFrameActual()  {  return frameActual;  }

private:
        QVideoFrame frameActual;
};

#endif // CAPTURADOR

