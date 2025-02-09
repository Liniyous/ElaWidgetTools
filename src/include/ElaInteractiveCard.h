#ifndef ELAINTERACTIVECARD_H
#define ELAINTERACTIVECARD_H

#include <QPixmap>
#include <QPushButton>

#include "Def.h"
#include "stdafx.h"

class ElaInteractiveCardPrivate;
class ELA_EXPORT ElaInteractiveCard : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaInteractiveCard)
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
    explicit ElaInteractiveCard(QWidget* parent = nullptr);
    ~ElaInteractiveCard();
    void setCardPixmapSize(int width, int height);

    // fuqaq : add time display area
    void setTimeContent(const QString& content,const QColor& color,const QFont& font);

    // fuqaq : add status display area
    void setStatusContent(const QString& content,const QFont& font,int xOffset = 0);

    void changeStatus(bool _curStatus);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString _timeContent;
    QString _statusContent;
    QColor _timeContentColor;
    QColor _statusContentColor;
    QFont _timeContentFont;
    QFont _statusContentFont;

    int _statusXOffset;
    bool _statusChecked = false;
};

#endif // ELAINTERACTIVECARD_H
