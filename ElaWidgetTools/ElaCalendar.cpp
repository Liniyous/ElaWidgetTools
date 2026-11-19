#include "ElaCalendar.h"

#include "ElaBaseListView.h"
#include "ElaCalendarDelegate.h"
#include "ElaCalendarModel.h"
#include "ElaCalendarPrivate.h"
#include "ElaCalendarTitleDelegate.h"
#include "ElaCalendarTitleModel.h"
#include "ElaColorAnimation.h"
#include "ElaScrollAnimation.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"
#include "ElaToolButtonStyle.h"
#include "ElaWidgetToolsDef.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QScopedValueRollback>
#include <QTimer>
#include <QVBoxLayout>
#include <QVariantAnimation>

ELA_PROPERTY_Q_CPP(ElaCalendar, int, BorderRaiuds)
ElaCalendar::ElaCalendar(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaCalendarPrivate())
{
    Q_D(ElaCalendar);
    setObjectName("ElaCalendar");
    d->q_ptr = this;
    d->_pBorderRaiuds = 8;

    // 日历标题
    d->_calendarTitleView = new ElaBaseListView(this);
    d->_calendarTitleView->setFlow(QListView::LeftToRight);
    d->_calendarTitleView->setViewMode(QListView::IconMode);
    d->_calendarTitleView->setResizeMode(QListView::Adjust);
    d->_calendarTitleView->setUniformItemSizes(true);
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
    d->_calendarView->setUniformItemSizes(true);
    ElaScrollBar* vScrollBar = dynamic_cast<ElaScrollBar*>(d->_calendarView->verticalScrollBar());
    vScrollBar->setSpeedLimit(6);
    d->_calendarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarView->setSelectionMode(QAbstractItemView::NoSelection);
    d->_calendarModel = new ElaCalendarModel(this);
    d->_calendarView->setModel(d->_calendarModel);
    d->_calendarDelegate = new ElaCalendarDelegate(d->_calendarModel, this);
    d->_calendarView->setItemDelegate(d->_calendarDelegate);
    // 视口保留完整行 使日期范围两端也能对齐
    d->_calendarView->setFixedHeight(d->_calendarDelegate->getItemHeight() * 6);
    connect(d->_calendarModel, &ElaCalendarModel::displayModeChanged, d->_calendarView, [d]() {
        d->_calendarView->setFixedHeight(d->_calendarDelegate->getItemHeight() * (d->_calendarModel->getDisplayMode() == DayMode ? 6 : 4));
    });
    connect(d->_calendarView, &ElaBaseListView::mousePress, this, [d](const QModelIndex& index) {
        d->_calendarDelegate->setPressIndex(index);
        d->_calendarView->viewport()->update();
    });
    connect(d->_calendarView, &ElaBaseListView::mouseRelease, this, [d]() {
        d->_calendarDelegate->setPressIndex(QPersistentModelIndex());
        d->_calendarView->viewport()->update();
    });
    connect(d->_calendarView, &ElaBaseListView::clicked, d, &ElaCalendarPrivate::onCalendarViewClicked);

    // 模式切换按钮
    d->_modeSwitchButton = new ElaToolButton(this);
    d->_modeSwitchButton->setAccessibleName("切换日历视图");
    d->_modeSwitchButton->setToolTip("选择月份或年份");
    QFont switchButtonFont = d->_modeSwitchButton->font();
    switchButtonFont.setWeight(QFont::DemiBold);
    d->_modeSwitchButton->setFont(switchButtonFont);
    d->_modeSwitchButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    d->_modeSwitchButton->findChild<ElaToolButtonStyle*>()->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    d->_modeSwitchButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(d->_modeSwitchButton, &ElaToolButton::clicked, d, &ElaCalendarPrivate::onSwitchButtonClicked);
    connect(d->_calendarView->verticalScrollBar(), &QScrollBar::valueChanged, d, [=]() {
        if (!d->_isUpdatingDateWindow && d->_isSwitchAnimationFinished)
        {
            d->_updateSwitchButtonText();
            d->_dateWindowTimer->start();
        }
    });

    // 翻页按钮
    d->_upButton = new ElaToolButton(this);
    d->_upButton->setFixedWidth(ElaWidgetToolsSize::DefaultControlHeight);
    d->_upButton->setIconSize(QSize(16, 16));
    d->_upButton->setAccessibleName("上一页");
    d->_upButton->setToolTip("上一页");
    connect(d->_upButton, &ElaToolButton::clicked, d, &ElaCalendarPrivate::onUpButtonClicked);

    d->_downButton = new ElaToolButton(this);
    d->_downButton->setFixedWidth(ElaWidgetToolsSize::DefaultControlHeight);
    d->_downButton->setIconSize(QSize(16, 16));
    d->_downButton->setAccessibleName("下一页");
    d->_downButton->setToolTip("下一页");
    connect(d->_downButton, &ElaToolButton::clicked, d, &ElaCalendarPrivate::onDownButtonClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(5, 6, 6, 6);
    buttonLayout->setSpacing(2);
    buttonLayout->addWidget(d->_modeSwitchButton);
    buttonLayout->addWidget(d->_upButton);
    buttonLayout->addWidget(d->_downButton);
    d->_headerHeight = buttonLayout->sizeHint().height();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(d->_calendarTitleView);
    mainLayout->addWidget(d->_calendarView);
    mainLayout->addStretch();
    // 保留月视图和年视图所需的额外空间 避免切换时弹层跳动
    setFixedSize(305, mainLayout->sizeHint().height() + 14);

    auto viewPolicy = d->_calendarView->sizePolicy();
    viewPolicy.setRetainSizeWhenHidden(true);
    d->_calendarView->setSizePolicy(viewPolicy);
    d->_dateWindowTimer = new QTimer(d);
    d->_dateWindowTimer->setSingleShot(true);
    d->_dateWindowTimer->setInterval(0);
    connect(d->_dateWindowTimer, &QTimer::timeout, d, &ElaCalendarPrivate::onVisibleDateWindowTimeout);
    d->_wheelAnimation = vScrollBar->findChild<ElaScrollAnimation*>();
    d->_calendarView->viewport()->installEventFilter(d);
    d->_scrollAnimation = new QPropertyAnimation(vScrollBar, "value", d);
    d->_scrollAnimation->setDuration(200);
    QEasingCurve scrollEasing(QEasingCurve::BezierSpline);
    scrollEasing.addCubicBezierSegment(QPointF(0, 0.35), QPointF(0.15, 1), QPointF(1, 1));
    d->_scrollAnimation->setEasingCurve(scrollEasing);
    connect(d->_scrollAnimation, &QPropertyAnimation::finished, d, [d]() {
        const QScopedValueRollback<bool> updating(d->_isUpdatingDateWindow, true);
        d->_displayDate = d->_pageDate;
        d->_pageDate = QDate();
        d->_updateSwitchButtonText();
        d->_dateWindowTimer->start();
    });
    d->_switchAnimation = new QVariantAnimation(d);
    d->_switchAnimation->setStartValue(0.0);
    d->_switchAnimation->setEndValue(1.0);
    d->_switchAnimation->setDuration(500);
    QEasingCurve switchEasing(QEasingCurve::BezierSpline);
    switchEasing.addCubicBezierSegment(QPointF(0, 0), QPointF(0, 1), QPointF(1, 1));
    // 前 150 ms 退出旧视图 后 350 ms 进入新视图
    connect(d->_switchAnimation, &QVariantAnimation::valueChanged, this, [this, d, switchEasing](const QVariant& value) {
        const qreal progress = value.toReal();
        d->_isDrawNewPix = progress >= 0.3;
        const qreal phase = switchEasing.valueForProgress(d->_isDrawNewPix ? (progress - 0.3) / 0.7 : progress / 0.3);
        const qreal zoom = d->_isDrawNewPix ? (d->_isZoomIn ? 0.84 : 1.29) : (d->_isZoomIn ? 1.29 : 0.84);
        d->_zoomRatio = d->_isDrawNewPix ? zoom + (1 - zoom) * phase : 1 + (zoom - 1) * phase;
        d->_pixOpacity = d->_isDrawNewPix ? phase : 1 - phase;
        update();
    });
    connect(d->_switchAnimation, &QVariantAnimation::finished, this, [this, d]() {
        d->_isSwitchAnimationFinished = true;
        auto titlePolicy = d->_calendarTitleView->sizePolicy();
        titlePolicy.setRetainSizeWhenHidden(false);
        d->_calendarTitleView->setSizePolicy(titlePolicy);
        d->_calendarTitleView->setVisible(d->_calendarModel->getDisplayMode() == DayMode);
        d->_calendarView->show();
        d->_oldCalendarViewPix = QPixmap();
        d->_newCalendarViewPix = QPixmap();
        d->_updateSwitchButtonText();
        update();
    });

    d->_themeMode = eTheme->getThemeMode();
    d->_updateNavigationIcons();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        d->_updateNavigationIcons();
    });
    setVisible(true);
    QDate currentDate = QDate::currentDate();
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
    d->_scrollToDate(selectedDate);
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
    d->_calendarModel->setMinimumDate(minimudate);
    if (d->_pSelectedDate.isValid() && d->_pSelectedDate < minimudate)
    {
        setSelectedDate(minimudate);
    }
    else
    {
        d->_scrollToDate(d->_pSelectedDate.isValid() ? d->_pSelectedDate : QDate::currentDate());
    }
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
    if (d->_pSelectedDate.isValid() && d->_pSelectedDate > maximumDate)
    {
        setSelectedDate(maximumDate);
    }
    else
    {
        d->_scrollToDate(d->_pSelectedDate.isValid() ? d->_pSelectedDate : QDate::currentDate());
    }
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
    painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(d->_themeMode, PopupBase), "Background"));
    painter.drawRoundedRect(baseRect, d->_pBorderRaiuds, d->_pBorderRaiuds);
    // 缩放动画
    if (!d->_isSwitchAnimationFinished)
    {
        painter.save();
        QRect pixRect = baseRect;
        pixRect.setTop(d->_headerHeight);
        QPainterPath clipPath;
        clipPath.addRoundedRect(baseRect, d->_pBorderRaiuds, d->_pBorderRaiuds);
        QPainterPath contentClipPath;
        contentClipPath.addRect(pixRect);
        painter.setClipPath(clipPath.intersected(contentClipPath));
        painter.setOpacity(d->_pixOpacity);
        const QPointF pixCenter = QRectF(pixRect).center();
        painter.translate(pixCenter);
        painter.scale(d->_zoomRatio, d->_zoomRatio);
        painter.translate(-pixCenter);
        painter.drawPixmap(pixRect.topLeft(), d->_isDrawNewPix ? d->_newCalendarViewPix : d->_oldCalendarViewPix);
        painter.restore();
    }
    // 分割线
    painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), d->_borderWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(baseRect, d->_pBorderRaiuds, d->_pBorderRaiuds);
    painter.restore();
}
