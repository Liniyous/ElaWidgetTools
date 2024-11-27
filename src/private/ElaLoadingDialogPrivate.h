#ifndef ELALOADINGDIALOGPRIVATE_H
#define ELALOADINGDIALOGPRIVATE_H

#include <QObject>
#include <QTimer>

#include "Def.h"
#include "stdafx.h"
#include "ElaText.h"
#include "ElaProgressRing.h"

class QVBoxLayout;
class ElaProgressRing;
class ElaLoadingDialog;
class ElaMaskWidget;
class ElaText;
class ElaLoadingDialogPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaLoadingDialog)
public:
    explicit ElaLoadingDialogPrivate(QObject* parent = nullptr);
    ~ElaLoadingDialogPrivate();

private:
    qint64 _currentWinID{0};
    ElaThemeType::ThemeMode _themeMode;
    ElaMaskWidget* _maskWidget{nullptr};
    QWidget* _centralWidget{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    ElaProgressRing *_progressRing;
    ElaText* _mainTitle;
    ElaText* _subTitle;
    int _ms = {5000};
    QTimer* _outTimer{nullptr};

    void _doCloseAnimation();
};

#endif // ELALOADINGDIALOGPRIVATE_H
