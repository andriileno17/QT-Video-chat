#include "AudioController.h"
#include <QDebug>

AudioController::AudioController(QObject *parent)
    :QObject(parent),
    micDevice(nullptr),
    speakerDevice(nullptr){

    audioFormat.setSampleRate(16000);
    audioFormat.setChannelCount(1);
    audioFormat.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice infoIn = QMediaDevices::defaultAudioInput();
    if(!infoIn.isFormatSupported(audioFormat)){
        qWarning() << "Мікрофон не підтримує стандартний формат!";
        audioFormat = infoIn.preferredFormat();
    }

    QAudioDevice infoOut = QMediaDevices::defaultAudioOutput();
    if(!infoOut.isFormatSupported(audioFormat)){
        qWarning() << "Динамік не підтримує стандартний формат!";
        audioFormat = infoOut.preferredFormat();
    }

    audioSource = new QAudioSource(infoIn, audioFormat, this);
    audioSink = new QAudioSink(infoOut, audioFormat, this);
}

AudioController::~AudioController(){
    stop();
}

void AudioController::start(){
    speakerDevice = audioSink->start();
    micDevice = audioSource->start();

    connect(micDevice, &QIODevice::readyRead, this, &AudioController::processMicrophoneData);
}

void AudioController::stop(){
    audioSource->stop();
    audioSink->stop();
}

void AudioController::processMicrophoneData() {
    if (!micDevice) return;
    
    QByteArray data = micDevice->readAll();
    if (data.size() > 0) {
        emit audioDataCaptured(data);
    }
}

void AudioController::playAudioData(const QByteArray &data){
    if(speakerDevice && audioSink->state() != QAudio::StoppedState){
        speakerDevice->write(data);
    }
}