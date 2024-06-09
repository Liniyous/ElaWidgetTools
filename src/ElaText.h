#ifndef ELATEXT_H
#define ELATEXT_H

#include <QLabel>

#include "Def.h"

class ElaText : public QLabel
{
    Q_OBJECT
public:
    explicit ElaText(QWidget* parent = nullptr);
    explicit ElaText(QString text, QWidget* parent = nullptr);
    ~ElaText();
    void setTextSize(int size);
    int getTextSize() const;
    void setTextStyle(ElaTextType::TextStyle textStyle);
    ElaTextType::TextStyle getTextStyle() const;
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    ElaTextType::TextStyle _textStyle{ElaTextType::NoStyle};
    qreal _textSpacing{0.5};
    int _textSize{28};
};

#endif // ELATEXT_H
