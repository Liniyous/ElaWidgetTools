#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATABDRAGDATA_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATABDRAGDATA_H_

#include <QMimeData>
#include <QPointer>
#include <QWidget>

class ElaTabWidget;

class ElaTabDragData : public QMimeData
{
    Q_OBJECT
private:
    QPointer<QWidget> _page;        // 拖动的页面
    QPointer<ElaTabWidget> _source; // 页面所属的标签组
    friend class ElaTabWidget;
    friend class ElaTabWidgetPrivate;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELATABDRAGDATA_H_
