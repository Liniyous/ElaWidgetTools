#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELASINGLETON_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELASINGLETON_H_

#define ELA_SINGLETON(Class)                \
public:                                     \
    static Class* getInstance()             \
    {                                       \
        static auto instance = new Class(); \
        return instance;                    \
    }

#define ELA_SINGLETON_H(Class) \
public:                        \
    static Class* getInstance();

#define ELA_SINGLETON_CPP(Class)            \
    Class* Class::getInstance()             \
    {                                       \
        static auto instance = new Class(); \
        return instance;                    \
    }

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELASINGLETON_H_
