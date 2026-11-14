#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ICON_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ICON_H_

#include <QMetaEnum>

#include "T_BasePage.h"
class ElaLineEdit;
class ElaText;
class ElaListView;
class T_IconModel;
class T_IconDelegate;
class T_Icon : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Icon(QWidget* parent = nullptr);
    ~T_Icon();
    Q_SLOT void onSearchEditTextEdit(const QString& searchText);

private:
    QMetaEnum _metaEnum;
    ElaLineEdit* _searchEdit{nullptr};
    ElaListView* _iconView{nullptr};
    T_IconModel* _iconModel{nullptr};
    T_IconDelegate* _iconDelegate{nullptr};
    ElaText* _resultText{nullptr};
    ElaText* _emptyText{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ICON_H_


