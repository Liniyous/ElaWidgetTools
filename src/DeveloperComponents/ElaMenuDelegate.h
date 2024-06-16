#ifndef ELAMENUDELEGATE_H
#define ELAMENUDELEGATE_H

#include <QStyledItemDelegate>

#include "Def.h"
#include "stdafx.h"
class ElaMenuDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ElaMenuDelegate(QObject* parent = nullptr);
    ~ElaMenuDelegate();
    void setMenuItemHeight(int height);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaApplicationType::ThemeMode _themeMode{ElaApplicationType::Light};
    int _menuItemHeight{35};
    int _iconLeftPadding{10};
    int _textLeftSpacing{15};
};

#endif // ELAMENUDELEGATE_H
