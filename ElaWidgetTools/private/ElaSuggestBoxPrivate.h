#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASUGGESTBOXPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASUGGESTBOXPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QAction>
#include <QObject>
#include <QSize>
#include <QVariantMap>
#include <QVector>

#include "ElaWidgetToolsDef.h"
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

class QVBoxLayout;
class ElaLineEdit;
class ElaNavigationNode;
class ElaSuggestModel;
class ElaBaseListView;
class ElaSuggestDelegate;
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

private:
    ElaThemeType::ThemeMode _themeMode;
    QAction* _lightSearchAction{nullptr};
    QAction* _darkSearchAction{nullptr};
    QVector<ElaSuggestion*> _suggestionVector;
    ElaSuggestBoxSearchViewContainer* _searchViewBaseWidget{nullptr};
    ElaLineEdit* _searchEdit{nullptr};
    ElaSuggestModel* _searchModel{nullptr};
    ElaBaseListView* _searchView{nullptr};
    ElaSuggestDelegate* _searchDelegate{nullptr};
    QVBoxLayout* _shadowLayout{nullptr};
    QSize _lastSize;
    bool _isExpandAnimationFinished{true};
    bool _isCloseAnimationFinished{true};
    void _startSizeAnimation(QSize oldSize, QSize newSize);
    void _startExpandAnimation();
    void _startCloseAnimation();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASUGGESTBOXPRIVATE_H_

