#include "audio_file.h"

#include <QUrl>

AudioFile::AudioFile(QObject *parent)
    : QIODevice(parent)
    , m_file{}
    , m_decoder{}
    , m_format{}
    , m_buffer{}
    , m_position{}
    , m_begin{}
    , m_end{}
{
    connect(&m_decoder, &QAudioDecoder::bufferReady, this, &AudioFile::onBufferReady);
    connect(&m_decoder, &QAudioDecoder::finished, this, &AudioFile::onFinished);
}

AudioFile::AudioFile(const QString &fileName, QObject *parent)
    : QIODevice(parent)
    , m_file{fileName}
    , m_decoder{}
    , m_format{}
    , m_buffer{}
{
    connect(&m_decoder, &QAudioDecoder::bufferReady, this, &AudioFile::onBufferReady);
    connect(&m_decoder, &QAudioDecoder::finished, this, &AudioFile::onFinished);
    m_decoder.setSource(m_file.fileName());
    m_decoder.start();
}

AudioFile::~AudioFile()
{
    QIODevice::close();

    if(m_decoder.isDecoding())
    {
        m_decoder.stop();
    }

    if(m_file.isOpen())
    {
        m_file.close();
    }
}

bool AudioFile::open(OpenMode mode)
{
    return QIODevice::open(mode);
}

void AudioFile::close()
{
    QIODevice::close();
    m_decoder.stop();
    m_file.close();
    m_position = m_begin;
}

qint64 AudioFile::bytesAvailable() const
{
    auto result{std::distance(m_position, m_end) + QIODevice::bytesAvailable()};
    result = result > 0 ? result : -1;
    return result;
}

bool AudioFile::canReadLine() const
{
    return false;
}

QString AudioFile::getFile() const
{
    return m_file.fileName();
}

void AudioFile::setFile(const QString &file)
{
    if (m_file.fileName() == file) {
        return;
    }

    m_file.setFileName(file);
    m_decoder.setSource(m_file.fileName());
    m_decoder.start();
    emit fileChanged();
}

QAudioFormat AudioFile::getFormat() const
{
    return m_format;
}

qint64 AudioFile::readData(char *data, qint64 maxSize)
{
    if (m_position == m_end)
    {
        return -1;
    }

    qint64 availableSize = std::distance(m_position, m_end);
    qint64 bytesRead = qMin(maxSize, availableSize);

    memcpy(data, m_position, static_cast<size_t>(bytesRead));
    std::advance(m_position, bytesRead);

    return bytesRead;
}

qint64 AudioFile::writeData(const char *data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)
    return -1;
}

void AudioFile::onBufferReady()
{
    QAudioBuffer buffer = m_decoder.read();
    if (buffer.format() != m_format) {
        m_format = buffer.format();
    }

    m_buffer.append(buffer.constData<char>(), buffer.byteCount());
    m_position = m_buffer.cbegin();
    m_begin = m_buffer.cbegin();
    m_end = m_buffer.cend();
    emit readyRead();
}

void AudioFile::onFinished()
{
    m_decoder.stop();
    m_file.close();

    emit readyRead();
}
