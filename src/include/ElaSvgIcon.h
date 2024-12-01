#ifndef ELASVGICON_H
#define ELASVGICON_H

#include <QObject>
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QSvgRenderer>

#include "Def.h"
#include "singleton.h"
#include "stdafx.h"

#define eSvg ElaSvgIcon::getInstance()
class ElaSvgIconPrivate;
class ELA_EXPORT ElaSvgIcon : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaSvgIcon)
    Q_SINGLETON_CREATE_H(ElaSvgIcon)
public:
    ~ElaSvgIcon();

    QIcon getSvgIcon(SvgIconType::IconName awesome);
    QIcon getSvgIcon(SvgIconType::IconName awesome, QColor iconColor);
    QIcon getSvgIcon(SvgIconType::IconName awesome, int pixelSize);
    QIcon getSvgIcon(SvgIconType::IconName awesome, int pixelSize, QColor iconColor);
    QIcon getSvgIcon(SvgIconType::IconName awesome, int fixedWidth, int fixedHeight);
    QIcon getSvgIcon(SvgIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QColor iconColor);

private:
    explicit ElaSvgIcon(QObject* parent = nullptr);

};

#endif// ELASVGICON_H
