#ifndef MYTINYSTL_TYPE_TRAITS_H
#define MYTINYSTL_TYPE_TRAITS_H
// 提供类型信息
#include <type_traits>
namespace mystl
{

  // helper struct
  template <class T, T v>
  struct m_integral_constant
  {
    static constexpr T value = v;
  };

  template <bool b>
  using m_bool_constant = m_integral_constant<bool, b>;

  typedef m_bool_constant<true> m_true_type;
  typedef m_bool_constant<false> m_false_type;
  /******************************************************************************/
  // 前向申明开始
  template <class T1, class T2>
  struct pair;
  // 前向申明结束

  template <class T>
  struct is_pair : mystl::m_false_type
  {
  };

  template <class T1, class T2>
  struct is_pair<mystl::pair<T1, T2>> : mystl::m_true_type
  {
  };
} // namespace mystl;
#endif