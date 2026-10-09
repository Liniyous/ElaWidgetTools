#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACOLORDIALOG_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACOLORDIALOG_H_

#include <QDialog>

#include "ElaAppBar.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaColorDialogPrivate;
class ELA_EXPORT ElaColorDialog : public QDialog
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaColorDialog)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit ElaColorDialog(QWidget* parent = nullptr);
    ~ElaColorDialog() override;

    void setCurrentColor(const QColor& currentColor);
    Q_REQUIRED_RESULT const QColor& getCurrentColor() const;

    const QList<QColor>& getCustomColorList() const;
    QColor getCustomColor(int index) const;
    QString getCurrentColorRGB() const;
Q_SIGNALS:
    Q_SIGNAL void pCurrentColorChanged();
    Q_SIGNAL void colorSelected(const QColor& color);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELACOLORDIALOG_H_
