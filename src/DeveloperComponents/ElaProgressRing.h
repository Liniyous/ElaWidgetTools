#ifndef ELAPROGRESSRING_H
#define ELAPROGRESSRING_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>

#include "ElaTheme.h"

class ElaProgressRing : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int startAngle READ startAngle WRITE setStartAngle)
    Q_PROPERTY(int spanAngle READ spanAngle WRITE setSpanAngle)

public:
    explicit ElaProgressRing(QWidget *parent = nullptr, bool start = true);
    ~ElaProgressRing() override = default;

    int startAngle() const;
    void setStartAngle(int angle);

    int spanAngle() const;
    void setSpanAngle(int angle);

    int strokeWidth() const;
    void setStrokeWidth(int width);

    QColor lightBarColor() const;
    QColor darkBarColor() const;

    void setCustomBarColor(const QColor &light, const QColor &dark);
    void setCustomBackgroundColor(const QColor &light, const QColor &dark);

    void start();
    void stop();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_lightBackgroundColor;
    QColor m_darkBackgroundColor;
    QColor m_lightBarColor;
    QColor m_darkBarColor;

    int m_strokeWidth;
    int m_startAngle;
    int m_spanAngle;

    QPropertyAnimation *m_startAngleAni1;
    QPropertyAnimation *m_startAngleAni2;
    QPropertyAnimation *m_spanAngleAni1;
    QPropertyAnimation *m_spanAngleAni2;

    QSequentialAnimationGroup *m_startAngleAniGroup;
    QSequentialAnimationGroup *m_spanAngleAniGroup;
    QParallelAnimationGroup *m_aniGroup;

    ElaThemeType::ThemeMode _themeMode;

    bool isDarkTheme() const;
};

#endif // ELAPROGRESSRING_H
