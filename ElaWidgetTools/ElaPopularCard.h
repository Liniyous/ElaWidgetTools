#ifndef ELAPOPULARCARD_H
#define ELAPOPULARCARD_H

#include <QPixmap>
#include <QWidget>

#include "ElaProperty.h"
class ElaPopularCardPrivate;
class ELA_EXPORT ElaPopularCard : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaPopularCard)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, SubTitle)
    Q_PROPERTY_CREATE_Q_H(QString, InteractiveTips)
    Q_PROPERTY_CREATE_Q_H(QString, DetailedText)
    Q_PROPERTY_CREATE_Q_H(QString, CardButtonText)
    Q_PROPERTY_CREATE_Q_H(QWidget*, CardFloatArea)
    Q_PROPERTY_CREATE_Q_H(QPixmap, CardFloatPixmap)
public:
    explicit ElaPopularCard(QWidget* parent = nullptr);
    ~ElaPopularCard() override;
Q_SIGNALS:
    Q_SIGNAL void popularCardClicked();
    Q_SIGNAL void popularCardButtonClicked();

protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAPOPULARCARD_H
