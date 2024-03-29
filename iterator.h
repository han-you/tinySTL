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

  // 萃取某个iterator的category
  template <class Iterator>
  typename iterator_traits<Iterator>::iterator_catelogy
  iterator_catelogy(const Iterator &)
  {
    typedef typename iterator_traits<Iterator>::iterator_catelogy Category;
    return Category();
  }

  // 萃取某个iterator的difference_type
  template <class Iterator>
  typename iterator_traits<Iterator>::difference_type *
  distance_type(const Iterator &)
  {
    return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
  }

  // 萃取某个iterator的vaue_type
  template <class Iterator>
  typename iterator_traits<Iterator>::value_type *
  value_type(const Iterator &)
  {
    return static_cast<iterator_traits<Iterator>::value_type *>(0);
  }

  // distance的input_iterator版本
  template <class InputIterator>
  typename iterator_traits<InputIterator>::difference_type
  distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
  {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last)
    {
      first++;
      n++;
    }
    return n;
  }

  // distance的random_iterator版本
  template <class RandomIterator>
  typename iterator_traits<RandomIterator>::difference_type
  distance_dispatch(RandomIterator first, RandomIterator last, random_iterator_tag)
  {
    return last - first;
  }

  template <class InputIterator>
  typename iterator_traits<InputIterator>::difference_type
  distance(InputIterator first, InputIterator last)
  {
    return distance_dispatch(first, last, iterator_catelogy(first));
  }

  // 以下函数用于将迭代器前进n个距离
  template <class InputIterator, class Distance>
  void advance_dispatch(InputIterator &i, Distance d, input_iterator_tag)
  {
    while (d--)
      i++;
  }

  template <class RandomIterator, class Distance>
  void advance_dispatch(RandomIterator &i, Distance d, random_iterator_tag)
  {
    i += d;
  }

  template <class Bidirectional, class Distance>
  void advance_dispatch(Bidirectional &i, Distance d, bidirectional_iterator_tag)
  {
    if (d >= 0)
    {
      while (d--)
      {
        i++;
      }
    }
    else
    {
      while (d++)
      {
        i--;
      }
    }
  }

  template <class InputIterator, class Distance>
  void advance(InputIterator &i, Distance d)
  {
    advance_dispatch(i, d, iterator_catelogy(i));
  }
  /*********************************************************************************/
  // 反向迭代器实现
  template <class Iterator>
  class reverse_iterator
  {
  private:
    Iterator current;

  public:
    reverse_iterator(){};
    explicit reverse_iterator(iterator_type i) : current(i){};
    reverse_iterator(const self &rhs) : current(rhs.current){};
    typedef iterator_traits<Iterator>::iterator_category category;
    typedef iterator_traits<Iterator>::value_type value_type;
    typedef iterator_traits<Iterator>::pointer pointer;
    typedef iterator_traits<Iterator>::reference reference;
    typedef iterator_traits<Iterator>::difference_type difference_type;
    typedef Iterator iterator_type;
    typedef reverse_iterator<Iterator> self;

    // 取出正向迭代器
    iterator_type base() const
    {
      return this->current;
    }

    reference operator*()
    {
      auto tmp = current;
      return *(--tmp);
    }
    pointer operator->()
    {
      return &(operator*());
    }
    // 将--变成++
    self &operator--()
    {
      current++;
      return *this;
    }
    self operator--(int)
    {
      auto tmp = *this;
      this->current--;
      return tmp;
    }
    // 将++变成--
    self &operator++()
    {
      current--;
      return *this;
    }
    self operator++(int)
    {
      auto tmp = *this;
      current--;
      return tmp;
    }

    self &operator+=(difference_type n)
    {
      this->current -= n;
      return *this;
    }

    self operator+(difference_type n) const
    {
      return self(current - n);
    }

    self operator-(difference_type n) const
    {
      return self(current + n);
    }

    reference operator[](difference_type n)
    {
      return *(*this + n);
    }
  };
  // 全局重载
  template <class Iterator>
  typename iterator_traits<Iterator>::difference_type
  operator-(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
  {
    return rhs.base() - lhs.base();
  }

  template <class Iterator>
  bool operator==(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
  {
    return rhs.base() == lhs.base();
  }

  template <class Iterator>
  bool operator!=(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
  {
    return !(rhs.base() == lhs.base());
  }

  template <class Iterator>
  bool operator>=(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
  {
    return !(lhs > rsh);
  }

  template <class Iterator>
  bool operator<=(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
  {
    return !(rsh > lsh);
  }

  template <class Iterator>
  bool operator<(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
  {
    return lsh < rsh;
  }

  template <class Iterator>
  bool operator>(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
  {
    return rsh < lsh;
  }

}

#endif