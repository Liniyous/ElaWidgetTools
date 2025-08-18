#ifndef ELACOLORDIALOG_H
#define ELACOLORDIALOG_H

#include <QDialog>

#include "ElaAppBar.h"
#include "ElaProperty.h"
class ElaColorDialogPrivate;
class ELA_EXPORT ElaColorDialog : public QDialog
{
    Q_OBJECT
    Q_Q_CREATE(ElaColorDialog)
    Q_PROPERTY_CREATE_Q_H(QColor, CurrentColor)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit ElaColorDialog(QWidget* parent = nullptr);
    ~ElaColorDialog() override;

    QList<QColor> getCustomColorList() const;
    QColor getCustomColor(int index) const;
    QString getCurrentColorRGB() const;
Q_SIGNALS:
    Q_SIGNAL void colorSelected(const QColor& color);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELACOLORDIALOG_H
