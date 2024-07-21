#include "ElaSuggestBoxPrivate.h"

#include <QPropertyAnimation>

#include "ElaLineEdit.h"
#include "ElaShadowWidget.h"
#include "ElaSuggestBox.h"
#include "ElaSuggestModel.h"
#include "ElaSuggestView.h"

ElaSuggestion::ElaSuggestion(QObject* parent)
    : QObject(parent)
{
    _pElaIcon = ElaIconType::None;
    _pSuggestText = "";
    _pSuggestData = QVariantMap();
}

ElaSuggestion::~ElaSuggestion()
{
}

ElaSuggestBoxPrivate::ElaSuggestBoxPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaSuggestBoxPrivate::~ElaSuggestBoxPrivate()
{
}

void ElaSuggestBoxPrivate::onSearchEditTextEdit(const QString& searchText)
{
    Q_Q(ElaSuggestBox);
    if (searchText.isEmpty())
    {
        _startCloseAnimation();
        return;
    }
    QVector<ElaSuggestion*> suggestionVector;
    for (const auto& suggest : _suggestionVector)
    {
        if (suggest->getSuggestText().contains(searchText, _pCaseSensitivity))
        {
            suggestionVector.append(suggest);
        }
    }
    if (!suggestionVector.isEmpty())
    {
        _searchModel->setSearchSuggestion(suggestionVector);
        int rowCount = suggestionVector.count();
        if (rowCount > 4)
        {
            rowCount = 4;
        }
        if (!_shadowWidget->isVisible())
        {
            q->raise();
            _shadowWidget->show();
            _shadowWidget->raise();
            QPoint cyclePoint = _shadowWidget->mapFromGlobal(q->mapToGlobal(QPoint(1, q->height())));
            if (cyclePoint != QPoint(0, 0))
            {
                _shadowWidget->move(cyclePoint);
            }
            _shadowWidget->resize(q->width(), 40 * rowCount + 16);
            _searchView->move(_searchView->x(), -_shadowWidget->height());
        }
        else
        {
            _startSizeAnimation(QSize(q->width(), 40 * rowCount + 16));
        }
        _startExpandAnimation();
    }
    else
    {
        _startCloseAnimation();
    }
}

void ElaSuggestBoxPrivate::onSearchViewClicked(const QModelIndex& index)
{
    Q_Q(ElaSuggestBox);
    _searchEdit->clear();
    _searchView->clearSelection();
    if (!index.isValid())
    {
        return;
    }
    ElaSuggestion* suggest = _searchModel->getSearchSuggestion(index.row());
    Q_EMIT q->suggestionClicked(suggest->getSuggestText(), suggest->getSuggestData());
    _startCloseAnimation();
}

void ElaSuggestBoxPrivate::_startSizeAnimation(QSize newSize)
{
    if (_lastSize == newSize)
    {
        return;
    }
    _lastSize = newSize;
    QPropertyAnimation* expandAnimation = new QPropertyAnimation(_shadowWidget, "size");
    expandAnimation->setDuration(300);
    expandAnimation->setEasingCurve(QEasingCurve::InOutSine);
    expandAnimation->setStartValue(_shadowWidget->size());
    expandAnimation->setEndValue(newSize);
    expandAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaSuggestBoxPrivate::_startExpandAnimation()
{
    if (!_isExpandAnimationFinished)
    {
        return;
    }
    _isCloseAnimationFinished = true;
    _isExpandAnimationFinished = false;
    QPropertyAnimation* expandAnimation = new QPropertyAnimation(_searchView, "pos");
    connect(expandAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isExpandAnimationFinished = true;
        _searchView->clearSelection();
    });
    expandAnimation->setDuration(300);
    expandAnimation->setEasingCurve(QEasingCurve::InOutSine);
    expandAnimation->setStartValue(_searchView->pos());
    expandAnimation->setEndValue(QPoint(0, 0));
    expandAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaSuggestBoxPrivate::_startCloseAnimation()
{
    if (!_isCloseAnimationFinished)
    {
        return;
    }
    _isExpandAnimationFinished = true;
    _isCloseAnimationFinished = false;
    QPropertyAnimation* closeAnimation = new QPropertyAnimation(_searchView, "pos");
    connect(closeAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isCloseAnimationFinished = true;
        _searchModel->clearSearchNode();
        _shadowWidget->hide();
    });
    closeAnimation->setDuration(300);
    closeAnimation->setEasingCurve(QEasingCurve::InOutSine);
    closeAnimation->setStartValue(_searchView->pos());
    closeAnimation->setEndValue(QPoint(_searchView->pos().x(), -_searchView->height()));
    closeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
