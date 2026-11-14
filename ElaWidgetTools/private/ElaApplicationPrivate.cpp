#include "ElaApplicationPrivate.h"

#include "ElaApplication.h"
#include "ElaMicaBaseInitObject.h"
#include "ElaMicaPainter.h"
#include "ElaWinShadowHelper.h"

#include <QApplication>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QScopedValueRollback>
#include <QScreen>
#include <QThread>
#include <QWidget>
#include <QtMath>

ElaApplicationPrivate::ElaApplicationPrivate(QObject* parent)
    : QObject{parent}
{
#ifdef Q_OS_WIN
    qApp->installNativeEventFilter(this);
#endif
    _micaInitThread = new QThread(this);
    auto* initObject = new ElaMicaBaseInitObject();
    initObject->moveToThread(_micaInitThread);
    connect(_micaInitThread, &QThread::finished, initObject, &QObject::deleteLater);
    connect(this, &ElaApplicationPrivate::initMicaBase, initObject, &ElaMicaBaseInitObject::onInitMicaBase);
    connect(initObject, &ElaMicaBaseInitObject::initFinished, this, [this](const QImage& lightImage, const QImage& darkImage, quint64 revision) {
        _isMicaInitializing = false;
        if (revision != _micaImageRevision)
        {
            _startMicaBaseInit();
            return;
        }
        if (!lightImage.isNull() && !darkImage.isNull())
        {
            _lightBaseImage = lightImage;
            _darkBaseImage = darkImage;
            _micaScreenCaches.clear();
        }
        Q_Q(ElaApplication);
        Q_EMIT q->pWindowDisplayModeChanged();
        _updateAllMicaWidget();
    });
    connect(qApp, &QCoreApplication::aboutToQuit, this, [this]() {
        _micaInitThread->quit();
        _micaInitThread->wait();
    });
    connect(qApp, &QGuiApplication::screenAdded, this, [this](QScreen* screen) {
        _watchMicaScreen(screen);
        _updateAllMicaWidget();
    });
    connect(qApp, &QGuiApplication::screenRemoved, this, [this](QScreen* screen) {
        _micaScreenCaches.remove(screen);
        _updateAllMicaWidget();
    });
    for (QScreen* screen: QGuiApplication::screens())
    {
        _watchMicaScreen(screen);
    }
}

ElaApplicationPrivate::~ElaApplicationPrivate()
{
#ifdef Q_OS_WIN
    if (qApp)
    {
        qApp->removeNativeEventFilter(this);
    }
#endif
    _micaInitThread->quit();
    _micaInitThread->wait();
}

void ElaApplicationPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    _micaScreenCaches.clear();
    switch (_pWindowDisplayMode)
    {
    case ElaApplicationType::Normal:
    {
        break;
    }
    case ElaApplicationType::ElaMica:
    {
        _updateAllMicaWidget();
        break;
    }
    default:
    {
#ifdef Q_OS_WIN
        for (const auto& widget: _micaWidgetList)
        {
            if (widget)
            {
                ElaWinShadowHelper::getInstance()->setWindowThemeMode(widget->winId(), _themeMode == ElaThemeType::Light);
            }
        }
#endif
        break;
    }
    }
}

bool ElaApplicationPrivate::eventFilter(QObject* watched, QEvent* event)
{
    QPointer<QWidget> widget = qobject_cast<QWidget*>(watched);
    if (!widget)
    {
        return QObject::eventFilter(watched, event);
    }
    switch (event->type())
    {
    case QEvent::Show:
    {
        if (_pWindowDisplayMode == ElaApplicationType::ElaMica)
        {
            _updateMica(widget);
        }
        else if (_pWindowDisplayMode != ElaApplicationType::Normal)
        {
#ifdef Q_OS_WIN
            ElaWinShadowHelper::getInstance()->setWindowDisplayMode(widget, _pWindowDisplayMode, _pWindowDisplayMode);
#endif
        }
        break;
    }
    case QEvent::Move:
    case QEvent::Resize:
    case QEvent::ScreenChangeInternal:
    {
        if (_pWindowDisplayMode == ElaApplicationType::ElaMica)
        {
            _updateMica(widget);
            _processMicaMoveFrame(widget);
        }
        break;
    }
    case QEvent::Paint:
    {
        if (_pWindowDisplayMode == ElaApplicationType::ElaMica)
        {
            _paintMica(widget, static_cast<QPaintEvent*>(event));
        }
        break;
    }
    case QEvent::Destroy:
    {
        _micaWidgetList.removeAll(widget);
        break;
    }
    default:
    {
        break;
    }
    }
    return !widget || QObject::eventFilter(watched, event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool ElaApplicationPrivate::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
#else
bool ElaApplicationPrivate::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
#endif
{
    Q_UNUSED(result);
#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG" && message)
    {
        const auto* msg = static_cast<const MSG*>(message);
        if (msg->message == WM_ENTERSIZEMOVE)
        {
            QWidget* widget = QWidget::find(reinterpret_cast<WId>(msg->hwnd));
            _micaMovingWidget = _micaWidgetList.contains(widget) ? widget : nullptr;
            _micaMoveClock.invalidate();
        }
        else if (msg->message == WM_EXITSIZEMOVE)
        {
            _micaMovingWidget.clear();
            _micaMoveClock.invalidate();
        }
    }
#else
    Q_UNUSED(eventType);
    Q_UNUSED(message);
#endif
    return false;
}

void ElaApplicationPrivate::_initMicaBaseImage(QImage image)
{
    if (image.isNull())
    {
        return;
    }
    ++_micaImageRevision;
    _pendingMicaImage = image;
    if (!_isMicaInitializing)
    {
        _startMicaBaseInit();
    }
}

void ElaApplicationPrivate::_startMicaBaseInit()
{
    if (_pendingMicaImage.isNull())
    {
        return;
    }
    _isMicaInitializing = true;
    if (!_micaInitThread->isRunning())
    {
        _micaInitThread->start();
    }
    Q_EMIT initMicaBase(_pendingMicaImage, _micaImageRevision);
    _pendingMicaImage = QImage();
}

void ElaApplicationPrivate::_watchMicaScreen(QScreen* screen)
{
    const auto invalidateCache = [this, screen]() {
        _micaScreenCaches.remove(screen);
        _updateAllMicaWidget();
    };
    connect(screen, &QScreen::geometryChanged, this, invalidateCache);
    connect(screen, &QScreen::logicalDotsPerInchChanged, this, invalidateCache);
    connect(screen, &QScreen::physicalDotsPerInchChanged, this, invalidateCache);
}

const ElaApplicationPrivate::MicaScreenCache& ElaApplicationPrivate::_getMicaScreenCache(QScreen* screen)
{
    auto it = _micaScreenCaches.find(screen);
    if (it != _micaScreenCaches.end())
    {
        return it.value();
    }
    MicaScreenCache cache;
    const QSize pixelSize = screen->size() * screen->devicePixelRatio();
    cache.nativeGeometry = QRectF(screen->geometry().topLeft(), pixelSize);
#ifdef Q_OS_WIN
    DEVMODEW displayMode{};
    displayMode.dmSize = sizeof(displayMode);
    const QString screenName = screen->name();
    if (EnumDisplaySettingsExW(reinterpret_cast<const wchar_t*>(screenName.utf16()), ENUM_CURRENT_SETTINGS, &displayMode, 0))
    {
        cache.nativeGeometry = QRectF(displayMode.dmPosition.x, displayMode.dmPosition.y, displayMode.dmPelsWidth, displayMode.dmPelsHeight);
    }
#endif
    const QImage& baseImage = _themeMode == ElaThemeType::Light ? _lightBaseImage : _darkBaseImage;
    if (!baseImage.isNull())
    {
        cache.pixmap = QPixmap::fromImage(baseImage.scaled(cache.nativeGeometry.size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    return _micaScreenCaches.insert(screen, cache).value();
}

void ElaApplicationPrivate::_paintMica(QWidget* widget, QPaintEvent* event)
{
    QPainter painter(widget);
    painter.setClipRegion(event->region());
    // 屏幕外和显示器空隙始终填充不透明底色 避免暴露旧帧或未初始化的像素
    painter.fillRect(widget->rect(), _themeMode == ElaThemeType::Light ? QColor(250, 250, 250) : QColor(32, 32, 32));
    qreal coordinateScale = 1.0;
    QPointF origin = widget->mapToGlobal(QPoint());
    bool isNativeGeometry = false;
#ifdef Q_OS_WIN
    POINT nativeOrigin{};
    if (ClientToScreen(reinterpret_cast<HWND>(widget->window()->winId()), &nativeOrigin))
    {
        // 混合缩放屏幕的 Qt 全局坐标存在空隙 跨屏采样统一在物理桌面坐标中计算
        coordinateScale = widget->devicePixelRatioF();
        const QPoint widgetOffset = widget->mapTo(widget->window(), QPoint());
        origin = QPointF(nativeOrigin.x, nativeOrigin.y) + QPointF(widgetOffset) * coordinateScale;
        isNativeGeometry = true;
    }
#endif
    const QRectF windowRect(origin, QSizeF(widget->size()) * coordinateScale);
    for (QScreen* screen: QGuiApplication::screens())
    {
        const MicaScreenCache& cache = _getMicaScreenCache(screen);
        const QRectF screenRect = isNativeGeometry ? cache.nativeGeometry : QRectF(screen->geometry());
        ElaMicaPainter::drawScreen(&painter, windowRect, coordinateScale, screenRect, cache.pixmap);
    }
}

void ElaApplicationPrivate::_updateMica(QWidget* widget)
{
    if (widget && widget->isVisible())
    {
        // 由 Qt 合并连续移动产生的重绘请求 绘制时再读取窗口的最新位置
        widget->update();
    }
}

void ElaApplicationPrivate::_processMicaMoveFrame(QWidget* widget)
{
    if (!widget || widget != _micaMovingWidget || _isProcessingMicaFrame || (_micaMoveClock.isValid() && _micaMoveClock.elapsed() < 16))
    {
        return;
    }
    QScopedValueRollback<bool> frameGuard(_isProcessingMicaFrame, true);
    QPointer<QWidget> window = widget->window();
    _micaMoveClock.start();
    // 原生拖动循环可能延后动画时钟与低优先级绘制事件 仅推进计时和当前窗口的待绘制区域
    QCoreApplication::sendPostedEvents(nullptr, QEvent::Timer);
    if (window && _pWindowDisplayMode == ElaApplicationType::ElaMica)
    {
        QCoreApplication::sendPostedEvents(window, QEvent::UpdateRequest);
    }
}

void ElaApplicationPrivate::_updateAllMicaWidget()
{
    if (_pWindowDisplayMode == ElaApplicationType::ElaMica)
    {
        for (const auto& widget: _micaWidgetList)
        {
            _updateMica(widget);
        }
    }
}

void ElaApplicationPrivate::_resetAllMicaWidget()
{
    _micaScreenCaches.clear();
    for (const auto& widget: _micaWidgetList)
    {
        _updateMica(widget);
    }
}
