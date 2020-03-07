#include <sead.h>

namespace sead {

template <typename T>
const T SafeStringBase<T>::cNullChar = 0;

template <typename T>
const T SafeStringBase<T>::cLineBreakChar = static_cast<T>('\n');

template <typename T>
const T SafeStringBase<T>::cNullString[1] = { SafeStringBase<T>::cNullChar };

template <typename T>
const SafeStringBase<T> SafeStringBase<T>::cEmptyString;

} // namespace sead
