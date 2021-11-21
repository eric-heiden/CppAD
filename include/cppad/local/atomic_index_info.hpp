#ifndef CPPAD_LOCAL_ATOMIC_INDEX_INFO_HPP
#define CPPAD_LOCAL_ATOMIC_INDEX_INFO_HPP

#include <string>

namespace CppAD {
namespace local {

struct atomic_index_info {
  size_t type;
  std::string name;
  void* ptr;
};
}  // namespace local
}  // namespace CppAD
#endif  // CPPAD_LOCAL_ATOMIC_INDEX_INFO_HPP