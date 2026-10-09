#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAAPPBARPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAAPPBARPRIVATE_H_
#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

class QLabel;
class QScreen;
class QHBoxLayout;
class QVBoxLayout;
class ElaText;
class ElaAppBar;
class ElaIconButton;
class ElaToolButton;
class QMenu;
class ElaAppBarPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaAppBar)
    ELA_PROPERTY_D(bool, IsStayTop)
    ELA_PROPERTY_D(bool, IsFixedSize)
    ELA_PROPERTY_D(bool, IsDefaultClosed)
    ELA_PROPERTY_D(bool, IsOnlyAllowMinAndClose)
    ELA_PROPERTY_D(int, AppBarHeight)
    ELA_PROPERTY_D(int, RibbonHeight)
    ELA_VARIABLE_D(QMenu*, CustomMenu)
public:
    explicit ElaAppBarPrivate(QObject* parent = nullptr);
    ~ElaAppBarPrivate() override;
    Q_SLOT void onMinButtonClicked();
    Q_SLOT void onMaxButtonClicked();
    Q_SLOT void onCloseButtonClicked();
    Q_SLOT void onStayTopButtonClicked();

private:
    ElaThemeType::ThemeMode _themeMode;
    QHBoxLayout* _mainLayout{nullptr};
    QVBoxLayout* _iconLabelLayout{nullptr};
    QVBoxLayout* _titleLabelLayout{nullptr};
    ElaAppBarType::ButtonFlags _buttonFlags;
    ElaToolButton* _routeBackButton{nullptr};
    ElaToolButton* _routeForwardButton{nullptr};
    ElaToolButton* _navigationButton{nullptr};
    ElaToolButton* _themeChangeButton{nullptr};
    ElaToolButton* _stayTopButton{nullptr};
    ElaToolButton* _minButton{nullptr};
    ElaToolButton* _maxButton{nullptr};
    ElaIconButton* _closeButton{nullptr};
    QList<QWidget*> _customAreaWidgetList{nullptr, nullptr, nullptr};
    QList<QObject*> _customAreaHitTestObjectList{nullptr, nullptr, nullptr};
    QStringList _customAreaHitTestFunctionNameList{"", "", ""};
    QList<QWidget*> _clientWidgetList;
    QScreen* _lastScreen{nullptr};
    ElaText* _titleLabel{nullptr};
    QLabel* _iconLabel{nullptr};
    qint64 _currentWinID{0};
    int _lastMinTrackWidth{0};
    quint64 _clickTimer{0};
    int _edges{0};
    int _margins{8};
    bool _isHoverMaxButton{false};
    int _win7Margins{0};
    void _changeMaxButtonAwesome(bool isMaximized);
    void _showAppBarMenu(QPoint point);
    void _updateCursor(int edges);
    bool _containsCursorToItem(QWidget* item);
    void _onThemeModeChange(ElaThemeType::ThemeMode themeMode);
    int _calculateMinimumWidth();
    QVBoxLayout* _createVLayout(QWidget* widget);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAAPPBARPRIVATE_H_

