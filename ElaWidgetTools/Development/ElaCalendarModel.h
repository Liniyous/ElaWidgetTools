#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACALENDARMODEL_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACALENDARMODEL_H_

#include <QAbstractListModel>
#include <QDate>
#include <QMetaType>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
enum ElaCalendarType
{
    YearMode = 0x0001,
    MonthMode = 0x0002,
    DayMode = 0x0003,
};

struct ElaCalendarData : public QObjectData {
public:
    ElaCalendarData(){};
    ~ElaCalendarData(){};
    ElaCalendarData(int year, int month, int day, QString desText = "")
        : year(year), month(month), day(day), desText(desText){};
    ElaCalendarData(const ElaCalendarData& other)
    {
        year = other.year;
        month = other.month;
        day = other.day;
        desText = other.desText;
    }
    int year = 1924;
    int month = 1;
    int day = 1;
    QString desText{""};
};
Q_DECLARE_METATYPE(ElaCalendarData);

class ElaCalendarModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ElaCalendarModel(QObject* parent = nullptr);
    ~ElaCalendarModel();

    void setMinimumDate(QDate minimudate);
    Q_REQUIRED_RESULT QDate getMinimumDate() const;
    void setMaximumDate(QDate maximumDate);
    Q_REQUIRED_RESULT QDate getMaximumDate() const;

    void setDisplayMode(ElaCalendarType displayType);
    ElaCalendarType getDisplayMode() const;

    QModelIndex getIndexFromDate(QDate date);
    QDate getDateFromIndex(const QModelIndex& index) const;
    bool ensureDateVisible(QDate date, bool isAlignToPage = false);
    virtual QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

Q_SIGNALS:
    Q_SIGNAL void currentYearMonthChanged(QString date);
    Q_SIGNAL void displayModeChanged();

protected:
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

private:
    QDate _pMinimumDate;
    QDate _pMaximumDate;
    QDate _windowStartDate; // 视图窗口起点 可选日期范围独立保存
    ElaCalendarType _displayMode{ElaCalendarType::DayMode};
    int _rowCount{0};
    QDate _getWindowStartDate(QDate date) const;
    void _updateRowCount();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACALENDARMODEL_H_

