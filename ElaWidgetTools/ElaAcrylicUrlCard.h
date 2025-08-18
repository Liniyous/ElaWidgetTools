#ifndef ELAACRYLICURLCARD_H
#define ELAACRYLICURLCARD_H

#include <QPixmap>
#include <QPushButton>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaAcrylicUrlCardPrivate;
class ELA_EXPORT ElaAcrylicUrlCard : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaAcrylicUrlCard);
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(qreal, MainOpacity)
    Q_PROPERTY_CREATE_Q_H(qreal, NoiseOpacity)
    Q_PROPERTY_CREATE_Q_H(int, BrushAlpha)
    Q_PROPERTY_CREATE_Q_H(QString, Title);
    Q_PROPERTY_CREATE_Q_H(QString, SubTitle);
    Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize);
    Q_PROPERTY_CREATE_Q_H(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_Q_H(int, TitleSpacing);
    Q_PROPERTY_CREATE_Q_H(int, SubTitleSpacing);
    Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_Q_H(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_Q_H(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_Q_H(ElaCardPixType::PixMode, CardPixMode);
    Q_PROPERTY_CREATE_Q_H(QString, Url);

public:
    explicit ElaAcrylicUrlCard(QWidget* parent = nullptr);
    ~ElaAcrylicUrlCard() override;
    void setCardPixmapSize(int width, int height);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAACRYLICURLCARD_H
