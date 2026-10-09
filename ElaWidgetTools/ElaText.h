#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATEXT_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATEXT_H_

#include <QLabel>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
class ElaTextPrivate;
class ELA_EXPORT ElaText : public QLabel
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaText)
public:
    explicit ElaText(QWidget* parent = nullptr);
    explicit ElaText(const QString& text, QWidget* parent = nullptr);
    explicit ElaText(const QString& text, int pixelSize, QWidget* parent = nullptr);
    ~ElaText() override;

    void setIsWrapAnywhere(bool isWrapAnywhere);
    Q_REQUIRED_RESULT bool getIsWrapAnywhere() const;
    void setIsBold(bool isBold);
    Q_REQUIRED_RESULT bool getIsBold() const;
    void setTextPixelSize(int size);
    Q_REQUIRED_RESULT int getTextPixelSize() const;
    void setTextPointSize(int size);
    Q_REQUIRED_RESULT int getTextPointSize() const;
    void setTextStyle(ElaTextType::TextStyle textStyle);
    Q_REQUIRED_RESULT ElaTextType::TextStyle getTextStyle() const;
    void setElaIcon(ElaIconType::IconName elaIcon);
    Q_REQUIRED_RESULT ElaIconType::IconName getElaIcon() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;

Q_SIGNALS:
    Q_SIGNAL void pIsWrapAnywhereChanged();
    Q_SIGNAL void pIsBoldChanged();
    Q_SIGNAL void pTextPixelSizeChanged();
    Q_SIGNAL void pTextPointSizeChanged();
    Q_SIGNAL void pTextStyleChanged();
    Q_SIGNAL void pElaIconChanged();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATEXT_H_
