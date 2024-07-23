#include "ElaApplication.h"

#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QWidget>

#include "private/ElaApplicationPrivate.h"
Q_SINGLETON_CREATE_CPP(ElaApplication)
Q_PROPERTY_CREATE_Q_CPP(ElaApplication, bool, IsApplicationClosed)
Q_PRIVATE_CREATE_Q_CPP(ElaApplication, QIcon, WindowIcon)
ElaApplication::ElaApplication(QObject* parent)
    : QObject{parent}, d_ptr(new ElaApplicationPrivate())
{
    Q_D(ElaApplication);
    d->q_ptr = this;
    d->_pIsApplicationClosed = false;
    d->_pWindowIcon = QIcon(":/include/Image/Cirno.jpg");
}

ElaApplication::~ElaApplication()
{
}

void ElaApplication::init()
{
    Q_D(ElaApplication);
    QFontDatabase::addApplicationFont(":/include/Font/ElaAwesome.ttf");
    QApplication::setWindowIcon(d->_pWindowIcon);
    //默认字体
    QFont font = qApp->font();
    font.setPointSize(10);
    font.setFamily("Microsoft YaHei");
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);
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
