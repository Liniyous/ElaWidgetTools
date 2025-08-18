#ifndef ELACUSTOMWIDGET_H
#define ELACUSTOMWIDGET_H

#include <QDialog>

#include "ElaAppBar.h"
#include "ElaDef.h"

class QVBoxLayout;
class ElaCustomWidget : public QDialog
{
    Q_OBJECT
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit ElaCustomWidget(QWidget* parent = nullptr);
    ~ElaCustomWidget() override;

    void setCentralWidget(QWidget* widget);

Q_SIGNALS:
    Q_SIGNAL void customWidgetClosed();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    QVBoxLayout* _mainLayout{nullptr};
    ElaAppBar* _appBar{nullptr};
    QWidget* _centralWidget{nullptr};

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaApplicationType::WindowDisplayMode _windowDisplayMode;
};

#endif // ELACUSTOMWIDGET_H
