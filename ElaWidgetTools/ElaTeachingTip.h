#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATEACHINGTIP_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATEACHINGTIP_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"

#include <QWidget>

class ElaTeachingTipPrivate;
class ELA_EXPORT ElaTeachingTip : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaTeachingTip)
public:
    explicit ElaTeachingTip(QWidget* parent = nullptr);
    ~ElaTeachingTip() override;

    void setTargetWidget(QWidget* targetWidget);
    QWidget* getTargetWidget() const;
    void setTitle(const QString& title);
    QString getTitle() const;
    void setSubTitle(const QString& subTitle);
    QString getSubTitle() const;
    void setElaIcon(ElaIconType::IconName icon);
    ElaIconType::IconName getElaIcon() const;

    // 接管自定义内容的所有权 替换时延迟释放旧内容 nullptr 清空内容
    void setCustomWidget(QWidget* customWidget);
    QWidget* getCustomWidget() const;
    void setActionButtonText(const QString& text);
    QString getActionButtonText() const;
    void setCloseButtonText(const QString& text);
    QString getCloseButtonText() const;

    void setPreferredPlacement(ElaTeachingTipType::PlacementMode placement);
    ElaTeachingTipType::PlacementMode getPreferredPlacement() const;
    ElaTeachingTipType::PlacementMode getEffectivePlacement() const;
    // 内容宽度 不包含阴影和尖角 单位为逻辑像素
    void setTipWidth(int width);
    int getTipWidth() const;
    void setIsTailVisible(bool isVisible);
    bool getIsTailVisible() const;
    void setIsLightDismissEnabled(bool isEnabled);
    bool getIsLightDismissEnabled() const;
    void setIsOpen(bool isOpen);
    bool getIsOpen() const;
    QSize sizeHint() const override;

Q_SIGNALS:
    Q_SIGNAL void pIsOpenChanged();
    Q_SIGNAL void pCustomWidgetChanged();
    Q_SIGNAL void opened();
    Q_SIGNAL void closed();
    Q_SIGNAL void actionButtonClicked();
    Q_SIGNAL void closeButtonClicked();

protected:
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELATEACHINGTIP_H_
