#ifndef T_ICON_H
#define T_ICON_H

#include <QMetaEnum>

#include "ElaScrollPage.h"
class ElaLineEdit;
class ElaTableView;
class T_IconModel;
class T_IconDelegate;
class T_Icon : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_Icon(QWidget* parent = nullptr);
    ~T_Icon();
    Q_SLOT void onSearchEditTextEdit(const QString& searchText);

protected:
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    QMetaEnum _metaEnum;
    ElaLineEdit* _searchEdit{nullptr};
    ElaTableView* _iconView;
    T_IconModel* _iconModel;
    T_IconDelegate* _iconDelegate;
};

#endif // T_ICON_H
