#ifndef ELAAPPBARPRIVATE_H
#define ELAAPPBARPRIVATE_H
#include <QObject>

#include "Def.h"
#include "stdafx.h"
class QLabel;
class QScreen;
class QHBoxLayout;
class QVBoxLayout;
class ElaText;
class ElaAppBar;
class ElaIconButton;
class ElaToolButton;
class ElaAppBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaAppBar)
    Q_PROPERTY_CREATE_D(bool, IsStayTop)
    Q_PROPERTY_CREATE_D(bool, IsFixedSize)
    Q_PROPERTY_CREATE_D(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_D(bool, IsOnlyAllowMinAndClose)
    Q_PROPERTY_CREATE_D(int, AppBarHeight)
    Q_PROPERTY_CREATE_D(QWidget*, CustomWidget)
    Q_PROPERTY_CREATE_D(int, CustomWidgetMaximumWidth)
public:
    explicit ElaAppBarPrivate(QObject* parent = nullptr);
    ~ElaAppBarPrivate() override;
    Q_SLOT void onMinButtonClicked();
    Q_SLOT void onMaxButtonClicked();
    Q_SLOT void onCloseButtonClicked();
    Q_SLOT void onStayTopButtonClicked();

private:
    QHBoxLayout* _mainLayout{nullptr};
    QVBoxLayout* _iconLabelLayout{nullptr};
    QVBoxLayout* _titleLabelLayout{nullptr};
    ElaAppBarType::ButtonFlags _buttonFlags;
    ElaToolButton* _routeBackButton{nullptr};
    ElaToolButton* _navigationButton{nullptr};
    ElaToolButton* _themeChangeButton{nullptr};
    ElaToolButton* _stayTopButton{nullptr};
    ElaToolButton* _minButton{nullptr};
    ElaToolButton* _maxButton{nullptr};
    ElaIconButton* _closeButton{nullptr};
    QScreen* _lastScreen{nullptr};
    ElaText* _titleLabel{nullptr};
    QLabel* _iconLabel{nullptr};
    qint64 _currentWinID{0};
    int _lastMinTrackWidth{0};
    quint64 _clickTimer{0};
    int _edges{0};
    int _margins{8};
    bool _isHoverMaxButton{false};
    void _changeMaxButtonAwesome(bool isMaximized);
    void _showSystemMenu(QPoint point);
    void _updateCursor(int edges);
    bool _containsCursorToItem(QWidget* item);
    void _onThemeModeChange(ElaThemeType::ThemeMode themeMode);
    int _calculateMinimumWidth();
    QVBoxLayout* _createVLayout(QWidget* widget);
};

#endif // ELAAPPBARPRIVATE_H
