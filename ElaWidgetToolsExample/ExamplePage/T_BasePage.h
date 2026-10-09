#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_BASEPAGE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_BASEPAGE_H_

#include <ElaScrollPage.h>

class QVBoxLayout;
class T_BasePage : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_BasePage(QWidget* parent = nullptr);
    ~T_BasePage() override;

protected:
    void createCustomWidget(QString desText);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_BASEPAGE_H_

