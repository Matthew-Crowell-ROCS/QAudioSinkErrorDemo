#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include <QIODevice>
#include <QObject>
#include <QFile>
#include <QAudioDecoder>
#include <QAudioFormat>
#include <QAudioBuffer>

#include <QtQml/qqmlregistration.h>

class AudioFile : public QIODevice
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString file READ getFile WRITE setFile NOTIFY fileChanged)

public:
    explicit AudioFile(QObject *parent = nullptr);
    AudioFile(const QString &fileName, QObject *parent = nullptr);
    ~AudioFile() override;

    bool open(OpenMode mode) override;
    void close() override;
    [[nodiscard]]
    qint64 bytesAvailable() const override;
    [[nodiscard]]
    bool canReadLine() const override;
    bool isSequential() const override { return true; }
    QByteArray buffer() {return m_buffer;}

    [[nodiscard]]
    QString getFile() const;
    void setFile(const QString &file);

    [[nodiscard]]
    QAudioFormat getFormat() const;

signals:
    void fileChanged();

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private slots:
    void onBufferReady();
    void onFinished();

private:
    QFile m_file;
    QAudioDecoder m_decoder;
    QAudioFormat m_format;
    QByteArray m_buffer;
    QByteArray::const_iterator m_position;
    QByteArray::const_iterator m_begin;
    QByteArray::const_iterator m_end;
};

#endif // AUDIO_FILE_H
