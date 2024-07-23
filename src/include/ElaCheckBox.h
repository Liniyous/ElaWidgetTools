#ifndef ELACHECKBOX_H
#define ELACHECKBOX_H

#include <QCheckBox>

#include "Def.h"
#include "stdafx.h"
class ELA_EXPORT ElaCheckBox : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, BorderRadius)
public:
    explicit ElaCheckBox(QWidget* parent = nullptr);
    explicit ElaCheckBox(const QString& text, QWidget* parent = nullptr);
    ~ElaCheckBox();

protected:
    virtual bool event(QEvent* event) override;
    virtual bool hitButton(const QPoint& pos) const override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    bool _isLeftButtonPress{false};
    bool _isHoverButton{false};
    int _textLeftSpacing{10};
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
};

#endif // ELACHECKBOX_H
