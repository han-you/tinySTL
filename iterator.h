#ifndef MYTINYSTL_ITERATOR_H
#define MYTINYSTL_ITERATOR_H

// 迭代器设计
#include <cstddef>
#include "type_traits.h"
namespace mystl
{
  // 五种迭代器
  struct input_iterator_tag
  {
  };
  struct output_iterator_tag
  {
  };
  struct forward_iterator_tag : public input_iterator_tag
  {
  };
  struct bidirectional_iterator_tag : public forward_iterator_tag
  {
  };
  struct random_iterator_tag : public bidirectional_iterator_tag
  {
  };

  // iterator模板
  template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T *, class Reference = T &>
  struct iterator
  {
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
  };

  // iterator traits
  template <class T>
  struct has_iterator_cat
  {
  private:
    struct two
    {
      char a;
      char b;
    };
    template <class U>
    static two test(...);
    template <class U>
    static char test(typename U::iterator_category *= 0);

  public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
  };

  // 描述iterator特性
  template <class Iterator, bool>
  struct iterator_traits_impl
  {
  };

  template <class Iterator>
  struct iterator_traits_impl<Iterator, true>
  {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::difference_type difference_type;
  };

  template <class Iterator, bool>
  struct iterator_traits_helper
  {
  };

  template <class Iterator, bool>
  struct iterator_traits_helper<Iterator, true>
      : public iterator_traits_impl<Iterator,
                                    std::is_convertible<Iterator::category, input_iterator_tag>::value ||
                                        std::is_convertible<Iterator::category, output_iterator_tag>::value>
  {
  };

  // 迭代器特性萃取
  template <class Iterator>
  struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>
  {
  };

  // 原生指针类型特化
  template <class T>
  struct iterator_traits<T *>
  {
    typedef random_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef ptrdiff_type difference_type;
  };

  template <class T>
  struct iterator_traits<const T *>
  {
    typedef random_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef ptrdiff_type difference_type;
  };

  template <class T, class U, bool = has_iterator_cat<T>::value>
  struct has_iterator_cat_of : public m_bool_constant<std::is_convertible<typename T::iterator_category, U>::value>
  {
  };

  template <class T, class U>
  struct has_iterator_cat_of<T, U, false> : public m_false_type
  {
  };

  // 萃取某种迭代器
  template <class Iterator>
  struct is_input_iterator : public has_iterator_cat_of<Iterator, input_iterator_tag>
  {
  };

  template <class Iterator>
  struct is_output_iterator : public has_iterator_cat_of<Iterator, output_iterator_tag>
  {
  };

  template <class Iterator>
  struct is_forward_iterator : public has_iterator_cat_of<Iterator, forward_iterator_tag>
  {
  };

  template <class Iterator>
  struct is_bidirectional_iterator : public has_iterator_cat_of<Iterator, bidirectional_iterator_tag>
  {
  };

  template <class Iterator>
  struct is_random_iterator : public has_iterator_cat_of<Iterator, random_iterator_tag>
  {
  };
}

#endif