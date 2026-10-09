#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROPERTY_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROPERTY_H_

#include <QSharedPointer>
#include <QtCore/qglobal.h>

#define ELA_PROPERTY(TYPE, M)                               \
    Q_PROPERTY(TYPE p##M MEMBER _p##M NOTIFY p##M##Changed) \
public:                                                     \
    Q_SIGNAL void p##M##Changed();                          \
    void set##M(TYPE const& M)                              \
    {                                                       \
        _p##M = M;                                          \
        Q_EMIT p##M##Changed();                             \
    }                                                       \
    Q_REQUIRED_RESULT TYPE const& get##M() const            \
    {                                                       \
        return _p##M;                                       \
    }                                                       \
                                                            \
private:                                                    \
    TYPE _p##M;

#define ELA_PROPERTY_Q_H(TYPE, M)                                       \
    Q_PROPERTY(TYPE p##M READ get##M WRITE set##M NOTIFY p##M##Changed) \
public:                                                                 \
    Q_SIGNAL void p##M##Changed();                                      \
    void set##M(TYPE const& M);                                         \
    Q_REQUIRED_RESULT TYPE const& get##M() const;

#define ELA_PROPERTY_Q_CPP(CLASS, TYPE, M) \
    void CLASS::set##M(TYPE const& M)      \
    {                                      \
        Q_D(CLASS);                        \
        d->_p##M = M;                      \
        Q_EMIT p##M##Changed();            \
    }                                      \
    TYPE const& CLASS::get##M() const      \
    {                                      \
        Q_D(const CLASS);                  \
        return d->_p##M;                   \
    }

#define ELA_PROPERTY_D(TYPE, M) \
private:                        \
    TYPE _p##M;

#define ELA_VARIABLE(TYPE, M)                    \
public:                                          \
    void set##M(TYPE const& M)                   \
    {                                            \
        _p##M = M;                               \
    }                                            \
    Q_REQUIRED_RESULT TYPE const& get##M() const \
    {                                            \
        return _p##M;                            \
    }                                            \
                                                 \
private:                                         \
    TYPE _p##M;

#define ELA_VARIABLE_Q_H(TYPE, M) \
public:                           \
    void set##M(TYPE const& M);   \
    Q_REQUIRED_RESULT TYPE const& get##M() const;

#define ELA_VARIABLE_Q_CPP(CLASS, TYPE, M) \
    void CLASS::set##M(TYPE const& M)      \
    {                                      \
        Q_D(CLASS);                        \
        d->_p##M = M;                      \
    }                                      \
    TYPE const& CLASS::get##M() const      \
    {                                      \
        Q_D(const CLASS);                  \
        return d->_p##M;                   \
    }

#define ELA_VARIABLE_D(TYPE, M) \
private:                        \
    TYPE _p##M;

#define ELA_PIMPL_Q(CLASS)                                         \
protected:                                                         \
    QSharedPointer<CLASS##Private> d_ptr;                          \
                                                                   \
private:                                                           \
    Q_DISABLE_COPY(CLASS)                                          \
    Q_DECLARE_PRIVATE(CLASS);                                      \
    void _set##CLASS##Private(QSharedPointer<CLASS##Private> ptr); \
    Q_REQUIRED_RESULT QSharedPointer<CLASS##Private> _get##CLASS##Private() const;

#define ELA_PIMPL_Q_CPP(CLASS)                                           \
    void CLASS::_set##CLASS##Private(QSharedPointer<CLASS##Private> ptr) \
    {                                                                    \
        d_ptr = std::move(ptr);                                          \
        d_ptr->q_ptr = this;                                             \
    }                                                                    \
    QSharedPointer<CLASS##Private> CLASS::_get##CLASS##Private() const   \
    {                                                                    \
        return d_ptr;                                                    \
    }

#define ELA_PIMPL_D(CLASS) \
protected:                 \
    CLASS* q_ptr;          \
                           \
private:                   \
    Q_DECLARE_PUBLIC(CLASS);

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROPERTY_H_
