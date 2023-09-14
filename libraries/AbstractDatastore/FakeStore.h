#ifndef FakeStore_h_INCLUDED
#define FakeStore_h_INCLUDED

#include "AbstractDatastore.h"

#include <vector>

template <typename T>
class FakeStore: public AbstractDatastore<T> {
  using result = typename AbstractDatastore<T>::result;

public:
  FakeStore(size_t cnt=1) {
    this->sz = cnt;
  }

  // Return a Hash over the Data in the Datastore
  uint32_t hash_data() {
    return 42; // TODO implement
  }

  // Overwrite the dataset at index `idx`
  result write(size_t idx, T dataset, bool commit=false) {
    if (idx >= this->sz) { return result::out_of_bounds; }
    if (idx > this->store.size()) { return result::out_of_bounds; }

    if (idx == this->store.size()) {
      this->store.push_back(dataset);
    } else {
      this->store[idx] = dataset;
    }

    return result::ok;
  }

  // Read the dataset at index `idx`
  result read(size_t idx, T& dataset) {
    if (idx >= this->store.size()) { return result::out_of_bounds; }

    dataset = this->store[idx];

    return result::ok;
  }

  // show how many datasets are stored
  size_t cnt() {
    return this->store.size();
  }

  // show the maximum number of datasets that could be stored
  size_t size() {
    return this->sz;
  }

  std::vector<T> store;
  size_t sz;
};

#endif // FakeStore_h_INCLUDED

