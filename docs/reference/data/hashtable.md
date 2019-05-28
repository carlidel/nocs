## Class `hashtable`

### Overview

Class `hashtable` is a polymorphic implementation of a generic hashtable.

### Interface

#### Constructor

  * `template <typename ktype, typename vtype> hashtable <ktype, vtype> :: hashtable()`

    builds an hashtable with `vtype` as value type and `ktype` as key type.

#### Destructor

  * `template <typename ktype, typename vtype> hashtable <ktype, vtype> :: ~hashtable()`

    destroys the hashtable.

#### Methods

  * `void add(const ktype & key, const vtype & value)`

    adds an element to the hashtable.

  * `void remove(const ktype & key)`

    removes the element with the givent key from the hashtable.

  * `template <typename lambda> void each(const lambda & function) const`

    given a lambda `function` that takes an argument of the same type of `vtype`, executes the lambda `function` to each element of the hashtable.

#### Operators

  * `const vtype & operator [] (const ktype & key)`

    access the k-th element of the hashtable.

### Private elements

#### Private methods

* `size_t slot(const ktype & key) const`
  
  Returns the corresponding index of the object with the given `key`.

* `void realloc(const size_t & alloc)`

  Allocates a new array of the given size an moves the elements of the hashtable to this new array. Called when it's necessary to allocate more memory.

#### Private static methods

* `template <typename type, typename std :: enable_if <sizeof(type) == 4> :: type * = nullptr> static size_t hash(const type & item)`
  
  Standard hashing method for an hashtable with `key` type of size 4.

* `template <typename type, typename std :: enable_if <sizeof(type) == 8> :: type * = nullptr> static size_t hash(const type &);`
  
  Standard hashing method for an hashtable with `key` type of size 8.