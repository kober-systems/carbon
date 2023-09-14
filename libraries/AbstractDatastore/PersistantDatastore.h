#ifndef PersistantDatastore_h_INCLUDED
#define PersistantDatastore_h_INCLUDED

#include "AbstractDatastore.h"

template <typename T>
class PersistantDatastore: public AbstractDatastore<T> {
  using result = typename AbstractDatastore<T>::result;

public:
  PersistantDatastore(size_t cnt=1) {}

  // Return a Hash over the Data in the Datastore
  uint32_t hash_data() {
  }

  // Overwrite the dataset at index `idx`
  result write(size_t idx, T dataset, bool commit=false) {
  }

  // Read the dataset at index `idx`
  result read(size_t idx, T& dataset) {
  }

  // show how many datasets are stored
  size_t cnt() {
    return 0; // TODO Implement
  }

  // show the maximum number of datasets that could be stored
  size_t size() {
  }
};

#endif // PersistantDatastore_h_INCLUDED

