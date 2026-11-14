#include "ElaSplitter.h"

#include "ElaSplitterHandle.h"

ElaSplitter::ElaSplitter(QWidget* parent)
    : ElaSplitter(Qt::Horizontal, parent)
{
}

ElaSplitter::ElaSplitter(Qt::Orientation orientation, QWidget* parent)
    : QSplitter(orientation, parent)
{
    setFrameShape(QFrame::NoFrame);
    setHandleWidth(5);
}

QSplitterHandle* ElaSplitter::createHandle()
{
    return new ElaSplitterHandle(orientation(), this);
}
