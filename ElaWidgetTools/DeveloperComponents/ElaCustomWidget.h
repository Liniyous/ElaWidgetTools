#ifndef ELACUSTOMWIDGET_H
#define ELACUSTOMWIDGET_H

#include <QDialog>

#include "Def.h"
#include "ElaAppBar.h"

class QVBoxLayout;
class ElaCustomWidget : public QDialog
{
    Q_OBJECT
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit ElaCustomWidget(QWidget* parent = nullptr);
    ~ElaCustomWidget();

    void setCentralWidget(QWidget* widget);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    QVBoxLayout* _mainLayout{nullptr};
    ElaAppBar* _appBar{nullptr};
    QWidget* _centralWidget{nullptr};

private:
    ElaThemeType::ThemeMode _themeMode;
    bool _isEnableMica;
};

#endif // ELACUSTOMWIDGET_H
