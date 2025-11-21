#ifndef ELASUGGESTBOX_H
#define ELASUGGESTBOX_H

#include <QVariantMap>
#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaSuggestBoxPrivate;
class ELA_EXPORT ElaSuggestBox : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaSuggestBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(Qt::CaseSensitivity, CaseSensitivity)
public:
    explicit ElaSuggestBox(QWidget* parent = nullptr);
    ~ElaSuggestBox() override;
    void setPlaceholderText(const QString& placeholderText);
    void setFixedSize(const QSize& size);
    void setFixedSize(int w, int h);
    void setFixedHeight(int h);

    struct ELA_EXPORT SuggestData {
        Q_PRIVATE_CREATE(ElaIconType::IconName, ElaIcon)
        Q_PRIVATE_CREATE(QString, SuggestText)
        Q_PRIVATE_CREATE(QString, SuggestKey)
        Q_PRIVATE_CREATE(QVariantMap, SuggestData)
    public:
        explicit SuggestData();
        explicit SuggestData(ElaIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {});
        ~SuggestData();
    };
    QString addSuggestion(const QString& suggestText, const QVariantMap& suggestData = {});
    QString addSuggestion(ElaIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {});
    QString addSuggestion(const ElaSuggestBox::SuggestData& suggestData);
    QStringList addSuggestion(const QList<ElaSuggestBox::SuggestData>& suggestDataList);

    void removeSuggestion(const QString& suggestKey);
    void removeSuggestion(int index);
    void clearSuggestion();
Q_SIGNALS:
    Q_SIGNAL void suggestionClicked(const ElaSuggestBox::SuggestData& suggestData);
};

#endif // ELASUGGESTBOX_H
