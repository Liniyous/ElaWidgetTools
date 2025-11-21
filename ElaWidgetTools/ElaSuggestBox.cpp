#include "ElaSuggestBox.h"

#include <QAction>
#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QMap>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>

#include "ElaBaseListView.h"
#include "ElaIcon.h"
#include "ElaLineEdit.h"
#include "ElaScrollBar.h"
#include "ElaSuggestBoxSearchViewContainer.h"
#include "ElaSuggestDelegate.h"
#include "ElaSuggestModel.h"
#include "ElaTheme.h"
#include "private/ElaSuggestBoxPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaSuggestBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaSuggestBox, Qt::CaseSensitivity, CaseSensitivity)
ElaSuggestBox::SuggestData::SuggestData()
{
}

ElaSuggestBox::SuggestData::SuggestData(ElaIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData)
    : _pElaIcon(icon), _pSuggestText(suggestText), _pSuggestData(suggestData)
{
}

ElaSuggestBox::SuggestData::~SuggestData()
{
}

ElaSuggestBox::ElaSuggestBox(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaSuggestBoxPrivate())
{
    Q_D(ElaSuggestBox);
    QWidget::setFixedSize(280, 35);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    d->_pCaseSensitivity = Qt::CaseInsensitive;
    d->_searchEdit = new ElaLineEdit(this);
    d->_searchEdit->setFixedHeight(35);
    d->_searchEdit->setPlaceholderText("查找功能");
    d->_searchEdit->setClearButtonEnabled(true);
    d->_lightSearchAction = new QAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::MagnifyingGlass), "Search", this);
    d->_darkSearchAction = new QAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::MagnifyingGlass, QColor(0xFF, 0xFF, 0xFF)), "Search", this);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaSuggestBoxPrivate::onThemeModeChanged);
    if (d->_themeMode == ElaThemeType::Light)
    {
        d->_searchEdit->addAction(d->_lightSearchAction, QLineEdit::TrailingPosition);
    }
    else
    {
        d->_searchEdit->addAction(d->_darkSearchAction, QLineEdit::TrailingPosition);
    }

    connect(d->_lightSearchAction, &QAction::triggered, this, [=](bool checked) {
        //qDebug() << "Search";
    });

    connect(d->_darkSearchAction, &QAction::triggered, this, [=](bool checked) {
        //qDebug() << "Search";
    });
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(d->_searchEdit);
    d->_searchViewBaseWidget = new ElaSuggestBoxSearchViewContainer(window());
    d->_shadowLayout = new QVBoxLayout(d->_searchViewBaseWidget);
    d->_shadowLayout->setContentsMargins(0, 0, 0, 0);
    d->_searchView = new ElaBaseListView(d->_searchViewBaseWidget);
    ElaScrollBar* floatVScrollBar = new ElaScrollBar(d->_searchView->verticalScrollBar(), d->_searchView);
    floatVScrollBar->setIsAnimation(true);
    d->_searchViewBaseWidget->resize(292, 300);
    d->_shadowLayout->addWidget(d->_searchView);
    d->_searchModel = new ElaSuggestModel(this);
    d->_searchDelegate = new ElaSuggestDelegate(this);
    d->_searchView->setModel(d->_searchModel);
    d->_searchView->setItemDelegate(d->_searchDelegate);
    d->_searchViewBaseWidget->hide();
    connect(d->_searchEdit, &ElaLineEdit::textEdited, d, &ElaSuggestBoxPrivate::onSearchEditTextEdit);
    connect(d->_searchEdit, &ElaLineEdit::focusIn, d, &ElaSuggestBoxPrivate::onSearchEditTextEdit);
    connect(d->_searchView, &ElaBaseListView::clicked, d, &ElaSuggestBoxPrivate::onSearchViewClicked);

    // 焦点事件
    connect(d->_searchEdit, &ElaLineEdit::wmFocusOut, this, [=]() {
        if (!d->_searchView->underMouse())
        {
            d->_startCloseAnimation();
        }
    });
}

ElaSuggestBox::~ElaSuggestBox()
{
}

void ElaSuggestBox::setPlaceholderText(const QString& placeholderText)
{
    Q_D(ElaSuggestBox);
    d->_searchEdit->setPlaceholderText(placeholderText);
}

void ElaSuggestBox::setFixedSize(const QSize& size)
{
    Q_D(ElaSuggestBox);
    d->_searchEdit->setFixedHeight(size.height());
    QWidget::setFixedSize(size);
}
void ElaSuggestBox::setFixedSize(int w, int h)
{
    Q_D(ElaSuggestBox);
    d->_searchEdit->setFixedHeight(h);
    QWidget::setFixedSize(w, h);
}
void ElaSuggestBox::setFixedHeight(int h)
{
    Q_D(ElaSuggestBox);
    d->_searchEdit->setFixedHeight(h);
    QWidget::setFixedHeight(h);
}

QString ElaSuggestBox::addSuggestion(const QString& suggestText, const QVariantMap& suggestData)
{
    Q_D(ElaSuggestBox);
    ElaSuggestion* suggest = new ElaSuggestion(this);
    suggest->setSuggestText(suggestText);
    suggest->setSuggestData(suggestData);
    d->_suggestionVector.append(suggest);
    return suggest->getSuggestKey();
}

QString ElaSuggestBox::addSuggestion(ElaIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData)
{
    Q_D(ElaSuggestBox);
    ElaSuggestion* suggest = new ElaSuggestion(this);
    suggest->setElaIcon(icon);
    suggest->setSuggestText(suggestText);
    suggest->setSuggestData(suggestData);
    d->_suggestionVector.append(suggest);
    return suggest->getSuggestKey();
}

QString ElaSuggestBox::addSuggestion(const ElaSuggestBox::SuggestData& suggestData)
{
    Q_D(ElaSuggestBox);
    ElaSuggestion* suggest = new ElaSuggestion(this);
    suggest->setElaIcon(suggestData.getElaIcon());
    suggest->setSuggestText(suggestData.getSuggestText());
    suggest->setSuggestData(suggestData.getSuggestData());
    d->_suggestionVector.append(suggest);
    return suggest->getSuggestKey();
}

QStringList ElaSuggestBox::addSuggestion(const QList<ElaSuggestBox::SuggestData>& suggestDataList)
{
    Q_D(ElaSuggestBox);
    QStringList suggestKeyList;
    for (const auto& suggestData: suggestDataList)
    {
        ElaSuggestion* suggest = new ElaSuggestion(this);
        suggest->setElaIcon(suggestData.getElaIcon());
        suggest->setSuggestText(suggestData.getSuggestText());
        suggest->setSuggestData(suggestData.getSuggestData());
        d->_suggestionVector.append(suggest);
        suggestKeyList.append(suggest->getSuggestKey());
    }
    return suggestKeyList;
}

void ElaSuggestBox::removeSuggestion(const QString& suggestKey)
{
    Q_D(ElaSuggestBox);
    foreach (auto suggest, d->_suggestionVector)
    {
        if (suggest->getSuggestKey() == suggestKey)
        {
            d->_suggestionVector.removeOne(suggest);
            suggest->deleteLater();
        }
    }
}

void ElaSuggestBox::removeSuggestion(int index)
{
    Q_D(ElaSuggestBox);
    if (index >= d->_suggestionVector.count())
    {
        return;
    }
    ElaSuggestion* suggest = d->_suggestionVector[index];
    d->_suggestionVector.removeOne(suggest);
    suggest->deleteLater();
}

void ElaSuggestBox::clearSuggestion()
{
    Q_D(ElaSuggestBox);
    foreach (auto suggest, d->_suggestionVector)
    {
        d->_suggestionVector.removeOne(suggest);
        suggest->deleteLater();
    }
}
