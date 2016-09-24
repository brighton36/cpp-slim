#include "types/Enumerator.hpp"
#include "types/Array.hpp"
#include "types/Hash.hpp"
#include "types/Proc.hpp"
#include "Function.hpp"
#include <sstream>

namespace slim
{
    const MethodTable & Enumerator::method_table() const
    {
        static const MethodTable table = MethodTable(Object::method_table())
            .add_all(Enumerable::get_methods<Enumerator>());
        return table;
    }

    ObjectPtr MethodEnumerator::each(const FunctionArgs &args2)
    {
        Proc *proc = nullptr;
        if (!args2.empty()) proc = dynamic_cast<Proc*>(args2.back().get());

        if (proc)
        {
            FunctionArgs all_args = args;
            all_args.insert(all_args.end(), args2.begin(), args2.end());
            return forward(forward_self.get(), all_args);
        }
        else
        {
            return make_enumerator(this, { &Enumerator::each, "each" }, args2);
        }
    }

    ObjectPtr FunctionEnumerator::each(const FunctionArgs &args2)
    {
        Proc *proc = nullptr;
        if (!args2.empty()) proc = dynamic_cast<Proc*>(args2.back().get());

        if (proc)
        {
            FunctionArgs all_args = args;
            all_args.insert(all_args.end(), args2.begin(), args2.end());
            return func(all_args);
        }
        else
        {
            return make_enumerator(this, { &Enumerator::each, "each" }, args2);
        }
    }
}
