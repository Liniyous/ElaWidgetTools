#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELALOGWRITER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELALOGWRITER_H_

#include <QIODevice>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <deque>

class QFile;
class ElaLogWriter : public QThread
{
    Q_OBJECT
public:
    explicit ElaLogWriter(QObject* parent = nullptr);
    ~ElaLogWriter() override;

    void setFilePath(const QString& filePath, bool isTruncate);
    void startLogging();
    void stopLogging();
    bool append(const QString& message);
    bool flush();

protected:
    void run() override;

private:
    struct ElaLogEntry {
        QString content;                                  // 普通项为完整日志，文件切换项为目标路径
        QIODevice::OpenMode openMode{QIODevice::NotOpen}; // NotOpen 为日志项，其余为文件切换时的打开模式
    };

    void _appendFileChange();
    static bool _writeBuffer(QFile& file, QByteArray& buffer, bool& isFileErrorReported);
    static bool _checkFileResult(const QFile& file, bool isSuccessful, bool& isFileErrorReported);

    QMutex _mutex;
    QWaitCondition _workAvailable;
    QWaitCondition _spaceAvailable;
    QWaitCondition _drained;
    std::deque<ElaLogEntry> _entryDeque;
    ElaLogEntry _fileChange;
    qint64 _pendingBytes{0};
    quint64 _submittedSequence{0};
    quint64 _completedSequence{0};
    bool _isAccepting{false};
    bool _isWriteSuccessful{true};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELALOGWRITER_H_
