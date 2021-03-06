#include "types/Number.hpp"
#include "types/Time.hpp"
#include "Error.hpp"
#include "Function.hpp"
#include <sstream>
#include <cmath>

namespace slim
{
    std::string Number::to_string()const
    {
        std::stringstream ss;
        ss << v;
        return ss.str();
    }

    const MethodTable &Number::method_table()const
    {
        static const MethodTable table(Object::method_table(),
        {
            { &Number::to_f, "to_f" },
            { &Number::to_f, "to_d" },
            { &Number::to_i, "to_i" },
            { &Number::abs, "abs" },
            { &Number::next_float, "next_float" },
            { &Number::prev_float, "prev_float" },
            { &Number::ceil, "ceil" },
            { &Number::floor, "floor" },
            { &Number::finite_q, "finite?" },
            { &Number::infinite_q, "infinite?" },
            { &Number::round, "round" },
            { &Number::nan_q, "nan?" },
            { &Number::zero_q, "zero?" },

            //alias
            { &Number::to_i, "truncate" },
            { &Number::to_i, "to_int" },
            { &Number::abs, "magnitude" },

            // Time extensions
            Method::free(&number_ago, "ago"), Method::free(&number_ago, "until"),
            Method::free(&number_from_now, "from_now"), Method::free(&number_from_now, "since"),
            Method::free(&number_fortnights, "fortnight"), Method::free(&number_fortnights, "fortnights"),
            Method::free(&number_weeks, "week"), Method::free(&number_weeks, "weeks"),
            Method::free(&number_days, "day"), Method::free(&number_days, "days"),
            Method::free(&number_hours, "hour"), Method::free(&number_hours, "hours"),
            Method::free(&number_minutes, "minute"), Method::free(&number_minutes, "minutes"),
            Method::free(&number_seconds, "second"), Method::free(&number_seconds, "seconds"),
        });
        return table;
    }

    ObjectPtr Number::mul(Object *rhs)
    {
        return make_value(v * coerce<Number>(rhs)->get_value());
    }
    ObjectPtr Number::div(Object *rhs)
    {
        return make_value(v / coerce<Number>(rhs)->get_value());
    }
    ObjectPtr Number::mod(Object *rhs)
    {
        return make_value(std::fmod(v, coerce<Number>(rhs)->get_value()));
    }
    ObjectPtr Number::pow(Object *rhs)
    {
        return make_value(std::pow(v, coerce<Number>(rhs)->get_value()));
    }
    ObjectPtr Number::add(Object *rhs)
    {
        return make_value(v + coerce<Number>(rhs)->get_value());
    }
    ObjectPtr Number::sub(Object *rhs)
    {
        return make_value(v - coerce<Number>(rhs)->get_value());
    }
    ObjectPtr Number::negate()
    {
        return make_value(-v);
    }

    ObjectPtr Number::bit_lshift(Object * rhs)
    {
        return make_value((int)v << (int)coerce<Number>(rhs)->get_value());
    }
    ObjectPtr Number::bit_rshift(Object * rhs)
    {
        return make_value((int)v >> (int)coerce<Number>(rhs)->get_value());
    }
    ObjectPtr Number::bit_and(Object * rhs)
    {
        return make_value((int)v & (int)coerce<Number>(rhs)->get_value());
    }
    ObjectPtr Number::bit_or(Object * rhs)
    {
        return make_value((int)v | (int)coerce<Number>(rhs)->get_value());
    }
    ObjectPtr Number::bit_xor(Object * rhs)
    {
        return make_value((int)v ^ (int)coerce<Number>(rhs)->get_value());
    }
    ObjectPtr Number::bit_not()
    {
        return make_value(~(int)v);
    }

    std::shared_ptr<Number> Number::to_f()
    {
        return std::static_pointer_cast<Number>(shared_from_this());
    }
    std::shared_ptr<Number> Number::to_i()
    {
        return make_value(std::trunc(v));
    }

    std::shared_ptr<Number> Number::abs()
    {
        return make_value(std::abs(v));
    }

    std::shared_ptr<Number> Number::next_float()
    {
        auto v2 = std::nextafter(v, INFINITY);
        return make_value(v2);
    }

    std::shared_ptr<Number> Number::prev_float()
    {
        auto v2 = std::nextafter(v, -INFINITY);
        return make_value(v2);
    }

    std::shared_ptr<Number> Number::ceil()
    {
        return make_value(std::ceil(v));
    }

    std::shared_ptr<Number> Number::floor()
    {
        return make_value(std::floor(v));
    }


    inline double round_f(double v, double ndigits)
    {
        auto factor = std::pow(10.0, ndigits - std::ceil(std::log10(std::fabs(v))));
        return std::round(v * factor) / factor;
    }
    std::shared_ptr<Number> Number::round(const FunctionArgs & args)
    {
        double ndigits = 0;
        if (args.size() == 1) ndigits = as_number(args[0]);
        else if (args.size() > 1) throw ArgumentError(this, "round");

        if (v == 0) return std::static_pointer_cast<Number>(shared_from_this());
        if (ndigits == 0) return make_value(std::round(v));
        else if (ndigits > 0) return make_value(round_f(v, ndigits));
        else return make_value(std::round(round_f(v, -ndigits)));
    }

    Ptr<Boolean> Number::finite_q()
    {
        return make_value(std::isfinite(v));
    }
    ObjectPtr Number::infinite_q()
    {
        if (std::isinf(v))
        {
            return make_value(v < 0 ? -1 : 1);
        }
        else return NIL_VALUE;
    }
    Ptr<Boolean> Number::nan_q()
    {
        return make_value(std::isnan(v));
    }

    Ptr<Boolean> Number::zero_q()
    {
        return make_value(v == 0);
    }


    namespace
    {
        static const unsigned char CACHE_MAX = 100;
        struct CachedNumbers
        {
            std::shared_ptr<Number> numbers[CACHE_MAX + 1];
            CachedNumbers()
            {
                for (int i = 0; i <= CACHE_MAX; ++i)
                    numbers[i] = create_object<Number>(i);
            }
        };
        std::shared_ptr<Number> *cached_numbers()
        {
            static CachedNumbers cache;
            return cache.numbers;
        }
    }
    std::shared_ptr<Number> make_value(int v)
    {
        if (v >= 0 && v <= CACHE_MAX) return cached_numbers()[v];
        else return create_object<Number>(v);
    }
    std::shared_ptr<Number> make_value(unsigned v)
    {
        if (v >= 0 && v <= CACHE_MAX) return cached_numbers()[v];
        else return create_object<Number>(v);
    }
    std::shared_ptr<Number> make_value(long long v)
    {
        if (v >= 0 && v <= CACHE_MAX) return cached_numbers()[v];
        else return create_object<Number>((double)v);
    }
    std::shared_ptr<Number> make_value(unsigned long long v)
    {
        if (v >= 0 && v <= CACHE_MAX) return cached_numbers()[v];
        else return create_object<Number>((double)v);
    }
}

