#ifndef ELASUGGESTBOX_H
#define ELASUGGESTBOX_H

#include <QVariantMap>
#include <QWidget>

#include "Def.h"
#include "stdafx.h"

class ElaSuggestBoxPrivate;
class ELA_EXPORT ElaSuggestBox : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaSuggestBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(Qt::CaseSensitivity, CaseSensitivity)
public:
    explicit ElaSuggestBox(QWidget* parent = nullptr);
    ~ElaSuggestBox();
    void setPlaceholderText(const QString& placeholderText);

    void addSuggestion(const QString& suggestText, const QVariantMap& suggestData = {});
    void addSuggestion(ElaIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {});
    void removeSuggestion(const QString& suggestText);
    void removeSuggestion(int index);
Q_SIGNALS:
    Q_SIGNAL void suggestionClicked(QString suggestText, QVariantMap suggestData);
};

#endif // ELASUGGESTBOX_H
