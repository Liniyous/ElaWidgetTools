#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAMESSAGEBAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAMESSAGEBAR_H_

#include <QWidget>

#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaMessageBarPrivate;
class ELA_EXPORT ElaMessageBar : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaMessageBar)

public:
    static void success(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);
    static void warning(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);
    static void information(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);
    static void error(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    explicit ElaMessageBar(ElaMessageBarType::PositionPolicy policy, ElaMessageBarType::MessageMode messageMode, QString& title, QString& text, int displayMsec, QWidget* parent = nullptr);
    ~ElaMessageBar() override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAMESSAGEBAR_H_
