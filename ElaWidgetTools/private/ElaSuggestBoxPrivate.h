#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASUGGESTBOXPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASUGGESTBOXPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QAction>
#include <QObject>
#include <QPointer>
#include <QVariantMap>
#include <QVector>

class ElaSuggestion : public QObject
{
    Q_OBJECT
    ELA_PROPERTY(ElaIconType::IconName, ElaIcon)
    ELA_PROPERTY(QString, SuggestText)
    ELA_PROPERTY(QString, SuggestKey)
    ELA_PROPERTY(QVariantMap, SuggestData)
public:
    explicit ElaSuggestion(QObject* parent = nullptr);
    ~ElaSuggestion() override;
};

class QVariantAnimation;
class ElaLineEdit;
class ElaSuggestModel;
class ElaBaseListView;
class ElaSuggestBox;
class ElaSuggestBoxSearchViewContainer;
class ElaSuggestBoxPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaSuggestBox)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(Qt::CaseSensitivity, CaseSensitivity)
public:
    explicit ElaSuggestBoxPrivate(QObject* parent = nullptr);
    ~ElaSuggestBoxPrivate() override;
    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);
    Q_SLOT void onSearchEditTextEdit(const QString& searchText);
    Q_SLOT void onSearchViewClicked(const QModelIndex& index);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void _animatePopupTo(qreal progress);
    void _applyPopupReveal(qreal progress);

    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
    QAction* _lightSearchAction{nullptr};
    QAction* _darkSearchAction{nullptr};
    QVector<ElaSuggestion*> _suggestionVector;
    QPointer<ElaSuggestBoxSearchViewContainer> _searchViewBaseWidget;
    ElaLineEdit* _searchEdit{nullptr};
    ElaSuggestModel* _searchModel{nullptr};
    ElaBaseListView* _searchView{nullptr};
    QVariantAnimation* _popupAnimation{nullptr}; // 建议面板展开与收起共用的动画
    qreal _popupProgress{0.0};                   // 面板已展开比例 范围为 0 到 1
    bool _isPopupAbove{false};                   // 面板是否锚定在输入框上边缘
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASUGGESTBOXPRIVATE_H_

