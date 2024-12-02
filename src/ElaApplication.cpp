#include "ElaApplication.h"

#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QWidget>

#include "ElaTheme.h"
#include "private/ElaApplicationPrivate.h"
Q_SINGLETON_CREATE_CPP(ElaApplication)
ElaApplication::ElaApplication(QObject* parent)
    : QObject{parent}, d_ptr(new ElaApplicationPrivate())
{
    Q_D(ElaApplication);
    d->q_ptr = this;
    d->_pIsEnableMica = false;
    d->_pMicaImagePath = ":/include/Image/MicaBase.png";
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaApplicationPrivate::onThemeModeChanged);
}

ElaApplication::~ElaApplication()
{
}

void ElaApplication::setIsEnableMica(bool isEnable)
{
    Q_D(ElaApplication);
    d->_pIsEnableMica = isEnable;
    if (isEnable)
    {
        d->_initMicaBaseImage(QImage(d->_pMicaImagePath));
    }
    else
    {
        d->onThemeModeChanged(d->_themeMode);
        Q_EMIT pIsEnableMicaChanged();
    }
}

bool ElaApplication::getIsEnableMica() const
{
    Q_D(const ElaApplication);
    return d->_pIsEnableMica;
}

void ElaApplication::setMicaImagePath(QString micaImagePath)
{
    Q_D(ElaApplication);
    d->_pMicaImagePath = micaImagePath;
    d->_initMicaBaseImage(QImage(d->_pMicaImagePath));
    Q_EMIT pMicaImagePathChanged();
}

QString ElaApplication::getMicaImagePath() const
{
    Q_D(const ElaApplication);
    return d->_pMicaImagePath;
}

void ElaApplication::init()
{
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QFontDatabase::addApplicationFont(":/include/Font/ElaAwesome.ttf");
    //默认字体
    QFont font = qApp->font();
    font.setFamilies({"Segoe UI", "Microsoft YaHei", "PingFang SC"});
    font.setPixelSize(14);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setStyleStrategy(QFont::PreferAntialias);
    qApp->setFont(font);
}

void ElaApplication::syncMica(QWidget* widget, bool isSync)
{
    Q_D(ElaApplication);
    if (!widget)
    {
        return;
    }
    if (isSync)
    {
        widget->installEventFilter(d);
        d->_micaWidgetList.append(widget);
        if (d->_pIsEnableMica)
        {
            d->_updateMica(widget, false);
        }
    }
    else
    {
        widget->removeEventFilter(d);
        d->_micaWidgetList.removeOne(widget);
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
