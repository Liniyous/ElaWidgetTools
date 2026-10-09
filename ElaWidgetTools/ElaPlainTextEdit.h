#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAPLAINTEXTEDIT_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAPLAINTEXTEDIT_H_

#include <QPlainTextEdit>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaPlainTextEditPrivate;
class ELA_EXPORT ElaPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaPlainTextEdit)
public:
    explicit ElaPlainTextEdit(QWidget* parent = nullptr);
    explicit ElaPlainTextEdit(const QString& text, QWidget* parent = nullptr);
    ~ElaPlainTextEdit() override;

protected:
    virtual void focusInEvent(QFocusEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAPLAINTEXTEDIT_H_
