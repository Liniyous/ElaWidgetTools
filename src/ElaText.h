#ifndef ELATEXT_H
#define ELATEXT_H

#include <QLabel>

#include "Def.h"
#include "stdafx.h"
class ElaTextPrivate;
class ELA_EXPORT ElaText : public QLabel
{
    Q_OBJECT
    Q_Q_CREATE(ElaText)
public:
    explicit ElaText(QWidget* parent = nullptr);
    explicit ElaText(QString text, QWidget* parent = nullptr);
    ~ElaText();
    void setTextSize(int size);
    int getTextSize() const;
    void setTextStyle(ElaTextType::TextStyle textStyle);
    ElaTextType::TextStyle getTextStyle() const;

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELATEXT_H
