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
      swap(*first1, *first2);
    return first2;
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

    template <class U1 = Ty1, class U2, typename = typename std::enable_if<std::is_copy_constructible<U1>::value && std::is_copy_constructible<U2>::value && (!std::is_convertible<const U1 &, Ty1>::value || !std::is_convertible<const & U2, Ty2>::value), int>::type = 0>
    explicit constexpr pair(const U1 &a, const U2 &b) : first(a), second(b) {}

    pair(const pair &) = default;
    pair(pair &&) = default;

    template <class Other1, class Other2, typename = typename std::enable_if<std::is_constructible<Other1, Ty1>::value && std::is_constructible<Other2, Ty2>::value && std::is_convertible<Other1 &&, Ty1>::value && std::is_convertible<Other2 &&, Ty2>::value, int>::type = 0>
    constexpr pair(Other1 &&a, Other2 &&b) : first(mystl::forward<Other1>(a)), second(mystl::forward<Other2>(b)){};

    template <class Other1, class Other2, typename = typename std::enable_if<std::is_constructible<Other1, Ty1>::value && std::is_constructible<Other2, Ty2>::value && (!std::is_convertible<Other1 &&, Ty1>::value || !std::is_convertible<Other2 &&, Ty2>::value), int>::type = 0>
    explicit constexpr pair(Other1 &&a, Other2 &&b) : first(mystl::forward<Other1>(a)), second(mystl::forward<Other2>(b)){};

    template <class Other1, class Other2, typename = typename std::enable_if<std::is_constructible<const Other1 &, Ty1>::value && std::is_constructible<const Other2 &, Ty2>::value && std::is_convertible<const Other1 &, Ty1>::value && std::is_convertible<const Other2 &, Ty2>::value, int>::type = 0>
    constexpr pair(const pair<Other1, Other2> &other) : first(other.first), second(other.second){};

    template <class Other1, class Other2, typename = typename std::enable_if<std::is_constructible<const Other1 &, Ty1>::value && std::is_constructible<const Other2 &, Ty2>::value && (!std::is_convertible<const Other1 &, Ty1>::value || !std::is_convertible<const Other2 &, Ty2>::value), int>::type = 0>
    explicit constexpr pair(const pair<Other1, Other2> &other) : first(other.first), second(other.second){};

    template <class Other1, class Other2, typename = typename std::enable_if<std::is_constructible<Other1, Ty1>::value && std::is_constructible<Other2, Ty2>::value && std::is_convertible<Other1, Ty1>::value && std::is_convertible<Other2, Ty2>::value, int>::type = 0>
    constexpr pair(pair<Otehr1, Other2> &&other) : first(mystl::forward<Other1>(other.first)), second(mystl::forward<Other2>(other.second)){};

    template <class Other1, class Other2, typename = typename std::enable_if<std::is_constructible<Other1, Ty1>::value && std::is_constructible<Other2, Ty2>::value && (!std::is_convertible<Other1, Ty1>::value || !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
    explicit constexpr pair(pair<Otehr1, Other2> &&other) : first(mystl::forward<Other1>(other.first)), second(mystl::forward<Other2>(other.second)){};

    // 重载运算符
    pair &operator=(const pair &lhs)
    {
      if (this != &lhs)
      {
        this->first = lhs.first;
        this->second = lhs.second;
      }
      return *this;
    }

    pair &operator=(pair &&rhs)
    {
      if (this != &rhs)
      {
        this->first = mystl::move(rhs.first);
        this->second = mystl::move(rhs.second);
      }
      return *this;
    }

    template <class Otehr1, class Other2>
    pair &operator=(const pair<Otehr1, Other2> &lhs)
    {
      this->first = lhs.first;
      this->second = lhs.second;
      return *this;
    }

    template <class Otehr1, class Other2>
    pair &operator=(pair<Otehr1, Other2> &&rhs)
    {
      this->first = mystl::forward<Other1>(rhs.first);
      this->second = mystl::forward<Other2>(rhs.first);
      return *this;
    }

    ~pair() default;

    void swap(const pair &lsh)
    {
      if (this != &lsh)
      {
        mystl::swap(first, lsh.first);
        mystl::swap(second, lsh.second);
      }
    };
  };
  template <class Ty1, class Ty2>
  bool operator==(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs)
  {
    return lhs.first == rhs.first && lhs.second == rhs.second;
  }

  template <class Ty1, class Ty2>
  bool operator!=(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs)
  {
    return !(lhs == rhs);
  }

  template <class Ty1, class Ty2>
  bool operator<(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs)
  {
    return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
  }

  template <class Ty1, class Ty2>
  bool operator>(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs)
  {
    return rhs < lhs;
  }

  template <class Ty1, class Ty2>
  bool operator<=(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs)
  {
    return !(rhs < lhs);
  }

  template <class Ty1, class Ty2>
  bool operator>=(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs)
  {
    return !(lhs < rhs);
  }

  template <class Ty1, class Ty2>
  void swap(pair<Ty1, Ty2> &lhs, pair<Ty1, Ty2> &rhs)
  {
    lhs.swap(rhs);
  }

  template <class Ty1, class Ty2>
  pair<Ty1, Ty2> make_pair(Ty1 &&first, Ty2 &&second)
  {
    return pair<Ty1, Ty2>(mystl::forward<Ty1>(first), mystl::forward<Ty2>(second));
  }
}

#endif