#include "T_TabWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "ElaTabWidget.h"
T_TabWidget::T_TabWidget(QWidget* parent)
    : ElaScrollPage(parent)
{
    _tabWidget = new ElaTabWidget(this);
    QLabel* page1 = new QLabel("新标签页1", this);
    page1->setAlignment(Qt::AlignCenter);
    QFont font = page1->font();
    font.setPointSize(20);
    page1->setFont(font);
    QLabel* page2 = new QLabel("新标签页2", this);
    page2->setFont(font);
    page2->setAlignment(Qt::AlignCenter);
    QLabel* page3 = new QLabel("新标签页3", this);
    page3->setFont(font);
    page3->setAlignment(Qt::AlignCenter);
    QLabel* page4 = new QLabel("新标签页4", this);
    page4->setFont(font);
    page4->setAlignment(Qt::AlignCenter);
    _tabWidget->addTab(page1, QIcon(":/Resource/Image/Cirno.jpg"), "新标签页1");
    _tabWidget->addTab(page2, "新标签页2");
    _tabWidget->addTab(page3, "新标签页3");
    _tabWidget->addTab(page4, "新标签页4");
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaTabWidget");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(_tabWidget);

    addCentralWidget(centralWidget, true, true, 0);
}

T_TabWidget::~T_TabWidget()
{
}
