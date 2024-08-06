#include "ElaApplication.h"

#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QWidget>

#include "private/ElaApplicationPrivate.h"
Q_SINGLETON_CREATE_CPP(ElaApplication)
Q_PROPERTY_CREATE_Q_CPP(ElaApplication, bool, IsApplicationClosed)
ElaApplication::ElaApplication(QObject* parent)
    : QObject{parent}, d_ptr(new ElaApplicationPrivate())
{
    Q_D(ElaApplication);
    d->q_ptr = this;
    d->_pIsApplicationClosed = false;
}

ElaApplication::~ElaApplication()
{
}

void ElaApplication::init()
{
    QFontDatabase::addApplicationFont(":/include/Font/ElaAwesome.ttf");
    //默认字体
    QFont font = qApp->font();
    font.setPixelSize(13);
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
