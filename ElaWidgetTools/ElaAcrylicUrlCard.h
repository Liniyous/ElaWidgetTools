#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAACRYLICURLCARD_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAACRYLICURLCARD_H_

#include <QPixmap>
#include <QPushButton>

#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaAcrylicUrlCardPrivate;
class ELA_EXPORT ElaAcrylicUrlCard : public QPushButton
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaAcrylicUrlCard);
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(qreal, MainOpacity)
    ELA_PROPERTY_Q_H(qreal, NoiseOpacity)
    ELA_PROPERTY_Q_H(int, BrushAlpha)
    ELA_PROPERTY_Q_H(QString, Title);
    ELA_PROPERTY_Q_H(QString, SubTitle);
    ELA_PROPERTY_Q_H(int, TitlePixelSize);
    ELA_PROPERTY_Q_H(int, SubTitlePixelSize);
    ELA_PROPERTY_Q_H(int, TitleSpacing);
    ELA_PROPERTY_Q_H(int, SubTitleSpacing);
    ELA_PROPERTY_Q_H(QPixmap, CardPixmap);
    ELA_PROPERTY_Q_H(QSize, CardPixmapSize);
    ELA_PROPERTY_Q_H(int, CardPixmapBorderRadius)
    ELA_PROPERTY_Q_H(ElaCardPixType::PixMode, CardPixMode);
    ELA_PROPERTY_Q_H(QString, Url);

public:
    explicit ElaAcrylicUrlCard(QWidget* parent = nullptr);
    ~ElaAcrylicUrlCard() override;
    void setCardPixmapSize(int width, int height);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAACRYLICURLCARD_H_
