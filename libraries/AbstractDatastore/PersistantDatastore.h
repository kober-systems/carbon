#ifndef PersistantDatastore_h_INCLUDED
#define PersistantDatastore_h_INCLUDED

#include "AbstractDatastore.h"

#include <EEPROM.h>
#include <type_traits>
#include <vector>

template <typename T>
class PersistantDatastore: public AbstractDatastore<T> {
  using result = typename AbstractDatastore<T>::result;
  static_assert(std::is_trivially_copyable<T>::value,
                "EEPROM datasets must be trivially copyable");

  struct Header {
    uint32_t magic;
    uint16_t element_size;
    uint16_t capacity;
    uint16_t count;
  };

  static constexpr uint32_t magic = 0x43525331; // "CRS1" storage format

public:
  // Reserves EEPROM bytes [0, sizeof(Header) + capacity * sizeof(T)).
  // Changing T or capacity initializes an empty store in this region.
  explicit PersistantDatastore(size_t capacity=1)
      : capacity_(capacity), count_(0), valid_(capacity <= UINT16_MAX &&
          sizeof(T) <= UINT16_MAX && sizeof(Header) <= EEPROM.length() &&
          capacity <= (EEPROM.length() - sizeof(Header)) / sizeof(T)) {
    if (!valid_) return;
    data_.resize(capacity_);

    Header header;
    EEPROM.get(0, header);
    if (header.magic == magic && header.element_size == sizeof(T) &&
        header.capacity == capacity_ && header.count <= capacity_) {
      count_ = header.count;
      for (size_t i = 0; i < count_; ++i) {
        EEPROM.get(sizeof(Header) + i * sizeof(T), data_[i]);
      }
    }
  }

  // FNV-1a over the currently visible datasets (including uncommitted writes).
  uint32_t hash_data() {
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < count_; ++i) {
      const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&data_[i]);
      for (size_t j = 0; j < sizeof(T); ++j) {
        hash = (hash ^ bytes[j]) * 16777619u;
      }
    }
    return hash;
  }

  // Overwrite the dataset at index `idx`
  result write(size_t idx, T dataset, bool commit=false) override {
    if (idx >= capacity_ || idx > count_) return result::out_of_bounds;
    if (!valid_) return result::internal;

    data_[idx] = dataset;
    if (idx == count_) ++count_;
    if (commit) {
      for (size_t i = 0; i < count_; ++i) {
        EEPROM.put(sizeof(Header) + i * sizeof(T), data_[i]);
      }
      const Header header = { magic, static_cast<uint16_t>(sizeof(T)),
                              static_cast<uint16_t>(capacity_),
                              static_cast<uint16_t>(count_) };
      EEPROM.put(0, header);
    }
    return result::ok;
  }

  // Read the dataset at index `idx`
  result read(size_t idx, T& dataset) override {
    if (idx >= count_) return result::out_of_bounds;
    if (!valid_) return result::internal;
    dataset = data_[idx];
    return result::ok;
  }

  // show how many datasets are stored
  size_t cnt() override { return count_; }

  // show the maximum number of datasets that could be stored
  size_t size() override { return capacity_; }

private:
  size_t capacity_;
  size_t count_;
  bool valid_;
  std::vector<T> data_;
};

#endif // PersistantDatastore_h_INCLUDED
