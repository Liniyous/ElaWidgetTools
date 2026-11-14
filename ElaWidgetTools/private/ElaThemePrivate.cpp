#include "ElaThemePrivate.h"

#include "ElaExponentialBlur.h"

#include <QDataStream>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

ElaThemePrivate::ElaThemePrivate(QObject* parent)
    : QObject(parent)
{
    // 按 ThemeColor 顺序集中维护浅色和深色配对 透明色叠加到实际背景
    const QRgb colors[][2] = {
        {0xFFF3F3F3, 0xFF202020}, // 窗口底色
        {0xFFF9F9F9, 0xFF2C2C2C}, // 浮层与对话框底色
        {0xB3FFFFFF, 0x0DFFFFFF}, // 卡片与内容层
        {0xE4000000, 0xFFFFFFFF}, // 正文与图标
        {0x9E000000, 0xC5FFFFFF}, // 次要与按压文字
        {0x5C000000, 0x5DFFFFFF}, // 禁用文字
        {0xFFFFFFFF, 0xFF000000}, // 强调色上的文字
        {0xB3FFFFFF, 0x0FFFFFFF}, // 普通控件填充
        {0x80F9F9F9, 0x15FFFFFF}, // 普通控件悬停填充
        {0x4DF9F9F9, 0x08FFFFFF}, // 普通控件按压填充
        {0x4DF9F9F9, 0x0BFFFFFF}, // 普通控件禁用填充
        {0xFFFFFFFF, 0xB31E1E1E}, // 输入控件获得焦点时的填充
        {0x09000000, 0x0FFFFFFF}, // 透明控件悬停与选中填充
        {0x06000000, 0x0AFFFFFF}, // 透明控件按压与选中悬停填充
        {0x0F000000, 0x12FFFFFF}, // 普通轮廓
        {0x29000000, 0x18FFFFFF}, // 控件次级轮廓与分隔线
        {0x72000000, 0x8BFFFFFF}, // 输入底线与强轮廓
        {0x72000000, 0x8BFFFFFF}, // 滚动条与滑轨等中性指示器
        {0xFF0067C0, 0xFF60CDFF}, // 统一强调色
        {0xE60067C0, 0xE660CDFF}, // 强调色悬停
        {0xCC0067C0, 0xCC60CDFF}, // 强调色按压
        {0xFF0F7B0F, 0xFF6CCB5F}, // 成功状态
        {0xFFDFF6DD, 0xFF393D1B}, // 成功提示底色
        {0xFF9D5D00, 0xFFFCE100}, // 警告状态
        {0xFFFFF4CE, 0xFF433519}, // 警告提示底色
        {0xFFC42B1C, 0xFFFF99A4}, // 错误状态
        {0xFFFDE7E9, 0xFF442726}, // 错误提示底色
        {0x37000000, 0x28FFFFFF}, // 强调控件禁用填充
        {0x06000000, 0x19000000}, // 选择指示器默认填充
        {0x0F000000, 0x0BFFFFFF}, // 选择指示器悬停填充
        {0x18000000, 0x12FFFFFF}, // 选择指示器按压填充
        {0xFFFFFFFF, 0xFF454545}, // 滑块实心填充
    };
    static_assert(sizeof(colors) / sizeof(colors[0]) == ElaThemeType::ThemeColorCount, "主题色表必须覆盖全部颜色角色");
    for (int index = 0; index < ElaThemeType::ThemeColorCount; ++index)
    {
        _lightThemeColorList[index] = QColor::fromRgba(colors[index][0]);
        _darkThemeColorList[index] = QColor::fromRgba(colors[index][1]);
    }
}

ElaThemePrivate::~ElaThemePrivate()
{
}

QPixmap ElaThemePrivate::_getShadowPixmap(const QPainterPath& path, const QSize& size, qreal devicePixelRatio, int shadowBorderWidth, qreal alphaRatio, const QColor& shadowColor)
{
    const QColor color = shadowColor.isValid() ? shadowColor : QColor(Qt::black);
    QByteArray key;
    QDataStream stream(&key, QIODevice::WriteOnly);
    stream << path << size << devicePixelRatio << shadowBorderWidth << alphaRatio << color.rgba() << static_cast<int>(_themeMode);
    if (const auto* cached = _shadowCache.object(key))
    {
        return *cached;
    }
    const qreal scale = shadowBorderWidth / 32.0;
    const QSize pixelSize(qCeil(size.width() * devicePixelRatio), qCeil(size.height() * devicePixelRatio));
    QImage mask(pixelSize, QImage::Format_ARGB32_Premultiplied);
    mask.setDevicePixelRatio(devicePixelRatio);
    mask.fill(Qt::transparent);
    {
        QPainter painter(&mask);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        painter.drawPath(path);
    }
    QPixmap shadow(pixelSize);
    shadow.setDevicePixelRatio(devicePixelRatio);
    shadow.fill(Qt::transparent);
    QPainter painter(&shadow);
    for (int layer = 0; layer < 2; ++layer)
    {
        const quint16 radius = static_cast<quint16>(qMax(1, qRound((layer == 0 ? 3 : 9) * scale * devicePixelRatio)));
        QPixmap blurred = ElaExponentialBlur::doExponentialBlur(mask, radius);
        blurred = ElaExponentialBlur::doExponentialBlur(blurred.toImage(), radius);
        blurred.setDevicePixelRatio(devicePixelRatio);
        const qreal opacity = _themeMode == ElaThemeType::Light ? (layer == 0 ? 0.12 : 0.20) : (layer == 0 ? 0.20 : 0.38);
        painter.setOpacity(qMin(qreal(1), opacity * alphaRatio));
        painter.drawPixmap(QPointF(0, (layer == 0 ? 2 : 8) * scale), blurred);
    }
    painter.setOpacity(1);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(QRect(QPoint(), size), color);
    // 清除内容内部的阴影 半透明面板只叠加自身主题色
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawPath(path);
    painter.end();
    const int cost = qMax(1, qCeil(static_cast<qreal>(shadow.width()) * shadow.height() * 4 / 1024));
    _shadowCache.insert(key, new QPixmap(shadow), cost);
    return shadow;
}
