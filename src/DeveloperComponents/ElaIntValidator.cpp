#include "ElaIntValidator.h"

ElaIntValidator::ElaIntValidator(QObject* parent)
    : QIntValidator{parent}
{
    _pIsHexMode = false;
}

ElaIntValidator::ElaIntValidator(int bottom, int top, QObject* parent)
    : QIntValidator{bottom, top, parent}
{
    _pIsHexMode = false;
}

ElaIntValidator::~ElaIntValidator()
{
}

QValidator::State ElaIntValidator::validate(QString& input, int& pos) const
{
    QString inputCopy = input;
    if (_pIsHexMode)
    {
        inputCopy.remove("#");
        if (!inputCopy.isEmpty())
        {
            bool isInt = false;
            int value = inputCopy.toInt(&isInt, 16);
            if (!isInt)
            {
                return QValidator::Invalid;
            }
            if (value < bottom() || value > top())
            {
                return QValidator::Invalid;
            }
            int topLength = QString::number(top(), 16).length();
            if (inputCopy.length() > topLength)
            {
                return QValidator::Invalid;
            }
        }
        inputCopy.prepend("#");
    }
    else
    {
        if (input.isEmpty())
        {
            return QValidator::Intermediate;
        }
        bool isInt = false;
        int value = inputCopy.toInt(&isInt);
        if (!isInt)
        {
            return QValidator::Invalid;
        }
        if (value < bottom() || value > top())
        {
            return QValidator::Invalid;
        }
    }
    input = inputCopy;
    return QValidator::Acceptable;
}

void ElaIntValidator::fixup(QString& input) const
{
    if (_pIsHexMode)
    {
        QString inputComplete = _completeInput(input, QString::number(top(), 16).length());
        input = QString("#") + inputComplete;
    }
    else
    {
        input = QString::number(bottom());
    }
}

QString ElaIntValidator::_completeInput(QString input, int length) const
{
    while (input.length() < length)
    {
        input.prepend("0");
    }
    return input;
}
