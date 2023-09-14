#ifndef AbstractDatastore_h_INCLUDED
#define AbstractDatastore_h_INCLUDED

#include <stdint.h>
#include <cstddef>

template <typename T>
class AbstractDatastore {
public:
  virtual ~AbstractDatastore() = default;

  enum class result {
    ok = 0,
    // the requested index is not available
    out_of_bounds = -1,
    invalid_inputs = -2,
    // Some internal problem in the store (E.g writing is not possible)
    internal = -3,
  };

  // Overwrite the dataset at index `idx`
  virtual result write(size_t idx, T dataset, bool commit=false) = 0;
  // Read the dataset at index `idx`
  virtual result read(size_t idx, T& dataset) = 0;

  // show how many datasets are stored
  virtual size_t cnt() = 0;
  // show the maximum number of datasets that could be stored
  virtual size_t size() = 0;
};

#endif // AbstractDatastore_h_INCLUDED
