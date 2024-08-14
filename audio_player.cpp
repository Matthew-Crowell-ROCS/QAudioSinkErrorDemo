#include "audio_player.h"

#include <QAudioSink>
#include <QEventLoop>

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject{parent}
{}

AudioPlayer::~AudioPlayer()
{
}

AudioFile *AudioPlayer::getAudioFile() const
{
    return m_audioFile;
}

void AudioPlayer::setAudioFile(AudioFile *audioFile)
{
    if (m_audioFile == audioFile)
    {
        return;
    }

    m_audioFile = audioFile;
    emit audioFileChanged();
}

void AudioPlayer::play()
{
    if (!m_audioFile || m_audioFile->buffer().isEmpty())
    {
        qWarning() << "Audio file is empty";
    }

    if (!m_audioFile->open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open audio file" << m_audioFile->getFile();
        return;
    }

    QAudioSink* audioSink = new QAudioSink(m_audioFile->getFormat(), this);
    audioSink->setVolume(1.0);


    if(!m_audioFile->isOpen()) {
        qWarning() << "Audio file is not open!";
        m_audioFile->open(QIODevice::ReadOnly);
    }

    audioSink->start(m_audioFile);

    if (audioSink->error() != QAudio::NoError)
    {
        qWarning() << "AudioSink error:" << audioSink->error();
        return;
    }

    QEventLoop loop;
    connect(audioSink, &QAudioSink::stateChanged, [this, &loop, &audioSink](QAudio::State state)
            {
                if (state == QAudio::IdleState || state == QAudio::StoppedState)
                {
                    m_audioFile->close();
                    loop.quit();
                }

                if(audioSink->error() != QAudio::NoError)
                {
                    qWarning() << "AudioSink error:" << audioSink->error();
                    loop.quit();
                }
            });

    loop.exec();
}



