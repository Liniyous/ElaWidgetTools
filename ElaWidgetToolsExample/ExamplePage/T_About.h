#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ABOUT_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ABOUT_H_

#include <ElaDialog.h>

class T_About : public ElaDialog
{
    Q_OBJECT
public:
    explicit T_About(QWidget* parent = nullptr);
    ~T_About();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ABOUT_H_

