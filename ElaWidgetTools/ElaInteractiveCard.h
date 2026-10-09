#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAINTERACTIVECARD_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAINTERACTIVECARD_H_

#include <QPixmap>
#include <QPushButton>

#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaInteractiveCardPrivate;
class ELA_EXPORT ElaInteractiveCard : public QPushButton
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaInteractiveCard)
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(QString, Title);
    ELA_PROPERTY_Q_H(QString, SubTitle);
    ELA_PROPERTY_Q_H(int, TitlePixelSize);
    ELA_PROPERTY_Q_H(int, SubTitlePixelSize);
    ELA_PROPERTY_Q_H(int, TitleSpacing);
    ELA_PROPERTY_Q_H(QPixmap, CardPixmap);
    ELA_PROPERTY_Q_H(QSize, CardPixmapSize);
    ELA_PROPERTY_Q_H(int, CardPixmapBorderRadius)
    ELA_PROPERTY_Q_H(ElaCardPixType::PixMode, CardPixMode);

public:
    explicit ElaInteractiveCard(QWidget* parent = nullptr);
    ~ElaInteractiveCard();
    void setCardPixmapSize(int width, int height);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAINTERACTIVECARD_H_
