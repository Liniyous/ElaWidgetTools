#ifndef ELATEXTEDIT_H
#define ELATEXTEDIT_H

#include <QPlainTextEdit>
#include <QTextEdit>

#include "Def.h"
#include "stdafx.h"
// #include "ElaTextEditPrivate.h"
class ElaTextEditPrivate;
class ELA_EXPORT ElaTextEdit : public QPlainTextEdit
{
    Q_OBJECT
    Q_Q_CREATE(ElaTextEdit)
public:
    explicit ElaTextEdit(QWidget* parent = nullptr);
    explicit ElaTextEdit(QString text, QWidget* parent = nullptr);
    explicit ElaTextEdit(QString text, int pixelSize, QWidget* parent = nullptr);
    ~ElaTextEdit() override;

    void setTextPixelSize(int size);
    int getTextPixelSize() const;
    void setTextPointSize(int size);
    int getTextPointSize() const;
    void setTextStyle(ElaTextType::TextStyle textStyle);
    ElaTextType::TextStyle getTextStyle() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELATEXTEDIT_H
