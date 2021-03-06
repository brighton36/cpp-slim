#Types
The following built in types exist for use with scripts and templates.
These provide most of the script functionality, and use the same method names and arguments as Ruby where possible.

   * [Array](types/Array.md)
   * Boolean
   * [Enumerable](types/Enumerable.md)
   * Enumerator
   * Hash
   * HtmlSafeString
   * MatchData
   * [Math](types/Math.md)
   * Nil
   * Number
   * Object
   * Range
   * Regexp
   * [String](types/String.md)
   * Symbol
   * [ViewModel](types/ViewModel.md)

# Boolean
The `Boolean` class has two instances, `true` and `false` and performs a similar purpose to the Ruby `TrueClass` and `FalseClass`.

##Methods

### `to_i`
`true` returns `1` and `false` returns `0`.

### `to_f`, `to_d`
`true` returns `1.0` and `false` returns `0.0`.

### `to_s`, `inspect`
Returns `"true"` or `"false"`.

# Enumerator
`Enumerator` class. Includes `Enumerable` and provides some of the functionality of the Ruby
`Enumerator` but notably does not support being used as an iterator (`next`, `peek`, etc.).

#Hash
The `Hash` object stores an ordered set of key-value pairs. The order is the order in which each unique key was added to the array.

Scripts can create array hash using curly brackets, e.g. `{}` (empty), `{a: 5}`, `{a: 5, 5 => true}`.

Includes `Enumerable` and most methods are the same as the Ruby hash methods.

   * `==`, `!=`, `<=>`
   * `[key]`
   * `dup`
   * `each`
   * `each_key`
   * `each_value`
   * `empty?`
   * `fetch key`, `fetch key, default`: A block is not supported.
   * `flatten level=0`
   * `has_key? key`, `key? key`
   * `has_value? value`, `value? value`
   * `invert`
   * `key value`
   * `keys`
   * `length`, `size`
   * `merge other`: A block is not supported.
   * `to_a`
   * `to_h`
   * `values`

# HtmlSafeString
`HtmlSafeString` is a sub type of `String` that is not escaped when being added to the template output.

# MatchData
Match data for a `Regexp#match` result.

   * `==`
   * `[i]`, `[start, length]`: No named capature or range support.
   * `begin`
   * `captures`
   * `end`
   * `length`
   * `offset`
   * `post_match`
   * `pre_match`
   * `regexp`
   * `size`
   * `string`
   * `to_a`
   * `to_s`
   * `values_at`

# Object
The Object class is the base class for all script types. It is mostly of interest for creating new types (see API docs for C++ methods), but also includes some basic script methods.

Many of the basic operators (`[]`, `+`, etc.) are implemented as virtual C++ methods on Object, however the default implementations throw `NoSuchMethod`, so it is as if they do not exist to script code.

The `false` and `nil` object instances evaluate to `false` in a Boolean context, all other instances are `true`, include zero and empty strings.

## Methods
### `to_s`
Returns the value of the C++ `Object::to_string` virtual method. The default implementation calls `inspect`.

### `inspect`
Returns the value of the C++ `Object::inspect` virtual method. The default implementation returns the type name and heap memory address as a string.

### `hash`
Returns the default value of the C++ `Object::hash` virtual method. The default implementation returns a hash based on the objects memory address.

### `==`, `!=`
Implemented by `Object::eq`. The default implementation compares object identity.

### `<`, `<=`, `>`, `>=`, `<=>`
Implemented by `Object::cmp`. The default implementation throws `UnorderableTypeError`.

# Nil

The `nil` class.

## Methods
### `to_s`, `inspect`
Returns `"nil"`.

# Number

The `Number` class represents numeric values using a `double`. This is used for all numeric literals, and fulfills the role of the various numeric types (`Numeric`, `Integer`, `Bignum`, `Fixnum`, `Float`).

Includes some Rails time helpers. The duration methods such as `hours` return another number object with seconds.

Most methods are the same as the Ruby methods.

   * `to_f`, `to_d`: Returns the same number.
   * `to_i`: Returns a number truncated by `std::trunc(double)`.
   * `number * number`, `number / number`, `number + number`, `number - number`: double precision arithmetic.
   * `number % number`: Returns `std::fmod(double, double)`.
   * `number ** number`: Returns `std::pow(double, double)`.
   * `-number`
   * `number << number`, `number >> number`, `number & number`, `number | number`, `number ^ number`, `~number`: Bit operators which cast both numbers to `int`.
   * `abs`
   * `ceil`
   * `finite?`
   * `floor`
   * `infinite?`
   * `nan?`
   * `next_float`
   * `prev_float`
   * `zero?`
   * `ago`, `util`
   * `from_now`, `since`
   * `fortnight`, `fortnights`
   * `week`, `weeks`
   * `day`, `days`
   * `hour`, `hours`
   * `minute`, `minutes`
   * `second`, `seconds`

#Regexp
Regular expression object using `std::regex` with `std::regex::ECMAScript`.
As a result the syntax and capabilities differ from the Ruby Regexp object.
Some details are available at http://www.regular-expressions.info/stdregex.html .

Notable limitations are no support for a multiline flag, and no named captures.

There is also currently no `$~` or related global variables.

   * `::compile`
   * `::escape`
   * `::new`
   * `::quote`
   * `::IGNORECASE`
   * `==`
   * `hash`
   * `casefold?`
   * `match`
   * `options`
   * `source`
   * `to_s`

#Range
Numeric float range object using `..` and `...` literals. Include Enumerable.

Similar to the Ruby range object, but without custom type support.

   * `==`
   * `hash`
   * `begin`
   * `cover?`
   * `each`
   * `end`
   * `exclude_end?`
   * `first`
   * `include?`
   * `last`
   * `member?`
   * `size`
   * `step`

# Symbol
The `Symbol` type represents process-wide unique strings, allowing for fast comparison via identity, but somewhat slower creation. The script parser creates instances at compile time to avoid the expense on every execution.

C++ code can get instances using `slim::symbol(string)`, which is thread safe.

## Methods

   * `==`, `!=`
   * `<=>`: Uses the internal String object for comparison.
   * `to_s`: Returns the internal String object.

# Time
Time type providing a similar API to the Ruby Time object using a `time_t` with UTC conversions. As such it can only work with second precision and does not support other timezones.

## Methods

 * `::at`
 * `::gm`
 * `::local`
 * `::mktime`
 * `::new`
 * `::now`
 * `::utc`
 * `==`, `!=`
 * `<=>`
 * `to_a`
 * `to_f`
 * `to_i`
 * `asctime`, `ctime`
 * `strftime`
 * `dst?`, `isdst`
 * `getgm`, `getutc`, `utc`, `gmtime`
 * `gmt?`, `utc?`
 * `gmt_offset`, `utc_offset`, `gmtoff`
 * `zone`
 * `day`, `mday`
 * `hour`
 * `min`
 * `month`, `mon`
 * `nsec`, `tv_nsec`
 * `sec`, `tv_sec`
 * `usec`, `tv_usec`
 * `wday`
 * `yday`
 * `year`
 * `sunday?`, `monday?`, `tuesday?`, `wednesday?`, `thursday?`, `friday?`, `saturday?`
