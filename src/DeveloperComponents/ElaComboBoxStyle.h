#ifndef ELACOMBOBOXSTYLE_H
#define ELACOMBOBOXSTYLE_H

#include <QProxyStyle>

class ElaComboBoxStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaComboBoxStyle(QStyle* style = nullptr);
    ~ElaComboBoxStyle();
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
};

#endif // ELACOMBOBOXSTYLE_H
