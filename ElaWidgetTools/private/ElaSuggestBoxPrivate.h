#ifndef ELASUGGESTBOXPRIVATE_H
#define ELASUGGESTBOXPRIVATE_H

#include <QAction>
#include <QObject>
#include <QSize>
#include <QVariantMap>
#include <QVector>

#include "ElaDef.h"
class ElaSuggestion : public QObject
{
    Q_OBJECT
    Q_PROPERTY_CREATE(ElaIconType::IconName, ElaIcon)
    Q_PROPERTY_CREATE(QString, SuggestText)
    Q_PROPERTY_CREATE(QString, SuggestKey)
    Q_PROPERTY_CREATE(QVariantMap, SuggestData)
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
    Q_D_CREATE(ElaSuggestBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(Qt::CaseSensitivity, CaseSensitivity)
public:
    explicit ElaSuggestBoxPrivate(QObject* parent = nullptr);
    ~ElaSuggestBoxPrivate();
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

#endif // ELASUGGESTBOXPRIVATE_H
