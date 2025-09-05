#ifndef ELAPLAINTEXTEDIT_H
#define ELAPLAINTEXTEDIT_H

#include <QPlainTextEdit>

#include "ElaProperty.h"

class ElaPlainTextEditPrivate;
class ELA_EXPORT ElaPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
    Q_Q_CREATE(ElaPlainTextEdit)
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

#endif // ELAPLAINTEXTEDIT_H
