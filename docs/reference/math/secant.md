## Class `secant`

### Overview

Class `secant` implements the secant method for a generic lambda function that takes a `double` as argument.

### Example usage

```c++
auto function = [&](const double & x)
{
  return x * x - 4;
}

std :: cout << secant :: compute(function, -5, 0) << std :: endl; // Prints -2
std :: cout << secant :: compute(function, -0, 5) << std :: endl; // Prints 2
```

### Interface

#### Static members

 * `static constexpr double epsilon`
    
    machine epsilon considered for the computation

 * `static constexpr unsigned int rounds`
    
    maximum number of iterations before forced timeout

#### Static methods

 * `template <typename type, typename std :: enable_if <secant :: valid <type> :: value> :: type *> double secant :: compute(const type & f, double a, double b)`
    
    using secant method, returns the zero of function inside the range [lower_bound, upper_bound]. The templatization, with the help of the Service nested classes, checks if the passed function `f` is a valid type function. If not, you will get a compilation error.
    
    **REMARK: works only if in the given range the zero is unique and the function is monotonic**
