#ifndef MYTINYSTL_ALGOBASE_H
#define MYTINYSTL_ALGOBASE_H
#include <cstring>
#include "util.h"
#include "iterator.h"
namespace mystl
{
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min
  /***********************************************************************************/
  // max
  // 取二者中的较大值，语义相等时保证返回第一个参数
  /***********************************************************************************/
  template <class T>
  const T &max(const T &lhs, const T &rhs)
  {
    return lhs < rhs ? rhs : lhs;
  }

  template <class T, class Compare>
  const T &max(const T &lhs, const T &rhs, Compare compare)
  {
    return compare(lhs, rhs) ? rhs : lhs;
  }
  // min
  // 取二者中的较小值，语义相等时保证返回第一个参数
  /**************************************************************************************/
  template <class T>
  const T &max(const T &lhs, const T &rhs)
  {
    return rhs < lhs ? rhs : lhs;
  }

  template <class T, class Compare>
  const T &max(const T &lhs, const T &rhs, Compare compare)
  {
    return compare(rhs, rhs) ? rhs : lhs;
  }

  // iter_swap
  //  将两个迭代器交换
  /**************************************************************************************/
  template <class FIter1, class FIter2>
  void iter_swap(FIter1 lhs, FIter2 rhs)
  {
    swap(*lhs, *rhs);
  }

  // copy
  // 将[first，last)内容拷贝到[result+first,result+last-first]
  /*************************************************************************************************/
  // input_iterator_tag 版本
  template <class InputIterator, class OutputIterator>
  OutputIterator uncheck_copy_cat(InputIterator first, InputIterator last, OutputIterator result, mystl::input_iterator_tag)
  {
    for (; first != last; first++, result++)
    {
      *result = *first;
    }
    return result;
  }

  // random_iterator_tag 版本
  template <class RandomIterator, class OutputIterator>
  OutputIterator unchecked_copy_cat(RandomIterator first, RandomIterator last, OutputIterator result, mystl::random_iterator_tag)
  {
    for (int i = last - first; n > 0; n--, result++, first++)
    {
      *result = *first;
    }
    return result;
  }

  template <class InputIter, class OutputIter>
  OutputIter unchecked_copy(InputIter first, InputIter last, OutputIter result)
  {
    return uncheck_copy_cat(first, last, result, iterator_category(first));
  }

  // trivially_copy_assignable
  template <class Tp, class Up>
  typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type, Up>::value && std::is_trivially_copy_assignable<Up>::value, Up *>::type
  unchecked_copy(Tp *first, Tp *last, Up *result)
  {
    const auto n = static_cast<size_t>(last - first);
    if (n != 0)
      std::memmove(result, first, n * sizeof(Up));
    return result + n;
  }

  template <class InputIter, class OutputIter>
  OutputIter copy(InputIter first, InputIter second, OutputIter result)
  {
    return unchecked_copy(first, last, result);
  }

  /********************************************************************************************************************/
  // copy_backward
  // 将[first，last)区间内的数据拷贝到[result-(last-first)，reuslt）内
  /***************************************************************************************************/
  // unchecked_copy_backward_cat 的 bidirectional_iterator_tag 版本
  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2 uncheked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result, mystl::bidirectional_iterator_tag)
  {
    while (first != last)
      *(--result) = *(--last);
    return result;
  }

  // unchecked_copy_backward_cat 的 random_access_iterator_tag 版本
  template <class RandomIter1, class BidirectionalIter2>
  BidirectionalIter2 uncheck_copy_backward_cat(RandomIter1 first, RandomIter1 last, BidirectionalIter2 result, mystl::random_iterator_tag)
  {
    for (auto i = last - first; i > 0; i--)
      *--result = *--last;
    return result;
  }

  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2 unchecked_copy_backward(BidirectionalIter1 first, BidirectionIter1 last, BidirectionalIter2 result)
  {
    return uncheck_copy_backward_cat(first, last, result, iterator_catelgory(first));
  }

  // 为 trivially_copy_assignable 类型提供特化版本
  template <class Tp, class Up>
  typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
          std::is_trivially_copy_assignable<Up>::value,
      Up *>::type
  unchecked_copy_backward(Tp *first, Tp *last, Up *result)
  {
    const auto n = static_cast<size_t>(last - first);
    if (n != 0)
    {
      result -= n;
      std::memmove(result, first, n * sizeof(Up));
    }
    return result;
  }

  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
  {
    return unchecked_copy_backward(first, last, result);
  }

  /*****************************************************************************************/
  // copy_if
  // 把[first, last)内满足一元操作 unary_pred 的元素拷贝到以 result 为起始的位置上
  /*****************************************************************************************/
  template <class InputIter, class OutputIter, class UnaryPredicate>
  OutputIter copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredicate unary_pred)
  {
    while (first != last)
    {
      if (unary_pred(*first))
        *result++ = *first++;
    }
    return result;
  }

  /*****************************************************************************************/
  // copy_n
  // 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
  // 返回一个 pair 分别指向拷贝结束的尾部
  /*****************************************************************************************/
  template <class InputIter, class OutputIter, class Size>
  mystl::pair<InputIter, OutputIter> unchecked_copy_n(InputIter first, OutputIter result, Size n, mystl::input_iterator_tag)
  {
    while (n--)
    {
      *result = *first;
      result++;
      first++;
    }
    return mystl::pair<InputIter, OutputIter>(first, result);
  }

  template <class RandomIter, class OutputIter, class Size>
  mystl::pair<RandomIter, OutputIter> unchecked_copy_n(RandomIter first, OutputIter result, Size n, mystl::random_iterator_tag)
  {
    auto last = first + n;
    return mystl::pair<RandomIter, OutputIter>(last, copy(first, last, result));
  }

  template <class InputIter, class OutputIter, class Size>
  mystl::pair<InputIter, OutputIter> copy_n(InputIter first, OutputIter result, Size n)
  {
    return unchecked_copy_n(first, result, n);
  }

  /*****************************************************************************************/
  // move
  // 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
  /*****************************************************************************************/
  // input_iterator_tag 版本
  template <class InputIter, class OutputIter>
  OutputIter unchecked_move_cat(InputIter first, InputIter last, OutputIter result, mystl::input_iterator_tag)
  {
    while (first != last)
    {
      *result = mystl::move(*first);
      first++;
      result++;
    }
    return result;
  }

  // random_iterator_tag版本
  template <class RandomIter, class OutputIter>
  OutputIter unchecked_move_cat(RandomIter first, RandomIter last, OutputIter result, mystl::random_iterator_tag)
  {
    for (auto i = last - first; i > 0; i--)
    {
      *result = mystl::move(*first);
      *first++;
      *result++;
    }
  }

  template <class InputIter, class OutputIter>
  OutputIter unchecked_move(InputIter first, InputIter last, OutputIter result)
  {
    return unchecked_move_cat(first, last, result, iterator_category(first));
  }

  // 为 trivially_copy_assignable 类型提供特化版本
  template <class Tp, class Up>
  typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
          std::is_trivially_move_assignable<Up>::value,
      Up *>::type
  unchecked_move(Tp *first, Tp *last, Up *result)
  {
    const size_t n = static_cast<size_t>(last - first);
    if (n != 0)
      std::memmove(result, first, n * sizeof(Up));
    return result + n;
  }

  template <class InputIter, class OutputIter>
  OutputIter move(InputIter first, InputIter last, OutputIter result)
  {
    return unchecked_move(first, last, result);
  }

  /*****************************************************************************************/
  // move_backward
  // 将 [first, last)区间内的元素移动到 [result - (last - first), result)内
  /*****************************************************************************************/
  // bidirectional_iterator_tag 版本
  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2 unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result, mystl::bidirectional_iterator_tag)
  {
    while (first != last)
    {
      *--result = mystl::move(*--last);
    }
    return result;
  }

  // random_iterator_tag
  template <class RandomIter, class BidirectionalIter>
  BidirectionalIter unchecked_move_backward_cat(RandomIter first, RandomIter last, BidirectionalIter result, mystl::random_iterator_tag)
  {
    for (auto i = last - first; i > 0; i--)
    {
      *--result = mystl::move(*--last);
    }
    return result;
  }

  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2 unchecked_move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
  {
    return uncheck_copy_backward_cat(first, last, result, iterator_category(first));
  }

  // 为 trivially_copy_assignable 类型提供特化版本
  template <class Tp, class Up>
  typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
          std::is_trivially_move_assignable<Up>::value,
      Up *>::type
  unchecked_move_backward(Tp *first, Tp *last, Up *result)
  {
    const size_t n = static_cast<size_t>(last - first);
    if (n != 0)
    {
      result -= n;
      std::memmove(result, first, n * sizeof(Up));
    }
    return result;
  }

  template <class BidirectionalIter1, class BidirectionalIter2>
  BidirectionalIter2 move_backward(BidirectionalIter1 first, BidirectionalIter2 last, BidirectionalIter2 result)
  {
    return unchecked_move_backward(first, last, result);
  }

  /*****************************************************************************************/
  // equal
  // 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
  /*****************************************************************************************/
  template <class InputIter1, class InputIter2>
  bool equal(InputIter1 first1, InputIter1 first2, InputIter2 result)
  {
    while (first1 != last1)
    {
      if (*first != *first2)
        return false;
      first1++;
      first2++;
    }
    return true;
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class InputIter1, class InputIter2, class Compare>
  bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compare comp)
  {
    while (first != last)
    {
      if (!comp(*first1, *first2))
        return false;
      first++;
      first2++;
    }
    return true;
  }
}

#endif