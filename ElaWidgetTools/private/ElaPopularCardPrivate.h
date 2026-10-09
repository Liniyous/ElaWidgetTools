#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPOPULARCARDPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPOPULARCARDPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>
#include <QPixmap>

#include "ElaWidgetToolsDef.h"
class ElaPopularCard;
class ElaPopularCardFloater;
class ElaPopularCardPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaPopularCard)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(QPixmap, CardPixmap)
    ELA_PROPERTY_D(QString, Title)
    ELA_PROPERTY_D(QString, SubTitle)
    ELA_PROPERTY_D(QString, InteractiveTips)
    ELA_PROPERTY_D(QString, DetailedText)
    ELA_PROPERTY_D(QString, CardButtonText)
    ELA_PROPERTY_D(QWidget*, CardFloatArea)
    ELA_PROPERTY_D(QPixmap, CardFloatPixmap)
    ELA_PROPERTY(qreal, HoverYOffset);
    ELA_PROPERTY(qreal, HoverOpacity);

public:
    explicit ElaPopularCardPrivate(QObject* parent = nullptr);
    ~ElaPopularCardPrivate();

private:
    friend class ElaPopularCardFloater;
    ElaThemeType::ThemeMode _themeMode;
    QTimer* _floatTimer{nullptr};
    ElaPopularCardFloater* _floater{nullptr};
    bool _isFloating{false};
    int _shadowBorderWidth{6};
    int _textHSpacing{20};
    int _textVSpacing{5};
    QRectF _interactiveTipsBaseRect;
    QRect _buttonTargetRect;

    void _showFloater();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPOPULARCARDPRIVATE_H_

