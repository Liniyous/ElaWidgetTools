#include "ElaApplicationPrivate.h"

#include <QApplication>
#include <QEvent>
#include <QImage>
#include <QPalette>
#include <QScreen>
#include <QThread>
#include <QWidget>
#include <QtMath>

#include "ElaApplication.h"
#include "ElaMicaBaseInitObject.h"
#include "ElaWinShadowHelper.h"
ElaApplicationPrivate::ElaApplicationPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaApplicationPrivate::~ElaApplicationPrivate()
{
}

void ElaApplicationPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
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
        for (auto widget: _micaWidgetList)
        {
            ElaWinShadowHelper::getInstance()->setWindowThemeMode(widget->winId(), _themeMode == ElaThemeType::Light);
        }
#endif
        break;
    }
    }
}

bool ElaApplicationPrivate::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Show:
    {
        if (_pWindowDisplayMode == ElaApplicationType::WindowDisplayMode::ElaMica)
        {
            QWidget* widget = qobject_cast<QWidget*>(watched);
            if (widget)
            {
                _updateMica(widget);
            }
        }
        else if (_pWindowDisplayMode != ElaApplicationType::WindowDisplayMode::Normal)
        {
#ifdef Q_OS_WIN
            QWidget* widget = qobject_cast<QWidget*>(watched);
            if (widget)
            {
                ElaWinShadowHelper::getInstance()->setWindowDisplayMode(widget, _pWindowDisplayMode, _pWindowDisplayMode);
            }
#endif
        }
        break;
    }
    case QEvent::Move:
    case QEvent::Resize:
    {
        if (_pWindowDisplayMode == ElaApplicationType::WindowDisplayMode::ElaMica)
        {
            QWidget* widget = qobject_cast<QWidget*>(watched);
            if (widget)
            {
                _updateMica(widget);
            }
        }
        break;
    }
    case QEvent::Destroy:
    {
        QWidget* widget = qobject_cast<QWidget*>(watched);
        if (widget)
        {
            _micaWidgetList.removeOne(widget);
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return QObject::eventFilter(watched, event);
}

void ElaApplicationPrivate::_initMicaBaseImage(QImage img)
{
    Q_Q(ElaApplication);
    if (img.isNull())
    {
        return;
    }
    QThread* initThread = new QThread();
    ElaMicaBaseInitObject* initObject = new ElaMicaBaseInitObject(this);
    connect(initThread, &QThread::finished, initObject, &ElaMicaBaseInitObject::deleteLater);
    connect(initObject, &ElaMicaBaseInitObject::initFinished, initThread, [=]() {
        Q_EMIT q->pWindowDisplayModeChanged();
        _updateAllMicaWidget();
        initThread->quit();
        initThread->wait();
        initThread->deleteLater();
    });
    initObject->moveToThread(initThread);
    initThread->start();
    connect(this, &ElaApplicationPrivate::initMicaBase, initObject, &ElaMicaBaseInitObject::onInitMicaBase);
    Q_EMIT initMicaBase(img);
}

QRect ElaApplicationPrivate::_calculateWindowVirtualGeometry(QWidget* widget)
{
    QRect geometry = widget->geometry();
    qreal xImageRatio = 1, yImageRatio = 1;
    QRect relativeGeometry;
    if (qApp->screens().count() > 1)
    {
        QScreen* currentScreen = qApp->screenAt(geometry.topLeft());
        if (currentScreen)
        {
            QRect screenGeometry = currentScreen->availableGeometry();
            xImageRatio = (qreal)_lightBaseImage.width() / screenGeometry.width();
            yImageRatio = (qreal)_lightBaseImage.height() / screenGeometry.height();
            relativeGeometry = QRect((geometry.x() - screenGeometry.x()) * xImageRatio, (geometry.y() - screenGeometry.y()) * yImageRatio, geometry.width() * xImageRatio, geometry.height() * yImageRatio);
            return relativeGeometry;
        }
    }
    QRect primaryScreenGeometry = qApp->primaryScreen()->availableGeometry();
    xImageRatio = (qreal)_lightBaseImage.width() / primaryScreenGeometry.width();
    yImageRatio = (qreal)_lightBaseImage.height() / primaryScreenGeometry.height();
    relativeGeometry = QRect((geometry.x() - primaryScreenGeometry.x()) * xImageRatio, (geometry.y() - primaryScreenGeometry.y()) * yImageRatio, geometry.width() * xImageRatio, geometry.height() * yImageRatio);
    return relativeGeometry;
}

void ElaApplicationPrivate::_updateMica(QWidget* widget, bool isProcessEvent)
{
    if (widget->isVisible())
    {
        QPalette palette = widget->palette();
        if (_themeMode == ElaThemeType::Light)
        {
            palette.setBrush(QPalette::Window, _lightBaseImage.copy(_calculateWindowVirtualGeometry(widget)).scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        else
        {
            palette.setBrush(QPalette::Window, _darkBaseImage.copy(_calculateWindowVirtualGeometry(widget)).scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        widget->setPalette(palette);
        if (isProcessEvent)
        {
            QApplication::processEvents();
        }
    }
}

void ElaApplicationPrivate::_updateAllMicaWidget()
{
    if (_pWindowDisplayMode == ElaApplicationType::WindowDisplayMode::ElaMica)
    {
        for (auto widget: _micaWidgetList)
        {
            _updateMica(widget, false);
        }
    }
}

void ElaApplicationPrivate::_resetAllMicaWidget()
{
    for (auto widget: _micaWidgetList)
    {
        QPalette palette = widget->palette();
        palette.setBrush(QPalette::Window, Qt::transparent);
        widget->setPalette(palette);
    }
}
