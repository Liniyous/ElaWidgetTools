#include "ElaApplication.h"

#include "ElaTheme.h"
#include "ElaWinShadowHelper.h"
#include "private/ElaApplicationPrivate.h"
#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QWidget>
#include <utility>
Q_SINGLETON_CREATE_CPP(ElaApplication)
ElaApplication::ElaApplication(QObject* parent)
    : QObject{parent}, d_ptr(new ElaApplicationPrivate())
{
    Q_D(ElaApplication);
    d->q_ptr = this;
    d->_pElaMicaImagePath = ":/include/Image/MicaBase.png";
    d->_pWindowDisplayMode = ElaApplicationType::Normal;
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaApplicationPrivate::onThemeModeChanged);
}

ElaApplication::~ElaApplication()
{
}

void ElaApplication::setWindowDisplayMode(ElaApplicationType::WindowDisplayMode windowDisplayType)
{
    Q_D(ElaApplication);
    auto lastDisplayMode = d->_pWindowDisplayMode;
    if (lastDisplayMode == windowDisplayType)
    {
        return;
    }
    if (lastDisplayMode == ElaApplicationType::ElaMica)
    {
        d->_resetAllMicaWidget();
    }
    switch (windowDisplayType)
    {
    case ElaApplicationType::Normal:
    {
        break;
    }
    case ElaApplicationType::ElaMica:
    {
        d->_pWindowDisplayMode = windowDisplayType;
        d->_initMicaBaseImage(QImage(d->_pElaMicaImagePath));
        break;
    }
    default:
    {
        break;
    }
    }
#ifdef Q_OS_WIN
    for (auto widget: d->_micaWidgetList)
    {
        ElaWinShadowHelper::getInstance()->setWindowDisplayMode(widget, windowDisplayType, lastDisplayMode);
        ElaWinShadowHelper::getInstance()->setWindowThemeMode(widget->winId(), d->_themeMode == ElaThemeType::Light);
    }
#endif
    if (windowDisplayType != ElaApplicationType::ElaMica)
    {
        d->_pWindowDisplayMode = windowDisplayType;
        Q_EMIT pWindowDisplayModeChanged();
    }
}

ElaApplicationType::WindowDisplayMode ElaApplication::getWindowDisplayMode() const
{
    Q_D(const ElaApplication);
    return d->_pWindowDisplayMode;
}

void ElaApplication::setElaMicaImagePath(QString micaImagePath)
{
    Q_D(ElaApplication);
    d->_pElaMicaImagePath = std::move(micaImagePath);
    d->_initMicaBaseImage(QImage(d->_pElaMicaImagePath));
    Q_EMIT pElaMicaImagePathChanged();
}

QString ElaApplication::getElaMicaImagePath() const
{
    Q_D(const ElaApplication);
    return d->_pElaMicaImagePath;
}

void ElaApplication::init()
{
    Q_INIT_RESOURCE(ElaWidgetTools);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QFontDatabase::addApplicationFont(":/include/Font/ElaAwesome.ttf");
    //默认字体
    QFont font = qApp->font();
    font.setPixelSize(13);
    font.setFamily("Microsoft YaHei");
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);
#ifdef Q_OS_WIN
    eWinHelper->initWinAPI();
#endif
}

void ElaApplication::syncWindowDisplayMode(QWidget* widget, bool isSync)
{
    Q_D(ElaApplication);
    if (!widget)
    {
        return;
    }
    if (isSync)
    {
        d->_micaWidgetList.append(widget);
        widget->installEventFilter(d);
    }
    else
    {
        d->_micaWidgetList.removeOne(widget);
        widget->removeEventFilter(d);
    }
    switch (d->_pWindowDisplayMode)
    {
    case ElaApplicationType::Normal:
    case ElaApplicationType::ElaMica:
    {
        if (isSync)
        {
            if (d->_pWindowDisplayMode == ElaApplicationType::WindowDisplayMode::ElaMica)
            {
                d->_updateMica(widget, false);
            }
        }
        break;
    }
    default:
    {
#ifdef Q_OS_WIN
        if (isSync)
        {
            ElaWinShadowHelper::getInstance()->setWindowDisplayMode(widget, d->_pWindowDisplayMode, ElaApplicationType::Normal);
            ElaWinShadowHelper::getInstance()->setWindowThemeMode(widget->winId(), d->_themeMode == ElaThemeType::Light);
        }
        else
        {
            ElaWinShadowHelper::getInstance()->setWindowDisplayMode(widget, ElaApplicationType::Normal, d->_pWindowDisplayMode);
            ElaWinShadowHelper::getInstance()->setWindowThemeMode(widget->winId(), true);
        }
#endif
        break;
    }
    }
}

bool ElaApplication::containsCursorToItem(QWidget* item)
{
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = item->window()->mapFromGlobal(QCursor::pos());
    QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}
