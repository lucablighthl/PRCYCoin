// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_MUTEXLOCK_H_
#define STORAGE_LEVELDB_UTIL_MUTEXLOCK_H_

#include "port/port.h"
#include "port/thread_annotations.h"

namespace leveldb {

// Helper class that locks a mutex on construction and unlocks the mutex when
// the destructor of the MutexLock object is invoked.
//
// Typical usage:
//
//   void MyClass::MyMethod() {
//     MutexLock l(&mu_);       // mu_ is an instance variable
//     ... some complex code, possibly with multiple return paths ...
//   }

class SCOPED_LOCKABLE MutexLock {
 public:
<<<<<<< HEAD
  explicit MutexLock(port::Mutex *mu) EXCLUSIVE_LOCK_FUNCTION(mu)
      : mu_(mu)  {
=======
  explicit MutexLock(port::Mutex* mu) EXCLUSIVE_LOCK_FUNCTION(mu) : mu_(mu) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    this->mu_->Lock();
  }
  ~MutexLock() UNLOCK_FUNCTION() { this->mu_->Unlock(); }

<<<<<<< HEAD
 private:
  port::Mutex *const mu_;
  // No copying allowed
  MutexLock(const MutexLock&);
  void operator=(const MutexLock&);
=======
  MutexLock(const MutexLock&) = delete;
  MutexLock& operator=(const MutexLock&) = delete;

 private:
  port::Mutex* const mu_;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

}  // namespace leveldb

<<<<<<< HEAD

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif  // STORAGE_LEVELDB_UTIL_MUTEXLOCK_H_
