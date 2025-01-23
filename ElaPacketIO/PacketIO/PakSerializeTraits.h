#ifndef PAKSERIALIZETRAITS_H
#define PAKSERIALIZETRAITS_H
/* This file is used to categorize, variable types.  Currently, basic types
 * such as int, float, etc are separated from other types.
 */

namespace PakSerialization
{

struct DefaultTypeTrait {
};
struct BasicTypeTrait {
};

template <class T>
struct SerializeTraits {
    typedef DefaultTypeTrait serialize_trait_type;
    typedef T value_type;
};

#define DEFINE_TRAIT(TYPE_NAME, TRAIT_NAME)      \
    template <>                                  \
    struct SerializeTraits<TYPE_NAME> {          \
        typedef TRAIT_NAME serialize_trait_type; \
    };

DEFINE_TRAIT(signed short, BasicTypeTrait)
DEFINE_TRAIT(unsigned short, BasicTypeTrait)
DEFINE_TRAIT(char, BasicTypeTrait)
DEFINE_TRAIT(signed char, BasicTypeTrait)
DEFINE_TRAIT(unsigned char, BasicTypeTrait)
DEFINE_TRAIT(int, BasicTypeTrait)
DEFINE_TRAIT(long, BasicTypeTrait)
DEFINE_TRAIT(unsigned int, BasicTypeTrait)
DEFINE_TRAIT(unsigned long, BasicTypeTrait)
DEFINE_TRAIT(double, BasicTypeTrait)
DEFINE_TRAIT(float, BasicTypeTrait)
DEFINE_TRAIT(unsigned long long, BasicTypeTrait)

DEFINE_TRAIT(const signed short, BasicTypeTrait)
DEFINE_TRAIT(const unsigned short, BasicTypeTrait)
DEFINE_TRAIT(const char, BasicTypeTrait)
DEFINE_TRAIT(const signed char, BasicTypeTrait)
DEFINE_TRAIT(const unsigned char, BasicTypeTrait)
DEFINE_TRAIT(const int, BasicTypeTrait)
DEFINE_TRAIT(const long, BasicTypeTrait)
DEFINE_TRAIT(const unsigned int, BasicTypeTrait)
DEFINE_TRAIT(const unsigned long, BasicTypeTrait)
DEFINE_TRAIT(const double, BasicTypeTrait)
DEFINE_TRAIT(const float, BasicTypeTrait)
DEFINE_TRAIT(const unsigned long long, BasicTypeTrait)
#undef DEFINE_TRAIT

template <typename T>
struct non_const_type {
    typedef T type;
};

template <typename T>
struct non_const_type<const T> {
    typedef T type;
};

} // namespace PakSerialization

#endif
