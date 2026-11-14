#include "ElaTheme.h"

#include "ElaColorAnimation.h"
#include "ElaThemePrivate.h"

#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <qdrawutil.h>

ELA_SINGLETON_CPP(ElaTheme)
ElaTheme::ElaTheme(QObject* parent)
    : QObject{parent}, d_ptr(new ElaThemePrivate())
{
    Q_D(ElaTheme);
    d->q_ptr = this;
}

ElaTheme::~ElaTheme()
{
}

void ElaTheme::setThemeMode(ElaThemeType::ThemeMode themeMode)
{
    Q_D(ElaTheme);
    d->_themeMode = themeMode;
    ElaColorAnimation::beginThemeTransition();
    Q_EMIT themeModeChanged(d->_themeMode);
}

ElaThemeType::ThemeMode ElaTheme::getThemeMode() const
{
    Q_D(const ElaTheme);
    return d->_themeMode;
}

QMargins ElaTheme::getShadowMargins(int shadowBorderWidth) const
{
    const int width = qMax(0, shadowBorderWidth);
    return QMargins(qCeil(width * 0.75), qCeil(width * 0.5), qCeil(width * 0.75), width);
}

void ElaTheme::drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius, qreal alphaRatio, const QColor& shadowColor)
{
    drawEffectShadow(painter, widgetRect, QMargins(shadowBorderWidth, shadowBorderWidth, shadowBorderWidth, shadowBorderWidth), borderRadius, alphaRatio, shadowColor);
}

void ElaTheme::drawEffectShadow(QPainter* painter, QRect widgetRect, const QMargins& shadowMargins, int borderRadius, qreal alphaRatio, const QColor& shadowColor)
{
    Q_D(ElaTheme);
    const QRect body = widgetRect.marginsRemoved(shadowMargins);
    const int shadowBorderWidth = qMax(qMax(shadowMargins.left(), shadowMargins.right()), qMax(shadowMargins.top(), shadowMargins.bottom()));
    if (!painter || !painter->isActive() || shadowBorderWidth <= 0 || body.isEmpty() || alphaRatio <= 0)
    {
        return;
    }
    const qreal ratio = painter->device()->devicePixelRatioF();
    const int radius = qBound(0, borderRadius, qMin(body.width(), body.height()) / 2);
    // 圆角和模糊过渡保持原尺寸 仅拉伸直边 避免窗口缩放或卡片动画反复生成阴影
    const int edge = radius + shadowBorderWidth;
    const QSize bodySize(qMin(body.width(), 2 * edge + 1), qMin(body.height(), 2 * edge + 1));
    const QSize size = bodySize + QSize(shadowMargins.left() + shadowMargins.right(), shadowMargins.top() + shadowMargins.bottom());
    QPainterPath path;
    path.addRoundedRect(QRectF(QPointF(shadowMargins.left(), shadowMargins.top()), bodySize), radius, radius);
    const QPixmap shadow = d->_getShadowPixmap(path, size, ratio, shadowBorderWidth, qMax(qreal(1), alphaRatio), shadowColor);
    const int horizontalEdge = qMin(edge, bodySize.width() / 2);
    const int verticalEdge = qMin(edge, bodySize.height() / 2);
    const QMargins edges = shadowMargins + QMargins(horizontalEdge, verticalEdge, horizontalEdge, verticalEdge);
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setOpacity(painter->opacity() * qMin(qreal(1), alphaRatio));
    // 中心完全透明 裁去这部分避免九宫格拉伸时仍遍历面板内部像素
    const QRect center = widgetRect.marginsRemoved(edges);
    painter->setClipRegion(QRegion(widgetRect).subtracted(QRegion(center)), Qt::IntersectClip);
    qDrawBorderPixmap(painter, widgetRect, edges, shadow, QRect(QPoint(), size * ratio),
                      QMargins(qRound(edges.left() * ratio), qRound(edges.top() * ratio), qRound(edges.right() * ratio), qRound(edges.bottom() * ratio)));
    painter->restore();
}

void ElaTheme::drawEffectShadow(QPainter* painter, const QPainterPath& contentPath, int shadowBorderWidth, qreal alphaRatio, const QColor& shadowColor)
{
    Q_D(ElaTheme);
    if (!painter || !painter->isActive() || contentPath.isEmpty() || shadowBorderWidth <= 0 || alphaRatio <= 0)
    {
        return;
    }
    const QRect rect = contentPath.boundingRect().marginsAdded(QMarginsF(getShadowMargins(shadowBorderWidth))).toAlignedRect();
    const QPainterPath path = contentPath.translated(-rect.topLeft());
    const QPixmap shadow = d->_getShadowPixmap(path, rect.size(), painter->device()->devicePixelRatioF(), shadowBorderWidth, qMax(qreal(1), alphaRatio), shadowColor);
    painter->save();
    painter->setOpacity(painter->opacity() * qMin(qreal(1), alphaRatio));
    painter->drawPixmap(rect.topLeft(), shadow);
    painter->restore();
}

void ElaTheme::setThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor, const QColor& newColor)
{
    Q_D(ElaTheme);
    if (themeColor < 0 || themeColor >= ElaThemeType::ThemeColorCount || !newColor.isValid())
    {
        return;
    }
    QColor* colors = themeMode == ElaThemeType::Light ? d->_lightThemeColorList : d->_darkThemeColorList;
    if (colors[themeColor] == newColor)
    {
        return;
    }
    colors[themeColor] = newColor;
    if (themeColor == ElaThemeType::PrimaryNormal)
    {
        colors[ElaThemeType::PrimaryHover] = newColor;
        colors[ElaThemeType::PrimaryHover].setAlphaF(newColor.alphaF() * 0.9);
        colors[ElaThemeType::PrimaryPress] = newColor;
        colors[ElaThemeType::PrimaryPress].setAlphaF(newColor.alphaF() * 0.8);
    }
    if (themeMode == d->_themeMode)
    {
        ElaColorAnimation::beginThemeTransition();
        Q_EMIT themeModeChanged(themeMode);
    }
}

const QColor& ElaTheme::getThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor)
{
    Q_D(ElaTheme);
    if (themeColor < 0 || themeColor >= ElaThemeType::ThemeColorCount)
    {
        themeColor = ElaThemeType::BasicBase;
    }
    if (themeMode == ElaThemeType::Light)
    {
        return d->_lightThemeColorList[themeColor];
    }
    else
    {
        return d->_darkThemeColorList[themeColor];
    }
}
