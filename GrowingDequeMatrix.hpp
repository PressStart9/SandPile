#pragma once

#include <cstdint>

#include "GrowingDeque.hpp"

template<typename Elem, size_t ChunkLinSize = 16>
class GrowingDequeMatrix {
 private:
  class MatrixRow;
  class Chunk;
 public:
  using value_type = Elem;
  using reference = Elem&;
  using const_reference = const Elem&;
  using pointer = Elem*;
  using const_pointer = const Elem*;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  GrowingDequeMatrix() {
    table_.push_back(GrowingDeque<Chunk*>());
    table_[0].push_back(new Chunk());
  }
  GrowingDequeMatrix(const GrowingDequeMatrix&) = delete;
  GrowingDequeMatrix(GrowingDequeMatrix&&) noexcept = delete;

  GrowingDequeMatrix& operator=(const GrowingDequeMatrix&) = delete;
  GrowingDequeMatrix& operator=(GrowingDequeMatrix&&) noexcept = delete;

  ~GrowingDequeMatrix() {
    for (size_t i = 0; i < table_.size(); ++i) {
      for (size_t j = 0; j < table_[i].size(); ++j) {
        delete table_[i][j];
      }
    }
  }

  MatrixRow operator[](size_t x) {
    return {*this, x};
  }

  const MatrixRow operator[](size_t x) const {
    return {*this, x};
  }

  void extend_x_back() {
    ++size_x_;
    if ((offset_x_ + size_x_) / ChunkLinSize == table_.size()) {
      add_chunk_x_back();
    }
  }

  void extend_x_front() {
    ++size_x_;
    if (offset_x_ == 0) {
      add_chunk_x_front();
    }
    --offset_x_;
  }

  void extend_y_back() {
    ++size_y_;
    if ((offset_y_ + size_y_) / ChunkLinSize == table_[0].size()) {
      add_chunk_y_back();
    }
  }

  void extend_y_front() {
    ++size_y_;
    if (offset_y_ == 0) {
      add_chunk_y_front();
    }
    --offset_y_;
  }

  size_t size_x() const { return size_x_; }

  size_t size_y() const { return size_y_; }
  
 private:
  class Chunk {
   public:
    Chunk() : bytes_(new value_type[ChunkLinSize * ChunkLinSize]{}) {}

    ~Chunk() {
      delete[] bytes_;
    }

    reference get(size_t x, size_t y) {
      return bytes_[x * ChunkLinSize + y];
    }

    const_reference get(size_t x, size_t y) const {
      return bytes_[x * ChunkLinSize + y];
    }

   private:
    value_type* bytes_;
  };

  class MatrixRow {
   public:
    reference operator[](size_t y) {
      size_t x = field_.offset_x_ + x_;
      y += field_.offset_y_;
      return field_.table_[x / ChunkLinSize][y / ChunkLinSize]->get(x % ChunkLinSize, y % ChunkLinSize);
    }
   private:
    MatrixRow(GrowingDequeMatrix& field, size_t x) : field_(field), x_(x) {}

    GrowingDequeMatrix& field_;
    size_t x_;

    friend GrowingDequeMatrix;
  };

  GrowingDeque<GrowingDeque<Chunk*>> table_;

  size_t size_x_ = 0;
  size_t size_y_ = 0;
  size_t offset_x_ = 0;
  size_t offset_y_ = 0;

  void add_chunk_x_back() {
    table_.push_back(GrowingDeque<Chunk*>(table_[0].size()));
    auto& row = table_[table_.size() - 1];
    for (size_t i = 0; i < row.size(); ++i) {
      row[i] = new Chunk();
    }
  }

  void add_chunk_x_front() {
    offset_x_ += ChunkLinSize;
    table_.push_front(GrowingDeque<Chunk*>(table_[0].size()));
    auto& row = table_[0];
    for (size_t i = 0; i < row.size(); ++i) {
      row[i] = new Chunk();
    }
  }

  void add_chunk_y_back() {
    for (size_t i = 0; i < table_.size(); ++i) {
      table_[i].push_back(new Chunk());
    }
  }

  void add_chunk_y_front() {
    offset_y_ += ChunkLinSize;
    for (size_t i = 0; i < table_.size(); ++i) {
      table_[i].push_front(new Chunk());
    }
  }
};
