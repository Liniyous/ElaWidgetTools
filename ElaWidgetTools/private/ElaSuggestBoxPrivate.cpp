#include "ElaSuggestBoxPrivate.h"

#include "ElaBaseListView.h"
#include "ElaColorAnimation.h"
#include "ElaLineEdit.h"
#include "ElaSuggestBox.h"
#include "ElaSuggestBoxSearchViewContainer.h"
#include "ElaSuggestModel.h"
#include "ElaTheme.h"

#include <QApplication>
#include <QEvent>
#include <QRegion>
#include <QUuid>
#include <QVariantAnimation>

ElaSuggestion::ElaSuggestion(QObject* parent)
    : QObject(parent)
{
    _pElaIcon = ElaIconType::None;
    _pSuggestText = "";
    _pSuggestKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _pSuggestData = QVariantMap();
}

ElaSuggestion::~ElaSuggestion()
{
}

ElaSuggestBoxPrivate::ElaSuggestBoxPrivate(QObject* parent)
    : QObject{parent}
{
    _popupAnimation = new QVariantAnimation(this);
    connect(_popupAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
        if (_popupAnimation->state() == QAbstractAnimation::Running)
        {
            _applyPopupReveal(value.toReal());
        }
    });
}

ElaSuggestBoxPrivate::~ElaSuggestBoxPrivate()
{
}

void ElaSuggestBoxPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    _searchEdit->removeAction(_themeMode == ElaThemeType::Light ? _darkSearchAction : _lightSearchAction);
    _searchEdit->addAction(_themeMode == ElaThemeType::Light ? _lightSearchAction : _darkSearchAction, QLineEdit::TrailingPosition);
    _searchEdit->update();
}

void ElaSuggestBoxPrivate::onSearchEditTextEdit(const QString& searchText)
{
    Q_Q(ElaSuggestBox);
    if (!q->isVisible() || !_searchViewBaseWidget)
    {
        return;
    }
    if (searchText.isEmpty())
    {
        _animatePopupTo(0.0);
        return;
    }
    QVector<ElaSuggestion*> suggestionVector;
    for (const auto& suggest: _suggestionVector)
    {
        if (suggest->getSuggestText().contains(searchText, _pCaseSensitivity))
        {
            suggestionVector.append(suggest);
        }
    }
    if (suggestionVector.isEmpty())
    {
        _animatePopupTo(0.0);
        return;
    }

    QWidget* host = q->window();
    if (_searchViewBaseWidget->parentWidget() != host)
    {
        _popupAnimation->stop();
        _searchViewBaseWidget->setParent(host);
    }
    _searchModel->setSearchSuggestion(suggestionVector);
    const QRect available = host->rect();
    const QRect editRect(_searchEdit->mapTo(host, QPoint()), _searchEdit->size());
    const QMargins shadowMargins = eTheme->getShadowMargins();
    const QMargins contentMargins = _searchViewBaseWidget->contentsMargins() - shadowMargins;
    const int verticalPadding = contentMargins.top() + contentMargins.bottom();
    const int shownRowCount = qMin<int>(4, suggestionVector.size());
    const int desiredHeight = shownRowCount * _searchView->sizeHintForRow(0) + verticalPadding;
    const int spaceBelow = available.bottom() - editRect.bottom();
    const int spaceAbove = editRect.top() - available.top();
    _isPopupAbove = spaceBelow < desiredHeight && spaceAbove > spaceBelow;
    const int bodyHeight = qMin(desiredHeight, _isPopupAbove ? spaceAbove : spaceBelow);
    const int bodyWidth = qMin(editRect.width(), available.width());
    if (bodyHeight <= verticalPadding || bodyWidth <= contentMargins.left() + contentMargins.right())
    {
        _popupAnimation->stop();
        _applyPopupReveal(0.0);
        return;
    }
    const int bodyX = qBound(available.left(), editRect.left(), available.right() - bodyWidth + 1);
    const int bodyY = _isPopupAbove ? editRect.top() - bodyHeight : editRect.bottom() + 1;
    const QRect bodyRect(bodyX, bodyY, bodyWidth, bodyHeight);
    _searchViewBaseWidget->setGeometry(bodyRect.marginsAdded(shadowMargins));
    // 阴影留白可能覆盖输入框 将该区域从绘制和鼠标命中范围中排除
    const QRect editInPopup = editRect.translated(-_searchViewBaseWidget->pos());
    _searchViewBaseWidget->setMask(QRegion(_searchViewBaseWidget->rect()).subtracted(editInPopup));
    _searchView->resize(_searchViewBaseWidget->contentsRect().size());
    _applyPopupReveal(_searchViewBaseWidget->isVisible() ? _popupProgress : 0.5);
    _animatePopupTo(1.0);
    _searchViewBaseWidget->show();
    _searchViewBaseWidget->raise();
}

void ElaSuggestBoxPrivate::onSearchViewClicked(const QModelIndex& index)
{
    Q_Q(ElaSuggestBox);
    _searchView->clearSelection();
    if (!index.isValid())
    {
        return;
    }
    ElaSuggestion* suggest = _searchModel->getSearchSuggestion(index.row());
    _searchEdit->setText(suggest->getSuggestText());
    ElaSuggestBox::SuggestData data(suggest->getElaIcon(), suggest->getSuggestText(), suggest->getSuggestData());
    data.setSuggestKey(suggest->getSuggestKey());
    _animatePopupTo(0.0);
    Q_EMIT q->suggestionClicked(data);
}

bool ElaSuggestBoxPrivate::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == q_ptr && event->type() == QEvent::Hide)
    {
        _popupAnimation->stop();
        _applyPopupReveal(0.0);
    }
    return QObject::eventFilter(watched, event);
}

void ElaSuggestBoxPrivate::_animatePopupTo(qreal progress)
{
    Q_Q(ElaSuggestBox);
    if (!ElaColorAnimation::getIsAnimationEnabled(q) || !QApplication::isEffectEnabled(Qt::UI_AnimateCombo))
    {
        _popupAnimation->stop();
        _applyPopupReveal(progress);
        return;
    }
    if (_popupAnimation->state() == QAbstractAnimation::Running && _popupAnimation->endValue().toReal() == progress)
    {
        return;
    }
    _popupAnimation->stop();
    if (_popupProgress == progress)
    {
        return;
    }
    if (progress > _popupProgress)
    {
        QEasingCurve easing(QEasingCurve::BezierSpline);
        easing.addCubicBezierSegment(QPointF(0, 0), QPointF(0, 1), QPointF(1, 1));
        _popupAnimation->setEasingCurve(easing);
        _popupAnimation->setDuration(180);
    }
    else
    {
        _popupAnimation->setEasingCurve(QEasingCurve::OutCubic);
        _popupAnimation->setDuration(150);
    }
    _popupAnimation->setStartValue(_popupProgress);
    _popupAnimation->setEndValue(progress);
    _popupAnimation->start();
}

void ElaSuggestBoxPrivate::_applyPopupReveal(qreal progress)
{
    _popupProgress = progress;
    if (!_searchViewBaseWidget)
    {
        return;
    }
    if (progress <= 0.0)
    {
        _searchViewBaseWidget->hide();
        _searchModel->setSearchSuggestion({});
        return;
    }

    QRect bodyRect = _searchViewBaseWidget->rect().marginsRemoved(eTheme->getShadowMargins());
    const int visibleHeight = qMax(1, qRound(bodyRect.height() * progress));
    if (_isPopupAbove)
    {
        bodyRect.setTop(bodyRect.bottom() - visibleHeight + 1);
    }
    else
    {
        bodyRect.setHeight(visibleHeight);
    }
    _searchViewBaseWidget->setBodyRect(bodyRect);
    const QRect contentRect = _searchViewBaseWidget->contentsRect();
    const int offset = qRound(24.0 * (1.0 - progress)) * (_isPopupAbove ? 1 : -1);
    _searchView->move(contentRect.topLeft() + QPoint(0, offset));
    // 视图尺寸保持不变 仅裁剪显示区域 避免滚动条随动画反复显隐
    const QRect clipRect = bodyRect.intersected(contentRect).translated(-_searchView->pos()).intersected(_searchView->rect());
    _searchView->setMask(clipRect == _searchView->rect() ? QRegion() : QRegion(clipRect));
    _searchView->setVisible(!clipRect.isEmpty());
}
