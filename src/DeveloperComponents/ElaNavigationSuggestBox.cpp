#include "ElaNavigationSuggestBox.h"

#include <QAction>
#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QMap>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>

#include "ElaEventBus.h"
#include "ElaIcon.h"
#include "ElaLineEdit.h"
#include "ElaNavigationSuggestDelegate.h"
#include "ElaNavigationSuggestModel.h"
#include "ElaNavigationSuggestView.h"
#include "private/ElaNavigationSuggestBoxPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationSuggestBox, int, BorderRadius)
ElaNavigationSuggestBox::ElaNavigationSuggestBox(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaNavigationSuggestBoxPrivate())
{
    Q_D(ElaNavigationSuggestBox);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    d->_searchEdit = new ElaLineEdit(this);
    d->_searchEdit->setFixedSize(280, 35);
    d->_searchEdit->setPlaceholderText("查找功能");
    d->_searchEdit->setClearButtonEnabled(true);
    QAction* searchAction = new QAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Fingerprint), "Search", this);
    d->_searchEdit->addAction(searchAction, QLineEdit::TrailingPosition);
    connect(searchAction, &QAction::triggered, this, [=](bool checked) { qDebug() << "Search"; });
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(d->_searchEdit);
    d->_searchView = new ElaNavigationSuggestView(window());
    d->_searchView->resize(280, 300);
    d->_searchView->setFixedWidth(278);
    d->_searchModel = new ElaNavigationSuggestModel(this);
    d->_searchDelegate = new ElaNavigationSuggestDelegate(this);
    d->_searchView->setModel(d->_searchModel);
    d->_searchView->setItemDelegate(d->_searchDelegate);
    d->_searchView->move(16, 174);
    d->_searchView->hide();
    connect(d->_searchEdit, &ElaLineEdit::textEdited, d, &ElaNavigationSuggestBoxPrivate::onSearchEditTextEdit);
    connect(d->_searchEdit, &ElaLineEdit::focusIn, d, &ElaNavigationSuggestBoxPrivate::onSearchEditTextEdit);
    connect(d->_searchView, &ElaNavigationSuggestView::clicked, d, &ElaNavigationSuggestBoxPrivate::onSearchViewPressed);

    // 事件总线
    d->_focusEvent = new ElaEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();
}

ElaNavigationSuggestBox::~ElaNavigationSuggestBox()
{
}

void ElaNavigationSuggestBox::setPlaceholderText(QString placeholderText)
{
    Q_D(ElaNavigationSuggestBox);
    d->_searchEdit->setPlaceholderText(placeholderText);
}
