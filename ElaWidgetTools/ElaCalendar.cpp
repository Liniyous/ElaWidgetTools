#include "ElaCalendar.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QVBoxLayout>

#include "ElaBaseListView.h"
#include "ElaCalendarDelegate.h"
#include "ElaCalendarModel.h"
#include "ElaCalendarPrivate.h"
#include "ElaCalendarTitleDelegate.h"
#include "ElaCalendarTitleModel.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"
Q_PROPERTY_CREATE_Q_CPP(ElaCalendar, int, BorderRaiuds)
ElaCalendar::ElaCalendar(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaCalendarPrivate())
{
    Q_D(ElaCalendar);
    setFixedSize(305, 340);
    setObjectName("ElaCalendar");
    d->q_ptr = this;
    d->_pBorderRaiuds = 5;

    // 日历标题
    d->_calendarTitleView = new ElaBaseListView(this);
    d->_calendarTitleView->setFlow(QListView::LeftToRight);
    d->_calendarTitleView->setViewMode(QListView::IconMode);
    d->_calendarTitleView->setResizeMode(QListView::Adjust);
    d->_calendarTitleView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarTitleView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarTitleView->setModel(new ElaCalendarTitleModel(this));
    d->_calendarTitleView->setItemDelegate(new ElaCalendarTitleDelegate(this));
    d->_calendarTitleView->setFixedHeight(30);

    // 日历内容
    d->_calendarView = new ElaBaseListView(this);
    d->_calendarView->setFlow(QListView::LeftToRight);
    d->_calendarView->setViewMode(QListView::IconMode);
    d->_calendarView->setResizeMode(QListView::Adjust);
    ElaScrollBar* vScrollBar = dynamic_cast<ElaScrollBar*>(d->_calendarView->verticalScrollBar());
    vScrollBar->setSpeedLimit(6);
    d->_calendarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarView->setSelectionMode(QAbstractItemView::NoSelection);
    d->_calendarModel = new ElaCalendarModel(this);
    d->_calendarView->setModel(d->_calendarModel);
    d->_calendarDelegate = new ElaCalendarDelegate(d->_calendarModel, this);
    d->_calendarView->setItemDelegate(d->_calendarDelegate);
    connect(d->_calendarView, &ElaBaseListView::clicked, d, &ElaCalendarPrivate::onCalendarViewClicked);

    // 模式切换按钮
    d->_modeSwitchButton = new ElaToolButton(this);
    d->_modeSwitchButton->setText("1924年1月");
    QFont switchButtonFont = d->_modeSwitchButton->font();
    switchButtonFont.setWeight(QFont::Bold);
    d->_modeSwitchButton->setFont(switchButtonFont);
    d->_modeSwitchButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    d->_modeSwitchButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    connect(d->_modeSwitchButton, &ElaToolButton::clicked, d, &ElaCalendarPrivate::onSwitchButtonClicked);
    connect(d->_calendarView->verticalScrollBar(), &QScrollBar::valueChanged, d, [=]() {
        d->_updateSwitchButtonText();
    });

    // 翻页按钮
    d->_upButton = new ElaToolButton(this);
    d->_upButton->setFixedSize(36, 36);
    d->_upButton->setElaIcon(ElaIconType::CaretUp);
    connect(d->_upButton, &ElaToolButton::clicked, d, &ElaCalendarPrivate::onUpButtonClicked);

    d->_downButton = new ElaToolButton(this);
    d->_downButton->setFixedSize(36, 36);
    d->_downButton->setElaIcon(ElaIconType::CaretDown);
    connect(d->_downButton, &ElaToolButton::clicked, d, &ElaCalendarPrivate::onDownButtonClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(5, 5, 10, 0);
    buttonLayout->addWidget(d->_modeSwitchButton);
    buttonLayout->addWidget(d->_upButton);
    buttonLayout->addWidget(d->_downButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 0, 0, 0);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(d->_calendarTitleView);
    mainLayout->addWidget(d->_calendarView);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
    setVisible(true);
    QDate currentDate = QDate::currentDate();
    d->_lastSelectedYear = currentDate.year();
    d->_lastSelectedMonth = currentDate.month();
    d->_scrollToDate(currentDate);
}

ElaCalendar::~ElaCalendar()
{
}

void ElaCalendar::setSelectedDate(QDate selectedDate)
{
    Q_D(ElaCalendar);
    if (!selectedDate.isValid() || selectedDate.daysTo(d->_calendarModel->getMaximumDate()) < 0 || selectedDate.daysTo(d->_calendarModel->getMinimumDate()) > 0)
    {
        return;
    }
    d->_pSelectedDate = selectedDate;
    d->_calendarView->selectionModel()->setCurrentIndex(d->_calendarModel->getIndexFromDate(selectedDate), QItemSelectionModel::Select);
    Q_EMIT pSelectedDateChanged();
}

QDate ElaCalendar::getSelectedDate() const
{
    Q_D(const ElaCalendar);
    return d->_pSelectedDate;
    //return d->_calendarModel->getDateFromIndex(d->_calendarView->selectionModel()->currentIndex());
}

void ElaCalendar::setMinimumDate(QDate minimudate)
{
    Q_D(ElaCalendar);
    if (!minimudate.isValid() || minimudate.daysTo(d->_calendarModel->getMaximumDate()) < 0)
    {
        return;
    }
    d->_calendarModel->setMaximumDate(minimudate);
    Q_EMIT pMinimumDateChanged();
}

QDate ElaCalendar::getMinimumDate() const
{
    Q_D(const ElaCalendar);
    return d->_calendarModel->getMinimumDate();
}

void ElaCalendar::setMaximumDate(QDate maximumDate)
{
    Q_D(ElaCalendar);
    if (!maximumDate.isValid() || maximumDate.daysTo(d->_calendarModel->getMinimumDate()) > 0)
    {
        return;
    }
    d->_calendarModel->setMaximumDate(maximumDate);
    Q_EMIT pMaximumDateChanged();
}

QDate ElaCalendar::getMaximumDate() const
{
    Q_D(const ElaCalendar);
    return d->_calendarModel->getMaximumDate();
}

void ElaCalendar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaCalendar);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QRect baseRect = rect();
    baseRect.adjust(d->_borderWidth, d->_borderWidth, -d->_borderWidth, -d->_borderWidth);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
    painter.drawRoundedRect(baseRect, d->_pBorderRaiuds, d->_pBorderRaiuds);
    // 缩放动画
    if (!d->_isSwitchAnimationFinished)
    {
        painter.save();
        QRect pixRect = QRect(baseRect.x(), d->_borderWidth + 45, baseRect.width(), baseRect.height() - 45);
        painter.setOpacity(d->_pPixOpacity);
        painter.translate(pixRect.center());
        painter.scale(d->_pZoomRatio, d->_pZoomRatio);
        painter.translate(-pixRect.center());
        painter.drawPixmap(pixRect, d->_isDrawNewPix ? d->_newCalendarViewPix : d->_oldCalendarViewPix);
        painter.restore();
    }
    // 分割线
    painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), d->_borderWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(baseRect, d->_pBorderRaiuds, d->_pBorderRaiuds);
    painter.drawLine(QPointF(baseRect.x(), 45), QPointF(baseRect.right(), 45));
    painter.restore();
}
