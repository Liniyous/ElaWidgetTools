#ifndef ELAREMINDERCARD_H
#define ELAREMINDERCARD_H

#include <QPushButton>

#include "Def.h"
class ElaReminderCardPrivate;
class ELA_EXPORT ElaReminderCard : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaReminderCard)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Title);
    Q_PROPERTY_CREATE_Q_H(QString, SubTitle);
    Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize);
    Q_PROPERTY_CREATE_Q_H(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_Q_H(int, TitleSpacing);
    Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_Q_H(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_Q_H(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_Q_H(ElaCardPixType::PixMode, CardPixMode);

public:
    explicit ElaReminderCard(QWidget* parent = nullptr);
    ~ElaReminderCard();
    void setCardPixmapSize(int width, int height);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAREMINDERCARD_H
