#pragma once

/// @file crypto/algorithm.hpp
/// @brief @copybrief crypto::algorithm

#include <string_view>

/// Miscellaneous cryptographic routines
namespace crypto::algorithm {

/// Performs constant-time string comparison if the strings are of equal size
bool AreStringsEqualConstTime(std::string_view str1, std::string_view str2);

}  // namespace crypto::algorithm
