#include "T_Home.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QVBoxLayout>

#include "ElaAcrylicUrlCard.h"
#include "ElaFlowLayout.h"
#include "ElaImageCard.h"
#include "ElaMenu.h"
#include "ElaMessageBar.h"
#include "ElaNavigationRouter.h"
#include "ElaReminderCard.h"
#include "ElaScrollArea.h"
#include "ElaText.h"
T_Home::T_Home(QWidget* parent)
    : ElaScrollPage(parent)
{
    setPageTitleSpacing(5);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Home");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    ElaImageCard* backgroundCard = new ElaImageCard(this);
    backgroundCard->setCardImage(QImage(":/include/Image/Home_Background.png"));

    ElaText* flowLayoutText = new ElaText("Flow Layout", this);
    // ElaFlowLayout
    ElaReminderCard* homeCard1 = new ElaReminderCard(this);
    connect(homeCard1, &ElaReminderCard::clicked, this, [=]() { Q_EMIT elaScreenNavigation(); });
    homeCard1->setTitle("ElaScreen");
    homeCard1->setSubTitle("Use ElaDxgiManager To Grab Screen");
    homeCard1->setCardPixmap(QPixmap(":/include/Image/control/AutomationProperties.png"));

    ElaReminderCard* homeCard2 = new ElaReminderCard(this);
    connect(homeCard2, &ElaReminderCard::clicked, this, [=]() { Q_EMIT elaSceneNavigation(); });
    homeCard2->setTitle("ElaScene");
    homeCard2->setSubTitle("Use ElaScene To Create Link Work");
    homeCard2->setCardPixmap(QPixmap(":/include/Image/control/BreadcrumbBar.png"));

    ElaReminderCard* homeCard3 = new ElaReminderCard(this);
    connect(homeCard3, &ElaReminderCard::clicked, this, [=]() { Q_EMIT elaBaseComponentNavigation(); });
    homeCard3->setTitle("ElaBaseComponent");
    homeCard3->setSubTitle("Use ElaBaseComponent To Start Project");
    homeCard3->setCardPixmap(QPixmap(":/include/Image/control/StandardUICommand.png"));

    ElaReminderCard* homeCard4 = new ElaReminderCard(this);
    connect(homeCard4, &ElaReminderCard::clicked, this, [=]() { Q_EMIT elaSceneNavigation(); });
    homeCard4->setTitle("ElaButton");
    homeCard4->setSubTitle("Use ElaButton To Trigger Action");
    homeCard4->setCardPixmap(QPixmap(":/include/Image/control/MenuFlyout.png"));

    ElaReminderCard* homeCard5 = new ElaReminderCard(this);
    connect(homeCard5, &ElaReminderCard::clicked, this, [=]() { Q_EMIT elaIconNavigation(); });
    homeCard5->setTitle("ElaIcon");
    homeCard5->setSubTitle("Use ElaIcon To Create A Light Icon");
    homeCard5->setCardPixmap(QPixmap(":/include/Image/control/Canvas.png"));
    ElaFlowLayout* flowLayout = new ElaFlowLayout(0, 5, 5);
    flowLayout->setIsAnimation(true);
    flowLayout->addWidget(homeCard1);
    flowLayout->addWidget(homeCard2);
    flowLayout->addWidget(homeCard3);
    flowLayout->addWidget(homeCard4);
    flowLayout->addWidget(homeCard5);

    centerVLayout->addWidget(backgroundCard);
    centerVLayout->addWidget(flowLayoutText);
    centerVLayout->addLayout(flowLayout);
    centerVLayout->setSpacing(20);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget);
    ElaText* homeStack1 = new ElaText("HomeStack1", this);
    QFont font = homeStack1->font();
    font.setPointSize(25);
    homeStack1->setFont(font);
    homeStack1->setAlignment(Qt::AlignCenter);
    homeStack1->setWindowTitle("HomeStack1");
    addCentralWidget(homeStack1);
    ElaText* homeStack2 = new ElaText("HomeStack2", this);
    homeStack2->setFont(font);
    homeStack2->setAlignment(Qt::AlignCenter);
    homeStack2->setWindowTitle("HomeStack2");
    addCentralWidget(homeStack2);

    ElaAcrylicUrlCard* urlCard1 = new ElaAcrylicUrlCard(this);
    urlCard1->setUrl("https://github.com/Liniyous/ElaWidgetTools");
    urlCard1->setCardPixmap(QPixmap(":/include/Image/github.png"));
    urlCard1->setTitle("ElaTool Github");
    urlCard1->setSubTitle("Use ElaWidgetTools To Create A Cool Project");
    ElaAcrylicUrlCard* urlCard2 = new ElaAcrylicUrlCard(this);
    urlCard2->setUrl("https://space.bilibili.com/21256707");
    urlCard2->setCardPixmap(QPixmap(":/include/Image/Moon.jpg"));
    urlCard2->setTitle("ElaWidgetTool");
    urlCard2->setSubTitle("8009963@qq.com");

    ElaScrollArea* cardScrollArea = new ElaScrollArea(backgroundCard);
    cardScrollArea->setWidgetResizable(true);
    cardScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardScrollArea->setIsGrabGesture(Qt::Horizontal, true);
    QWidget* cardScrollAreaWidget = new QWidget(this);
    cardScrollAreaWidget->setStyleSheet("background-color:transparent;");
    cardScrollArea->setWidget(cardScrollAreaWidget);
    QHBoxLayout* cardScrollAreaWidgetLayout = new QHBoxLayout(cardScrollAreaWidget);
    cardScrollAreaWidgetLayout->setSpacing(15);
    cardScrollAreaWidgetLayout->setContentsMargins(20, 40, 0, 0);
    cardScrollAreaWidgetLayout->addWidget(urlCard1);
    cardScrollAreaWidgetLayout->addWidget(urlCard2);
    cardScrollAreaWidgetLayout->addStretch();

    QHBoxLayout* backgroundLayout = new QHBoxLayout(backgroundCard);
    backgroundLayout->addWidget(cardScrollArea);
    backgroundLayout->setContentsMargins(0, 0, 0, 0);

    // 菜单
    _homeMenu = new ElaMenu(this);
    ElaMenu* checkMenu = _homeMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu->addAction("查看1");
    checkMenu->addAction("查看2");
    checkMenu->addAction("查看3");
    checkMenu->addAction("查看4");

    ElaMenu* checkMenu1 = _homeMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu1->addAction("查看1");
    checkMenu1->addAction("查看2");
    checkMenu1->addAction("查看3");
    checkMenu1->addAction("查看4");

    ElaMenu* checkMenu2 = checkMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu2->addAction("查看1");
    checkMenu2->addAction("查看2");
    checkMenu2->addAction("查看3");
    checkMenu2->addAction("查看4");

    // QKeySequence key = QKeySequence(Qt::CTRL | Qt::Key_S);

    _homeMenu->addSeparator();
    _homeMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::Save);
    _homeMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    QAction* action = _homeMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    connect(action, &QAction::triggered, this, [=]() {
        ElaNavigationRouter::getInstance()->navigationRouteBack();
    });

    _homeMenu->addElaIconAction(ElaIconType::Copy, "复制");
    _homeMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");

    // 初始化提示
    ElaMessageBar::success(ElaMessageBarType::BottomRight, "Success", "初始化成功!", 2000);
    qDebug() << "初始化成功";
}

T_Home::~T_Home()
{
}

void T_Home::mouseReleaseEvent(QMouseEvent* event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
    {
        //ElaMessageBar::success(ElaMessageBarType::TopRight, "Success", "Never Close Your Eyes", 2500);
        //ElaMessageBar::success(ElaMessageBarType::TopRight, "Success", "Never Close Your Eyes", 1500);
        break;
    }
    case Qt::RightButton:
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        _homeMenu->popup(event->globalPosition().toPoint());
#else
        _homeMenu->popup(event->globalPos());
#endif
        break;
    }
    case Qt::BackButton:
    {
        this->navigation(0);
        break;
    }
    case Qt::ForwardButton:
    {
        this->navigation(1);
        break;
    }
    case Qt::MiddleButton:
    {
        this->navigation(2);
        break;
    }
    default:
    {
        break;
    }
    }
    ElaScrollPage::mouseReleaseEvent(event);
}
