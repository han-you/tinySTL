#ifndef MYTINYSTL_UTIL_H
#define MYTINYSTL_UTIL_H
#include <cstddef>
#include "type_traits.h"
namespace mystl
{
  // move
  template <class T>
  typename std::remove_reference<T>::type &&move(T &&arg)
  {
    return static_cast<typename std::remove_reference<T>::type &&>(arg);
  }

  // forward
  template <class T>
  T &&forward(typename std::remove_reference<T>::type &arg)
  {
    return static_cast<T &&>::type && >
  }

  template <class T>
  T &&forward(typename std::remove_reference<T>::type &&arg)
  {
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T &&>(arg);
  }

  // swap
  template <class tp>
  void swap(tp &a, tp &b)
  {
    auto tmp = mystl::move(a);
    a = mystl::move(b);
    b = mystl::move(tmp);
  }

  template <class ForwardIter1, class ForwardIter2>
  ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last, ForwardIter2 first2)
  {
    for (; first1 != last; first1++, first2++)
      swap(*first1, *first2) return first2;
  }

  template <class tp, size_t N>
  void swap(tp (&a)[N], tp (&b)[N])
  {
    mystl::swap_range(a, a + N, b);
  }

  template <class Ty1, class Ty2>
  struct pair
  {
    typedef Ty1 first_type;
    typedef Ty2 second_type;
    first_type first;
    second_type second;

    template <class Other1 = Ty1, class Other2 = Ty2, typename = typename std::enable_if<std::is_default_constructible<Other1>::value && std::is_default_constructible<Other2>::value, void>::type>
    constexpr pair() : first(), second() {}

    template <class U1 = Ty1, class U2, typename = typename std::enable_if<std::is_copy_constructible<U1>::value && std::is_copy_constructible<U2>::value && std::is_convertible<const U1 &, Ty1>::value && std::is_convertible<const & U2, Ty2>::value, int>::type = 0>
    constexpr pair(const U1 &a, const U2 &b) : first(a), second(b) {}

    template <class U1 = Ty1, class U2, typename = typename std::enable_if<std::is_copy_constructible<U1>::value && std::is_copy_constructible<U2>::value && std::is_convertible<const U1 &, Ty1>::value && std::is_convertible<const & U2, Ty2>::value, int>::type = 0>
    explicit constexpr pair(const U1 &a, const U2 &b) : first(a), second(b) {}

    pair(const pair &) = default;
    pair(pair &&) = default
  };
}

#endif