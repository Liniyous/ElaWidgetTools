#include "ElaApplication.h"

#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QWidget>
ElaApplication::ElaApplication(QObject* parent)
    : QObject{parent}
{
    _pIsApplicationClosed = false;
    _pWindowIcon = QIcon(":/src/Image/Cirno.jpg");
    _pShadowEffectColor = QColor(0xDA, 0xDA, 0xDA);
    //_pGraphicsDropShadowEffectColor = QColor(0xAE, 0xAE, 0xB0);
}

ElaApplication::~ElaApplication()
{
}

void ElaApplication::setThemeMode(ElaApplicationType::ThemeMode themeMode)
{
    this->_themeMode = themeMode;
    Q_EMIT themeModeChanged(this->_themeMode);
}

ElaApplicationType::ThemeMode ElaApplication::getThemeMode() const
{
    return this->_themeMode;
}

void ElaApplication::init()
{
    QFontDatabase::addApplicationFont(":/src/Font/ElaAwesome.ttf");
    QApplication::setWindowIcon(_pWindowIcon);
    QFont font = qApp->font();
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);
}

bool ElaApplication::containsCursorToItem(QWidget* item)
{
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = QCursor::pos();
    QRectF rect = QRectF(item->mapToGlobal(QPoint(0, 0)), item->size());
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}
