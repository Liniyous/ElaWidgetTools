#ifndef ELAPOPULARCARDPRIVATE_H
#define ELAPOPULARCARDPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "ElaDef.h"
class ElaPopularCard;
class ElaPopularCardFloater;
class ElaPopularCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPopularCard)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QPixmap, CardPixmap)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, SubTitle)
    Q_PROPERTY_CREATE_D(QString, InteractiveTips)
    Q_PROPERTY_CREATE_D(QString, DetailedText)
    Q_PROPERTY_CREATE_D(QString, CardButtonText)
    Q_PROPERTY_CREATE_D(QWidget*, CardFloatArea)
    Q_PROPERTY_CREATE_D(QPixmap, CardFloatPixmap)
    Q_PROPERTY_CREATE(qreal, HoverYOffset);
    Q_PROPERTY_CREATE(qreal, HoverOpacity);

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

#endif // ELAPOPULARCARDPRIVATE_H
