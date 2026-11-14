#include "ElaLogWriter.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <cstdio>

ElaLogWriter::ElaLogWriter(QObject* parent)
    : QThread(parent)
{
    setObjectName("ElaLogWriter");
    _fileChange.openMode = QIODevice::WriteOnly | QIODevice::Truncate;
}

ElaLogWriter::~ElaLogWriter()
{
    stopLogging();
}

void ElaLogWriter::setFilePath(const QString& filePath, bool isTruncate)
{
    QMutexLocker locker(&_mutex);
    _fileChange.content = filePath;
    _fileChange.openMode = QIODevice::WriteOnly | (isTruncate ? QIODevice::Truncate : QIODevice::Append);
    if (_isAccepting)
    {
        _appendFileChange();
        _workAvailable.wakeOne();
    }
}

void ElaLogWriter::startLogging()
{
    QMutexLocker locker(&_mutex);
    if (_isAccepting)
    {
        return;
    }
    _isAccepting = true;
    _isWriteSuccessful = true;
    _appendFileChange();
    start();
}

void ElaLogWriter::stopLogging()
{
    {
        QMutexLocker locker(&_mutex);
        _isAccepting = false;
        _workAvailable.wakeOne();
        _spaceAvailable.wakeAll();
    }
    wait();
}

bool ElaLogWriter::append(const QString& message)
{
    // 限制待写文本约 1 MiB，慢磁盘时施加背压；超长单条日志独占队列
    const qint64 bytes = static_cast<qint64>(message.size()) * sizeof(QChar);
    QMutexLocker locker(&_mutex);
    while (_isAccepting && !_entryDeque.empty() && _pendingBytes + bytes > 1024 * 1024)
    {
        _spaceAvailable.wait(&_mutex);
    }
    if (!_isAccepting)
    {
        return false;
    }
    _entryDeque.push_back({message});
    _pendingBytes += bytes;
    ++_submittedSequence;
    if (_entryDeque.size() == 1)
    {
        _workAvailable.wakeOne();
    }
    return true;
}

bool ElaLogWriter::flush()
{
    QMutexLocker locker(&_mutex);
    const quint64 sequence = _submittedSequence;
    while (_completedSequence < sequence)
    {
        _drained.wait(&_mutex);
    }
    return _isWriteSuccessful;
}

void ElaLogWriter::run()
{
    // QFile 只在写入线程内创建、操作和销毁
    QFile file;
    bool isFileErrorReported = false;
    for (;;)
    {
        std::deque<ElaLogEntry> entries;
        quint64 sequence = 0;
        {
            QMutexLocker locker(&_mutex);
            while (_entryDeque.empty() && _isAccepting)
            {
                _workAvailable.wait(&_mutex);
            }
            if (_entryDeque.empty())
            {
                break;
            }
            entries.swap(_entryDeque);
            sequence = _submittedSequence;
            _pendingBytes = 0;
            _spaceAvailable.wakeAll();
        }

        QByteArray fileBuffer;
        QByteArray consoleBuffer;
        // 累积失败状态，同时继续执行后续文件切换和写入
        bool isSuccessful = true;
        for (const ElaLogEntry& entry: entries)
        {
            if (entry.openMode != QIODevice::NotOpen)
            {
                isSuccessful &= _writeBuffer(file, fileBuffer, isFileErrorReported);
                file.close();
                file.setFileName(entry.content);
                QDir().mkpath(QFileInfo(file).absolutePath());
                isFileErrorReported = false;
                isSuccessful &= _checkFileResult(file, file.open(entry.openMode), isFileErrorReported);
            }
            else
            {
                fileBuffer.append(entry.content.toUtf8());
                fileBuffer.append('\n');
                consoleBuffer.append(entry.content.toLocal8Bit());
                consoleBuffer.append('\n');
            }
        }
        isSuccessful &= _writeBuffer(file, fileBuffer, isFileErrorReported);
        if (!consoleBuffer.isEmpty())
        {
            std::fwrite(consoleBuffer.constData(), 1, static_cast<size_t>(consoleBuffer.size()), stderr);
            std::fflush(stderr);
        }
        {
            QMutexLocker locker(&_mutex);
            _isWriteSuccessful = _isWriteSuccessful && isSuccessful;
            _completedSequence = sequence;
            _drained.wakeAll();
        }
    }
}

void ElaLogWriter::_appendFileChange()
{
    _entryDeque.push_back(_fileChange);
    _fileChange.openMode = QIODevice::WriteOnly | QIODevice::Append;
    ++_submittedSequence;
}

bool ElaLogWriter::_writeBuffer(QFile& file, QByteArray& buffer, bool& isFileErrorReported)
{
    if (buffer.isEmpty())
    {
        return true;
    }
    bool isSuccessful = file.isOpen();
    qint64 offset = 0;
    while (isSuccessful && offset < buffer.size())
    {
        const qint64 written = file.write(buffer.constData() + offset, buffer.size() - offset);
        isSuccessful = written > 0;
        offset += qMax<qint64>(written, 0);
    }
    if (isSuccessful)
    {
        isSuccessful = file.flush();
    }
    buffer.clear();
    return _checkFileResult(file, isSuccessful, isFileErrorReported);
}

bool ElaLogWriter::_checkFileResult(const QFile& file, bool isSuccessful, bool& isFileErrorReported)
{
    if (!isSuccessful && !isFileErrorReported)
    {
        // 每个文件只报告首次失败，避免再次进入 Qt 日志处理器
        const QByteArray error = QString("ElaLog 文件写入失败：%1 (%2)\n").arg(file.fileName(), file.errorString()).toLocal8Bit();
        std::fwrite(error.constData(), 1, static_cast<size_t>(error.size()), stderr);
        isFileErrorReported = true;
    }
    return isSuccessful;
}
