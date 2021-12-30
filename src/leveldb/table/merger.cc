// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "table/merger.h"

#include "leveldb/comparator.h"
#include "leveldb/iterator.h"
#include "table/iterator_wrapper.h"

namespace leveldb {

namespace {
class MergingIterator : public Iterator {
 public:
  MergingIterator(const Comparator* comparator, Iterator** children, int n)
      : comparator_(comparator),
        children_(new IteratorWrapper[n]),
        n_(n),
<<<<<<< HEAD
        current_(NULL),
=======
        current_(nullptr),
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        direction_(kForward) {
    for (int i = 0; i < n; i++) {
      children_[i].Set(children[i]);
    }
  }

<<<<<<< HEAD
  virtual ~MergingIterator() {
    delete[] children_;
  }

  virtual bool Valid() const {
    return (current_ != NULL);
  }

  virtual void SeekToFirst() {
=======
  ~MergingIterator() override { delete[] children_; }

  bool Valid() const override { return (current_ != nullptr); }

  void SeekToFirst() override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    for (int i = 0; i < n_; i++) {
      children_[i].SeekToFirst();
    }
    FindSmallest();
    direction_ = kForward;
  }

<<<<<<< HEAD
  virtual void SeekToLast() {
=======
  void SeekToLast() override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    for (int i = 0; i < n_; i++) {
      children_[i].SeekToLast();
    }
    FindLargest();
    direction_ = kReverse;
  }

<<<<<<< HEAD
  virtual void Seek(const Slice& target) {
=======
  void Seek(const Slice& target) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    for (int i = 0; i < n_; i++) {
      children_[i].Seek(target);
    }
    FindSmallest();
    direction_ = kForward;
  }

<<<<<<< HEAD
  virtual void Next() {
=======
  void Next() override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    assert(Valid());

    // Ensure that all children are positioned after key().
    // If we are moving in the forward direction, it is already
    // true for all of the non-current_ children since current_ is
    // the smallest child and key() == current_->key().  Otherwise,
    // we explicitly position the non-current_ children.
    if (direction_ != kForward) {
      for (int i = 0; i < n_; i++) {
        IteratorWrapper* child = &children_[i];
        if (child != current_) {
          child->Seek(key());
          if (child->Valid() &&
              comparator_->Compare(key(), child->key()) == 0) {
            child->Next();
          }
        }
      }
      direction_ = kForward;
    }

    current_->Next();
    FindSmallest();
  }

<<<<<<< HEAD
  virtual void Prev() {
=======
  void Prev() override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    assert(Valid());

    // Ensure that all children are positioned before key().
    // If we are moving in the reverse direction, it is already
    // true for all of the non-current_ children since current_ is
    // the largest child and key() == current_->key().  Otherwise,
    // we explicitly position the non-current_ children.
    if (direction_ != kReverse) {
      for (int i = 0; i < n_; i++) {
        IteratorWrapper* child = &children_[i];
        if (child != current_) {
          child->Seek(key());
          if (child->Valid()) {
            // Child is at first entry >= key().  Step back one to be < key()
            child->Prev();
          } else {
            // Child has no entries >= key().  Position at last entry.
            child->SeekToLast();
          }
        }
      }
      direction_ = kReverse;
    }

    current_->Prev();
    FindLargest();
  }

<<<<<<< HEAD
  virtual Slice key() const {
=======
  Slice key() const override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    assert(Valid());
    return current_->key();
  }

<<<<<<< HEAD
  virtual Slice value() const {
=======
  Slice value() const override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    assert(Valid());
    return current_->value();
  }

<<<<<<< HEAD
  virtual Status status() const {
=======
  Status status() const override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    Status status;
    for (int i = 0; i < n_; i++) {
      status = children_[i].status();
      if (!status.ok()) {
        break;
      }
    }
    return status;
  }

 private:
<<<<<<< HEAD
=======
  // Which direction is the iterator moving?
  enum Direction { kForward, kReverse };

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  void FindSmallest();
  void FindLargest();

  // We might want to use a heap in case there are lots of children.
  // For now we use a simple array since we expect a very small number
  // of children in leveldb.
  const Comparator* comparator_;
  IteratorWrapper* children_;
  int n_;
  IteratorWrapper* current_;
<<<<<<< HEAD

  // Which direction is the iterator moving?
  enum Direction {
    kForward,
    kReverse
  };
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  Direction direction_;
};

void MergingIterator::FindSmallest() {
<<<<<<< HEAD
  IteratorWrapper* smallest = NULL;
  for (int i = 0; i < n_; i++) {
    IteratorWrapper* child = &children_[i];
    if (child->Valid()) {
      if (smallest == NULL) {
=======
  IteratorWrapper* smallest = nullptr;
  for (int i = 0; i < n_; i++) {
    IteratorWrapper* child = &children_[i];
    if (child->Valid()) {
      if (smallest == nullptr) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        smallest = child;
      } else if (comparator_->Compare(child->key(), smallest->key()) < 0) {
        smallest = child;
      }
    }
  }
  current_ = smallest;
}

void MergingIterator::FindLargest() {
<<<<<<< HEAD
  IteratorWrapper* largest = NULL;
  for (int i = n_-1; i >= 0; i--) {
    IteratorWrapper* child = &children_[i];
    if (child->Valid()) {
      if (largest == NULL) {
=======
  IteratorWrapper* largest = nullptr;
  for (int i = n_ - 1; i >= 0; i--) {
    IteratorWrapper* child = &children_[i];
    if (child->Valid()) {
      if (largest == nullptr) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        largest = child;
      } else if (comparator_->Compare(child->key(), largest->key()) > 0) {
        largest = child;
      }
    }
  }
  current_ = largest;
}
}  // namespace

<<<<<<< HEAD
Iterator* NewMergingIterator(const Comparator* cmp, Iterator** list, int n) {
=======
Iterator* NewMergingIterator(const Comparator* comparator, Iterator** children,
                             int n) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  assert(n >= 0);
  if (n == 0) {
    return NewEmptyIterator();
  } else if (n == 1) {
<<<<<<< HEAD
    return list[0];
  } else {
    return new MergingIterator(cmp, list, n);
=======
    return children[0];
  } else {
    return new MergingIterator(comparator, children, n);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  }
}

}  // namespace leveldb
