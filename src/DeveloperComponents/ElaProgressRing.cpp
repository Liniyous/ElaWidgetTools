#include "ElaProgressRing.h"

ElaProgressRing::ElaProgressRing(QWidget *parent, bool start)
    : QWidget(parent),
    m_lightBackgroundColor(Qt::transparent),
    m_darkBackgroundColor(Qt::transparent),
    m_strokeWidth(6),
    m_startAngle(0),
    m_spanAngle(0)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });

    m_startAngleAni1 = new QPropertyAnimation(this, "startAngle");
    m_startAngleAni2 = new QPropertyAnimation(this, "startAngle");
    m_spanAngleAni1 = new QPropertyAnimation(this, "spanAngle");
    m_spanAngleAni2 = new QPropertyAnimation(this, "spanAngle");

    m_startAngleAniGroup = new QSequentialAnimationGroup(this);
    m_spanAngleAniGroup = new QSequentialAnimationGroup(this);
    m_aniGroup = new QParallelAnimationGroup(this);

    // Initialize start angle animation
    m_startAngleAni1->setDuration(1000);
    m_startAngleAni1->setStartValue(0);
    m_startAngleAni1->setEndValue(450);

    m_startAngleAni2->setDuration(1000);
    m_startAngleAni2->setStartValue(450);
    m_startAngleAni2->setEndValue(1080);

    m_startAngleAniGroup->addAnimation(m_startAngleAni1);
    m_startAngleAniGroup->addAnimation(m_startAngleAni2);

    // Initialize span angle animation
    m_spanAngleAni1->setDuration(1000);
    m_spanAngleAni1->setStartValue(0);
    m_spanAngleAni1->setEndValue(180);

    m_spanAngleAni2->setDuration(1000);
    m_spanAngleAni2->setStartValue(180);
    m_spanAngleAni2->setEndValue(0);

    m_spanAngleAniGroup->addAnimation(m_spanAngleAni1);
    m_spanAngleAniGroup->addAnimation(m_spanAngleAni2);

    m_aniGroup->addAnimation(m_startAngleAniGroup);
    m_aniGroup->addAnimation(m_spanAngleAniGroup);
    m_aniGroup->setLoopCount(-1);

    setFixedSize(80, 80);

    if (start)
        this->start();
}

int ElaProgressRing::startAngle() const
{
    return m_startAngle;
}

void ElaProgressRing::setStartAngle(int angle)
{
    m_startAngle = angle;
    update();
}

int ElaProgressRing::spanAngle() const
{
    return m_spanAngle;
}

void ElaProgressRing::setSpanAngle(int angle)
{
    m_spanAngle = angle;
    update();
}

int ElaProgressRing::strokeWidth() const
{
    return m_strokeWidth;
}

void ElaProgressRing::setStrokeWidth(int width)
{
    m_strokeWidth = width;
    update();
}

QColor ElaProgressRing::lightBarColor() const
{
    return m_lightBarColor.isValid() ? m_lightBarColor : QColor("#4ECAFF"); // Default to blue
}

QColor ElaProgressRing::darkBarColor() const
{
    return m_darkBarColor.isValid() ? m_darkBarColor : QColor("#4ECAFF"); // Default to blue
}

void ElaProgressRing::setCustomBarColor(const QColor &light, const QColor &dark)
{
    m_lightBarColor = light;
    m_darkBarColor = dark;
    update();
}

void ElaProgressRing::setCustomBackgroundColor(const QColor &light, const QColor &dark)
{
    m_lightBackgroundColor = light;
    m_darkBackgroundColor = dark;
    update();
}

void ElaProgressRing::start()
{
    m_startAngle = 0;
    m_spanAngle = 0;
    m_aniGroup->start();
}

void ElaProgressRing::stop()
{
    m_aniGroup->stop();
    m_startAngle = 0;
    m_spanAngle = 0;
    update();
}

void ElaProgressRing::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);


    int cw = m_strokeWidth;
    int w = qMin(height(), width()) - cw;
    QRectF rc(cw / 2.0, height() / 2.0 - w / 2.0, w, w);

    // Draw background
    QColor bc = isDarkTheme() ? m_darkBackgroundColor : m_lightBackgroundColor;
    QPen pen(bc, cw, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    painter.setPen(pen);
    painter.drawArc(rc, 0, 360 * 16);

    // Draw bar
    pen.setColor(ElaThemeColor(_themeMode, PrimaryNormal));
    painter.setPen(pen);

    int startAngle = -m_startAngle + 180;
    painter.drawArc(rc, (startAngle % 360) * 16, -m_spanAngle * 16);
}

bool ElaProgressRing::isDarkTheme() const
{
    return _themeMode == ElaThemeType::Dark;
}
