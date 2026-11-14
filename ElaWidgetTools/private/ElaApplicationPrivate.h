#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAAPPLICATIONPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAAPPLICATIONPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QAbstractNativeEventFilter>
#include <QElapsedTimer>
#include <QHash>
#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QPointer>
#include <QRectF>

class ElaApplication;
class QPaintEvent;
class QScreen;
class QThread;
class QWidget;
class ElaApplicationPrivate : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
    ELA_PIMPL_D(ElaApplication)
    ELA_PROPERTY_D(ElaApplicationType::WindowDisplayMode, WindowDisplayMode)
    ELA_PROPERTY_D(QString, ElaMicaImagePath)
    ELA_PROPERTY_D(int, FontPixelSize)

public:
    explicit ElaApplicationPrivate(QObject* parent = nullptr);
    ~ElaApplicationPrivate() override;
    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);
Q_SIGNALS:
    Q_SIGNAL void initMicaBase(QImage image, quint64 revision);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;
#else
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;
#endif

private:
    struct MicaScreenCache {
        QRectF nativeGeometry; // 屏幕在 Windows 物理桌面中的范围
        QPixmap pixmap;        // 当前主题按屏幕物理尺寸生成的共享背景
    };

    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light}; // 当前背景主题
    QList<QPointer<QWidget>> _micaWidgetList;                // 同步显示模式的窗口 不持有窗口所有权
    QImage _lightBaseImage;                                  // 浅色背景源图 仅在主线程发布
    QImage _darkBaseImage;                                   // 深色背景源图 仅在主线程发布
    QHash<QScreen*, MicaScreenCache> _micaScreenCaches;      // 每个屏幕复用一份当前主题背景
    QThread* _micaInitThread{nullptr};                       // 按需启动的背景生成线程
    QImage _pendingMicaImage;                                // 生成期间最后一次请求的源图
    quint64 _micaImageRevision{0};                           // 最新背景请求序号 用于丢弃过期结果
    bool _isMicaInitializing{false};                         // 是否已有背景生成任务在执行
    QPointer<QWidget> _micaMovingWidget;                     // 正在原生移动或缩放的窗口
    QElapsedTimer _micaMoveClock;                           // 原生移动期间距离上一帧的时间
    bool _isProcessingMicaFrame{false};                      // 防止动画回调嵌套推进移动帧

    void _initMicaBaseImage(QImage image);
    void _startMicaBaseInit();
    void _watchMicaScreen(QScreen* screen);
    const MicaScreenCache& _getMicaScreenCache(QScreen* screen);
    void _paintMica(QWidget* widget, QPaintEvent* event);
    void _updateMica(QWidget* widget);
    void _processMicaMoveFrame(QWidget* widget);
    void _updateAllMicaWidget();
    void _resetAllMicaWidget();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAAPPLICATIONPRIVATE_H_
