#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAIMAGECARD_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAIMAGECARD_H_

#include <QWidget>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"

class ElaImageCardPrivate;
class ELA_EXPORT ElaImageCard : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaImageCard)
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(bool, IsPreserveAspectCrop)
public:
    explicit ElaImageCard(QWidget* parent = nullptr);
    ~ElaImageCard() override;

    void setCardImage(const QImage& cardImage);
    Q_REQUIRED_RESULT const QImage& getCardImage() const;

protected:
    void paintEvent(QPaintEvent* event) override;

Q_SIGNALS:
    Q_SIGNAL void pCardImageChanged();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAIMAGECARD_H_
