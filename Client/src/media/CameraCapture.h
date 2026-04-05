#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <QObject>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QVideoFrame>
#include <QImage>

class CameraCapture: public QObject{
    Q_OBJECT
    
    public:
        explicit CameraCapture(QObject *parent = nullptr);
        void start();
        void stop();

    signals:
        void frameCapture(const QImage &image);

    private slots:
        void processFrame(const QVideoFrame &frame);

    private:
        QCamera *camera;
        QMediaCaptureSession captureSession;
        QVideoSink *videoSink;
};

#endif