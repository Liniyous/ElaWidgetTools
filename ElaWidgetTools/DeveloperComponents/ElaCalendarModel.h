#ifndef ELACALENDARMODEL_H
#define ELACALENDARMODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QMetaType>

#include "ElaProperty.h"
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
    Q_PRIVATE_CREATE_Q_H(QDate, MinimumDate)
    Q_PRIVATE_CREATE_Q_H(QDate, MaximumDate)
public:
    explicit ElaCalendarModel(QObject* parent = nullptr);
    ~ElaCalendarModel();

    void setDisplayMode(ElaCalendarType displayType);
    ElaCalendarType getDisplayMode() const;

    QModelIndex getIndexFromDate(QDate date);
    QDate getDateFromIndex(const QModelIndex& index) const;
    virtual QVariant data(const QModelIndex& index, int role) const override;

Q_SIGNALS:
    Q_SIGNAL void currentYearMonthChanged(QString date);
    Q_SIGNAL void displayModeChanged();

protected:
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

private:
    int _offset{0};
    QDate _pMinimumDate;
    QDate _pMaximumDate;
    ElaCalendarType _displayMode{ElaCalendarType::DayMode};
    int _dayRowCount{0};
    void _initRowCount();
    int _getCurrentDay(int row) const;
};

#endif // ELACALENDARMODEL_H
