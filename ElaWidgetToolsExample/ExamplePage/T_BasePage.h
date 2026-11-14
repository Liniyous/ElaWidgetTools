#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_BASEPAGE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_BASEPAGE_H_

#include <ElaScrollPage.h>

class ElaImageCard;
class QVBoxLayout;
class T_BasePage : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_BasePage(QWidget* parent = nullptr);
    ~T_BasePage() override;

protected:
    void createCustomWidget(const QString& description, const QString& title = {});
    QVBoxLayout* createPageLayout();
    QVBoxLayout* addExampleSection(QVBoxLayout* pageLayout, const QString& title, const QString& description = {});
    QWidget* createLabeledControl(const QString& title, QWidget* control);
    QWidget* createSettingRow(const QString& title, const QString& description, QWidget* control);
    void setPreviewPlaceholder(ElaImageCard* previewCard, const QString& description);
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_BASEPAGE_H_
