#include "ElaComboBoxViewPrivate.h"

#include "ElaComboBoxView.h"

ElaComboBoxViewPrivate::ElaComboBoxViewPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaComboBoxViewPrivate::~ElaComboBoxViewPrivate()
{
}

void ElaComboBoxViewPrivate::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    Q_Q(ElaComboBoxView);
    if (themeMode == ElaApplicationType::Light)
    {
        q->setStyleSheet(
            "QListView{border:2px solid #DFDFDF;border-radius:8px;background-color: #F9F9F9;}"
            "QListView::item{height: 35px;border:none;}");
    }
    else
    {
        q->setStyleSheet(
            "QListView{border:2px solid #494949;border-radius:8px;background-color: #2D2D2D;}"
            "QListView::item{height: 35px;border:none;}");
    }
}
