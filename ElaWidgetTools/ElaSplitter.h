#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELASPLITTER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELASPLITTER_H_

#include "ElaWidgetToolsExport.h"

#include <QSplitter>

// 使用 Ela 主题分割线的可调节布局
class ELA_EXPORT ElaSplitter : public QSplitter
{
    Q_OBJECT
public:
    explicit ElaSplitter(QWidget* parent = nullptr);
    explicit ElaSplitter(Qt::Orientation orientation, QWidget* parent = nullptr);

protected:
    QSplitterHandle* createHandle() override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELASPLITTER_H_
