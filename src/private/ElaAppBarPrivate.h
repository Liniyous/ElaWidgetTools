#ifndef ELAAPPBARPRIVATE_H
#define ELAAPPBARPRIVATE_H
#include <QObject>

#include "Def.h"
#include "stdafx.h"
class QLabel;
class ElaEvent;
class ElaAppBar;
class ElaIconButton;
class ElaAppBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaAppBar)
    Q_PROPERTY_CREATE_D(bool, IsStayTop)
    Q_PROPERTY_CREATE_D(bool, IsFixedSize)
    Q_PROPERTY_CREATE_D(bool, IsDefaultClosed)
public:
    explicit ElaAppBarPrivate(QObject* parent = nullptr);
    ~ElaAppBarPrivate();
    Q_SLOT void onMinButtonClicked();
    Q_SLOT void onMaxButtonClicked();
    Q_SLOT void onCloseButtonClicked();
    Q_SLOT void onStayTopButtonClicked();

private:
    ElaAppBarType::ButtonFlags _buttonFlags;
    ElaIconButton* _routeBackButton{nullptr};
    ElaIconButton* _navigationButton{nullptr};
    ElaIconButton* _themeChangeButton{nullptr};
    ElaIconButton* _stayTopButton{nullptr};
    ElaIconButton* _minButton{nullptr};
    ElaIconButton* _maxButton{nullptr};
    ElaIconButton* _closeButton{nullptr};
    QLabel* _titleLabel{nullptr};
    qint64 _currentWinID{0};
    int _margins{8};
    bool _isHoverMaxButton{false};
    void _changeMaxButtonAwesome(bool isMaximized);
    void _showSystemMenu(QPoint point);
    bool _containsCursorToItem(QWidget* item);
    void _onThemeModeChange(ElaApplicationType::ThemeMode themeMode);
};

#endif // ELAAPPBARPRIVATE_H
