#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAPIVOT_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAPIVOT_H_
#include <QStringList>
#include <QWidget>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"

class ElaPivotPrivate;
class ELA_EXPORT ElaPivot : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaPivot)
public:
    explicit ElaPivot(QWidget* parent = nullptr);
    ~ElaPivot() override;

    void setTextPixelSize(int textPixelSize);
    Q_REQUIRED_RESULT int getTextPixelSize() const;
    void setCurrentIndex(int currentIndex);
    Q_REQUIRED_RESULT int getCurrentIndex() const;
    void setPivotSpacing(int pivotSpacing);
    Q_REQUIRED_RESULT int getPivotSpacing() const;
    void setMarkWidth(int markWidth);
    Q_REQUIRED_RESULT int getMarkWidth() const;

    void appendPivot(const QString& pivotTitle);
    void removePivot(const QString& pivotTitle);
    void clearPivot();
    const QStringList& getPivotTitle() const;
    void setCurrentTitle(const QString& pivotTitle);

Q_SIGNALS:
    Q_SIGNAL void pTextPixelSizeChanged();
    Q_SIGNAL void pCurrentIndexChanged();
    Q_SIGNAL void pPivotSpacingChanged();
    Q_SIGNAL void pMarkWidthChanged();
    Q_SIGNAL void pivotClicked(int index);
    Q_SIGNAL void pivotDoubleClicked(int index);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAPIVOT_H_
