#include "ElaComboBoxListView.h"

#include <QGraphicsDropShadowEffect>

#include "ElaApplication.h"
#include "ElaScrollBar.h"
ElaComboBoxListView::ElaComboBoxListView(QWidget* parent)
    : QListView(parent)
{
    setMaximumHeight(200);
    setStyleSheet(
        "QListView{border:2px solid #DFDFDF;border-radius:8px;background-color: #F9F9F9;}"
        "QListView::item{height: 35px;border:none;}");
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
    setSelectionMode(QAbstractItemView::NoSelection);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, &ElaComboBoxListView::onThemeChanged);
}

ElaComboBoxListView::~ElaComboBoxListView()
{
}

void ElaComboBoxListView::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    if (themeMode == ElaApplicationType::Light)
    {
        setStyleSheet(
            "QListView{border:2px solid #DFDFDF;border-radius:8px;background-color: #F9F9F9;}"
            "QListView::item{height: 35px;border:none;}");
    }
    else
    {
        setStyleSheet(
            "QListView{border:2px solid #494949;border-radius:8px;background-color: #2D2D2D;}"
            "QListView::item{height: 35px;border:none;}");
    }
}
