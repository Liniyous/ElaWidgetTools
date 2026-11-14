#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_LOGMODEL_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_LOGMODEL_H_

#include <QAbstractListModel>

class T_LogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    static constexpr int MaximumLogCount = 10000;

    explicit T_LogModel(QObject* parent = nullptr);
    ~T_LogModel();
    void setLogList(QStringList list);
    void appendLogList(QString log);
    void appendLogs(const QStringList& logs);
    QStringList getLogList() const;

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QStringList _logList;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_LOGMODEL_H_
