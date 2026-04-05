#include "CameraCapture.h"

CameraCapture::CameraCapture(QObject *parent)
    :QObject(parent){
    
    camera = new QCamera(this);
    videoSink = new QVideoSink(this);

    captureSession.setCamera(camera);
    captureSession.setVideoSink(videoSink);

    connect(videoSink, &QVideoSink::videoFrameChanged, this, &CameraCapture::processFrame);
}

void CameraCapture::start(){
    camera->start();
}

void CameraCapture::stop(){
    camera->stop();
}

void CameraCapture::processFrame(const QVideoFrame &frame){
    if(frame.isValid()){
        QImage image = frame.toImage();

        image = image.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        emit frameCaptured(image);
    }
}