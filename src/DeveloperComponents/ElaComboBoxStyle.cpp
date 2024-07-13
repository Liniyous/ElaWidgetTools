#include "ElaComboBoxStyle.h"

ElaComboBoxStyle::ElaComboBoxStyle(QStyle* style)
{
}

ElaComboBoxStyle::~ElaComboBoxStyle()
{
}

QSize ElaComboBoxStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    if (type == QStyle::CT_ComboBox)
    {
        QSize comboBoxSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        comboBoxSize.setWidth(comboBoxSize.width() + 20);
        return comboBoxSize;
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}
