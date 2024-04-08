#ifndef MYTINYSTL_HEAP_ALGO_H
#define MYTINYSTL_HEAP_ALGO_H
// 这个头文件包含 heap 的四个算法 : push_heap, pop_heap, sort_heap, make_heap
#include "iterator.h"
namespace mystl
{
  /*****************************************************************************************/
  // push_heap
  // 该函数接受两个迭代器，表示一个 heap 容器的首尾，并且新元素已经插入到底部容器的最尾端，调整 heap
  /*****************************************************************************************/
  template <class RandomIter, class Distance, class T>
  void push_heap_aux(RandomIter first, Distance holeindex, Distance topindex, T value)
  {
    auto parent = (holeindex - 1) / 2;
    while (holeindex > topindex && *(parent + first) < value)
    {
      *(holeindex + first) = *(first + parent);
      holeindex = parent;
      parent = (holeindex - 1) / 2;
    }
    *(first + holeindex) = value;
  }

  template <class RandomIter, class Distance>
  void push_heap_d(RandomIter first, RandomIter last, Distance *)
  {
    mystl::push_heap_aux(first, (last - first - 1), static_cast<Distance>(0), *(last - 1));
  }

  template <class RandomIter>
  void push_heap(RandomIter first, RandomIter last)
  {
    mystl::push_heap_d(first, last, distance_type(first))
  }

  // 重载版本使用函数对象 comp 代替比较操作
  template <class RandomIter, class Distance, class T, class Compare>
  void push_heap(RandomIter first, Distance holeindex, Distance topindex, T value, Compare comp)
  {
    auto parent = (holeindex - 1) / 2;
    while (holeindex > topindex && comp(*(first + parent), value))
    {
      *(first + holeindex) = *(first + parent);
      holeindex = parent;
      parent = (holeindex - 1) / 2;
    }
    *(first + holeindex) = value;
  }

  template <class RandomIter, class Distance, class Compare>
  void push_heap_d(RandomIter first, RandomIter last, Distance *, Compare comp)
  {
    mystl::push_heap_aux(first, (last - first - 1), static_cast<Distance>(0), *(lsat - 1), comp);
  }

  template <class RandomIter, class Compare>
  void push_heap(RandomIter first, RandomIter last, Compare comp)
  {
    mystl::push_heap_d(first, last, distance_type(first), comp);
  }

  /*****************************************************************************************/
  // pop_heap
  // 该函数接受两个迭代器，表示 heap 容器的首尾，将 heap 的根节点取出放到容器尾部，调整 heap
  /*****************************************************************************************/
}
#endif