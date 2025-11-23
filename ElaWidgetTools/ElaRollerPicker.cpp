#include "ElaRollerPicker.h"

#include "ElaRollerPickerPrivate.h"
#include "ElaTheme.h"
#include <QDebug>
#include <QPainter>
#include <QVBoxLayout>

Q_PROPERTY_CREATE_Q_CPP(ElaRollerPicker, int, BorderRadius)
ElaRollerPicker::ElaRollerPicker(QWidget* parent)
    : QPushButton{parent}, d_ptr(new ElaRollerPickerPrivate())
{
    Q_D(ElaRollerPicker);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    setMouseTracking(true);
    setFixedSize(90, 30);
    setObjectName("ElaRollerPicker");
    setStyleSheet("#ElaRollerPicker{background-color:transparent;}");
    QFont font = this->font();
    font.setPixelSize(16);
    setFont(font);

    d->_rollerPickerContainer = new ElaRollerPickerContainer(this);
    d->_rollerPickerContainer->resize(90, d->_pickerRollerHeight + d->_rollerPickerContainer->getButtonAreaHeight());
    d->_rollerPickerContainer->hide();
    connect(this, &QPushButton::clicked, d, &ElaRollerPickerPrivate::onRollerPickerClicked);
    connect(d->_rollerPickerContainer, &ElaRollerPickerContainer::overButtonClicked, d, &ElaRollerPickerPrivate::onOverButtonClicked);
    connect(d->_rollerPickerContainer, &ElaRollerPickerContainer::cancelButtonClicked, d, &ElaRollerPickerPrivate::onCancelButtonClicked);

    d->_containerLayout = new QHBoxLayout(d->_rollerPickerContainer);
    d->_containerLayout->setSpacing(0);
    d->_containerLayout->setContentsMargins(0, 0, 0, 0);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaRollerPicker::~ElaRollerPicker()
{
}

void ElaRollerPicker::addRoller(const QStringList& itemList, bool isEnableLoop)
{
    Q_D(ElaRollerPicker);
    if (itemList.isEmpty())
    {
        return;
    }
    ElaRoller* roller = new ElaRoller(this);
    roller->setIsContainer(true);
    roller->setItemList(itemList);
    roller->setMaxVisibleItems(7);
    roller->setIsEnableLoop(isEnableLoop);
    roller->setItemHeight(35);
    roller->setFixedHeight(d->_pickerRollerHeight);
    connect(roller, &ElaRoller::currentDataChanged, this, [=]() {
        Q_EMIT currentDataSelectionChanged(getCurrentData());
    });
    d->_rollerPickerContainer->_rollerList.append(roller);
    d->_containerLayout->addWidget(roller);
    setFixedWidth(d->_getRollerTotalWidth());
}

void ElaRollerPicker::removeRoller(int index)
{
    Q_D(ElaRollerPicker);
    if (index >= d->_rollerPickerContainer->_rollerList.count())
    {
        return;
    }
    auto roller = d->_rollerPickerContainer->_rollerList[index];
    d->_rollerPickerContainer->_rollerList.removeAt(index);
    d->_containerLayout->removeWidget(roller);
    roller->deleteLater();
    setFixedWidth(d->_getRollerTotalWidth());
}

void ElaRollerPicker::setRollerItemList(int index, const QStringList& itemList)
{
    Q_D(ElaRollerPicker);
    if (index >= d->_rollerPickerContainer->_rollerList.count())
    {
        return;
    }
    auto roller = d->_rollerPickerContainer->_rollerList[index];
    roller->setItemList(itemList);
}

void ElaRollerPicker::setRollerWidth(int index, int width)
{
    Q_D(ElaRollerPicker);
    if (index >= d->_rollerPickerContainer->_rollerList.count())
    {
        return;
    }
    auto roller = d->_rollerPickerContainer->_rollerList[index];
    roller->setFixedWidth(width);
    setFixedWidth(d->_getRollerTotalWidth());
}

void ElaRollerPicker::setCurrentData(int index, const QString& data)
{
    Q_D(ElaRollerPicker);
    if (index >= d->_rollerPickerContainer->_rollerList.count())
    {
        return;
    }
    d->_rollerPickerContainer->_rollerList[index]->setCurrentData(data);
}

void ElaRollerPicker::setCurrentData(const QStringList& dataList)
{
    Q_D(ElaRollerPicker);
    for (int i = 0; i < d->_rollerPickerContainer->_rollerList.count(); i++)
    {
        if (dataList.count() <= i)
        {
            return;
        }
        auto roller = d->_rollerPickerContainer->_rollerList[i];
        roller->setCurrentData(dataList[i]);
    }
}

QString ElaRollerPicker::getCurrentData(int index) const
{
    Q_D(const ElaRollerPicker);
    if (index >= d->_rollerPickerContainer->_rollerList.count())
    {
        return {};
    }
    return d->_rollerPickerContainer->_rollerList[index]->getCurrentData();
}

QStringList ElaRollerPicker::getCurrentData() const
{
    Q_D(const ElaRollerPicker);
    QStringList dataList;
    for (auto roller: d->_rollerPickerContainer->_rollerList)
    {
        dataList.append(roller->getCurrentData());
    }
    return dataList;
}

void ElaRollerPicker::setCurrentIndex(int rollerIndex, int index)
{
    Q_D(ElaRollerPicker);
    if (rollerIndex >= d->_rollerPickerContainer->_rollerList.count())
    {
        return;
    }
    d->_rollerPickerContainer->_rollerList[rollerIndex]->setCurrentIndex(index);
}

void ElaRollerPicker::setCurrentIndex(const QList<int>& indexList)
{
    Q_D(ElaRollerPicker);
    for (int i = 0; i < d->_rollerPickerContainer->_rollerList.count(); i++)
    {
        if (indexList.count() <= i)
        {
            return;
        }
        auto roller = d->_rollerPickerContainer->_rollerList[i];
        roller->setCurrentIndex(indexList[i]);
    }
}

int ElaRollerPicker::getCurrentIndex(int rollerIndex) const
{
    Q_D(const ElaRollerPicker);
    if (rollerIndex >= d->_rollerPickerContainer->_rollerList.count())
    {
        return -1;
    }
    return d->_rollerPickerContainer->_rollerList[rollerIndex]->getCurrentIndex();
}

QList<int> ElaRollerPicker::getCurrentIndex() const
{
    Q_D(const ElaRollerPicker);
    QList<int> currentIndexList;
    for (auto roller: d->_rollerPickerContainer->_rollerList)
    {
        currentIndexList.append(roller->getCurrentIndex());
    }
    return currentIndexList;
}

void ElaRollerPicker::paintEvent(QPaintEvent* event)
{
    Q_D(ElaRollerPicker);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 边框和背景绘制
    QRect baseRect = rect();
    baseRect.adjust(1, 1, -1, -1);
    painter.setPen(ElaThemeColor(d->_themeMode, BasicBorder));
    painter.setBrush(underMouse() ? ElaThemeColor(d->_themeMode, BasicHover) : ElaThemeColor(d->_themeMode, BasicBase));
    painter.drawRoundedRect(baseRect, d->_pBorderRadius, d->_pBorderRadius);

    // Roller内容绘制
    int rollerXOffset = 0;
    for (int i = 0; i < d->_rollerPickerContainer->_rollerList.count(); i++)
    {
        auto roller = d->_rollerPickerContainer->_rollerList[i];
        painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
        int rollerWidth = roller->width();
        painter.drawText(QRect(rollerXOffset, baseRect.y(), rollerWidth, baseRect.height()), Qt::AlignCenter, roller->getCurrentData());
        rollerXOffset += rollerWidth;
        if (i != d->_rollerPickerContainer->_rollerList.count() - 1)
        {
            painter.setPen(ElaThemeColor(d->_themeMode, BasicBorder));
            painter.drawLine(rollerXOffset, 0, rollerXOffset, baseRect.bottom());
        }
    }
    painter.restore();
}
