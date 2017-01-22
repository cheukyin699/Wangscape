#pragma once
#include <type_traits>
#include <stdexcept>
#include <static_if.hpp>

namespace
{

template<typename I>
I ipow_imp(I base, I exp)
{
    constexpr I zero(0);
    constexpr I two(2);
    I result(1);
    while(exp != zero)
    {
        if (exp % two)
            result *= base;
        exp /= two;
        base *= base;
    }
    return result;
}

}

// Calculates base raised to the power of exp,
// where base and exp share an integral type.
//
// Requires:
// is_integral<I>
// is_arithmetic<I>
// numeric_limits<I>
// operators(I, I): /=, *=, !=, ==, <, >=, %, <<
//
// Returns a correct result for all argument pairs
// that have an integer result representable in I.
// This includes:
// ipow(x, 0) == 0
// ipow(0, x) == 0 when x >= 0
// ipow(1, x) == 1
// ipow(-1, x) == +-1
// ipow(-2, digits) == min when digits is odd
// Some obvious failure cases raise exceptions:
// ipow(x, y) when |x| != 1 and y < 0
// ipow(x, y) when x >= 2 and y >= digits
// ipow(-2, y) when y > digits
// ipow(x, y) when x < 2 and y >= digits
// However, all other behaviour when the result
// is not an integer or is not representable in I
// is undefined.
// In particular, cases like ipow(digits-1, digits-1)
// are not checked, and may return nonsense results.
template <typename I>
I ipow(I base, I exp)
{
    static_assert(std::is_integral<I>::value,
                  "I must be an integral type");
    static_assert(std::is_arithmetic<I>::value,
                  "I must be an arithmetic type");
    if (exp == 0)
        return 1;
    if (base == 1)
        return 1;

    bool stop = false;
    I result;

    cpp::static_if<std::is_signed<I>::value>([&](auto)
    {
        if (base == -1)
        {
            stop = true;
            result = exp % 2 ? -1 : 1;
        }
    });
    if (stop)
        return result;

    cpp::static_if<std::is_signed<I>::value>([&](auto)
    {
        if(exp < 0)
            throw std::domain_error("Integer pow() with exp < 0 and |base| != 1");
    });

    if (base == 0)
        return 0;

    cpp::static_if<std::is_signed<I>::value>([&](auto)
    {
        if (exp > std::numeric_limits<I>::digits)
            throw std::range_error("Integer pow() with exp > digits and |base| > 1");
        if (base == -2)
        {
            stop = true;
            result = ((I)1 << exp) * (exp % 2 ? -1 : 1);
        }
    });
    if (stop)
        return result;

    if (exp >= std::numeric_limits<I>::digits)
        throw std::range_error("Integer pow() with exp >= digits and base not in {-2, -1, 0, 1}");
    if (base == 2)
        return (I)1 << exp;

    return ipow_imp(base, exp);
}
