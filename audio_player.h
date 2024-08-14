#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QObject>
#include <QQmlEngine>

#include <audio_file.h>

class AudioPlayer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(AudioFile *audioFile READ getAudioFile WRITE setAudioFile NOTIFY audioFileChanged)

public:
    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer() override;

    [[nodiscard]]
    AudioFile *getAudioFile() const;
    void setAudioFile(AudioFile *audioFile);

    Q_INVOKABLE void play();

signals:
    void audioFileChanged();

private:
    AudioFile *m_audioFile;
};

#endif // AUDIO_PLAYER_H
