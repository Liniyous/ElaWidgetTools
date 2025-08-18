#ifndef ELAINTVALIDATOR_H
#define ELAINTVALIDATOR_H

#include <QIntValidator>

#include "ElaProperty.h"
class ElaIntValidator : public QIntValidator
{
    Q_OBJECT
    Q_PRIVATE_CREATE(bool, IsHexMode)
public:
    explicit ElaIntValidator(QObject* parent = nullptr);
    explicit ElaIntValidator(int bottom, int top, QObject* parent = nullptr);
    ~ElaIntValidator();

    QValidator::State validate(QString& input, int& pos) const override;
    void fixup(QString& input) const override;

private:
    QString _completeInput(QString input, int length) const;
};

#endif // ELAINTVALIDATOR_H
