#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAINTVALIDATOR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAINTVALIDATOR_H_

#include <QIntValidator>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"
class ElaIntValidator : public QIntValidator
{
    Q_OBJECT
    ELA_VARIABLE(bool, IsHexMode)
public:
    explicit ElaIntValidator(QObject* parent = nullptr);
    explicit ElaIntValidator(int bottom, int top, QObject* parent = nullptr);
    ~ElaIntValidator() override;

    QValidator::State validate(QString& input, int& pos) const override;
    void fixup(QString& input) const override;

private:
    QString _completeInput(QString input, int length) const;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAINTVALIDATOR_H_

