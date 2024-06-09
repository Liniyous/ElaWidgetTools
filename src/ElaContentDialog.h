#ifndef ELACONTENTDIALOG_H
#define ELACONTENTDIALOG_H
#include <QAbstractNativeEventFilter>
#include <QDialog>

class QHBoxLayout;
class QVBoxLayout;
class ElaPushButton;
class ElaContentDialog : public QDialog, QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit ElaContentDialog(QWidget* parent = nullptr);
    ~ElaContentDialog();
    Q_SLOT virtual void onLeftButtonClicked();
    Q_SLOT virtual void onMiddleButtonClicked();
    Q_SLOT virtual void onRightButtonClicked();
    void setCentralWidget(QWidget* centralWidget);
Q_SIGNALS:
    Q_SIGNAL void leftButtonClicked();
    Q_SIGNAL void middleButtonClicked();
    Q_SIGNAL void rightButtonClicked();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
#if (QT_VERSION == QT_VERSION_CHECK(6, 5, 3) || QT_VERSION == QT_VERSION_CHECK(6, 6, 0))
    bool eventFilter(QObject* obj, QEvent* event) override;
#endif
    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;

private:
    QWidget* _shadowWidget{nullptr};
    QWidget* _centralWidget{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QHBoxLayout* _buttonLayout{nullptr};
    ElaPushButton* _leftButton{nullptr};
    ElaPushButton* _middleButton{nullptr};
    ElaPushButton* _rightButton{nullptr};
};

#endif // ELACONTENTDIALOG_H
