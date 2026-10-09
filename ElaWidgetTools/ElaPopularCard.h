#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAPOPULARCARD_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAPOPULARCARD_H_

#include <QPixmap>
#include <QWidget>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaPopularCardPrivate;
class ELA_EXPORT ElaPopularCard : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaPopularCard)
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(QPixmap, CardPixmap)
    ELA_PROPERTY_Q_H(QString, Title)
    ELA_PROPERTY_Q_H(QString, SubTitle)
    ELA_PROPERTY_Q_H(QString, InteractiveTips)
    ELA_PROPERTY_Q_H(QString, DetailedText)
    ELA_PROPERTY_Q_H(QPixmap, CardFloatPixmap)
public:
    explicit ElaPopularCard(QWidget* parent = nullptr);
    ~ElaPopularCard() override;

    void setCardButtonText(const QString& cardButtonText);
    Q_REQUIRED_RESULT const QString& getCardButtonText() const;
    void setCardFloatArea(QWidget* floatArea);
    Q_REQUIRED_RESULT QWidget* getCardFloatArea() const;

Q_SIGNALS:
    Q_SIGNAL void pCardButtonTextChanged();
    Q_SIGNAL void pCardFloatAreaChanged();
    Q_SIGNAL void popularCardClicked();
    Q_SIGNAL void popularCardButtonClicked();

protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAPOPULARCARD_H_
