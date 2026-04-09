#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>
#include <QAudioSource>
#include <QAudioSink>
#include <QMediaDevices>
#include <QAudioFormat>
#include <QIODevice>

class AudioController: public QObject{
    Q_OBJECT

    public:
        explicit AudioController(QObject *parent = nullptr);
        ~AudioController();

        void start();
        void stop();

    public slots:
        void playAudioData(const QByteArray &data);

    signals:
        void audioDataCaptured(const QByteArray &data);

    private slots:
        void processMicrophoneData();

    private:
        QAudioSource *audioSource;
        QAudioSink *audioSink;
        QIODevice *micDevice;
        QIODevice *speakerDevice;
        QAudioFormat audioFormat;
};

#endif