#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELASPLITTERHANDLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELASPLITTERHANDLE_H_

#include <QSplitterHandle>

// 可拖动区域中央的单像素主题分割线
class ElaSplitterHandle : public QSplitterHandle
{
    Q_OBJECT
public:
    ElaSplitterHandle(Qt::Orientation orientation, QSplitter* parent);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELASPLITTERHANDLE_H_
