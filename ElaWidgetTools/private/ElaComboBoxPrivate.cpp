#include "ElaComboBoxPrivate.h"
#include "ElaComboBox.h"
#include <QLineEdit>
#include <QTimer>
ElaComboBoxPrivate::ElaComboBoxPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaComboBoxPrivate::~ElaComboBoxPrivate()
{
}

void ElaComboBoxPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaComboBox);
    _themeMode = themeMode;
    auto lineEdit = q->lineEdit();
    if (lineEdit)
    {
        if (q->isVisible())
        {
            _changeTheme(lineEdit);
        }
        else
        {
            QTimer::singleShot(1, this, [=] {
                _changeTheme(lineEdit);
            });
        }
    }
}

void ElaComboBoxPrivate::_changeTheme(QLineEdit* lineEdit)
{
    Q_Q(ElaComboBox);
    QPalette palette = lineEdit->palette();
    if (_themeMode == ElaThemeType::Light)
    {
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::PlaceholderText, QColor(0x00, 0x00, 0x00, 128));
    }
    else
    {
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::PlaceholderText, QColor(0xBA, 0xBA, 0xBA));
    }
    lineEdit->setPalette(palette);
}
