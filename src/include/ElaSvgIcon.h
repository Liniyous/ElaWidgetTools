#ifndef ELASVGICON_H
#define ELASVGICON_H

#include <QObject>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QSvgRenderer>
#include <QDebug>

#include "Def.h"
#include "stdafx.h"

class ElaSvgIconPrivate;
class ELA_EXPORT ElaSvgIcon : public QLabel
{
    Q_OBJECT
    Q_Q_CREATE(ElaSvgIcon)

public:
    explicit ElaSvgIcon(QWidget* parent = nullptr);
    ~ElaSvgIcon();

    void setSvgIcon(SvgIconType::IconName icon);

private:


protected:
    virtual void paintEvent(QPaintEvent* event) override;

};

#endif// ELASVGICON_H
