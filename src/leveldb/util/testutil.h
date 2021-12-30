// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_TESTUTIL_H_
#define STORAGE_LEVELDB_UTIL_TESTUTIL_H_

<<<<<<< HEAD
=======
#include "helpers/memenv/memenv.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "leveldb/env.h"
#include "leveldb/slice.h"
#include "util/random.h"

namespace leveldb {
namespace test {

// Store in *dst a random string of length "len" and return a Slice that
// references the generated data.
<<<<<<< HEAD
extern Slice RandomString(Random* rnd, int len, std::string* dst);

// Return a random key with the specified length that may contain interesting
// characters (e.g. \x00, \xff, etc.).
extern std::string RandomKey(Random* rnd, int len);
=======
Slice RandomString(Random* rnd, int len, std::string* dst);

// Return a random key with the specified length that may contain interesting
// characters (e.g. \x00, \xff, etc.).
std::string RandomKey(Random* rnd, int len);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

// Store in *dst a string of length "len" that will compress to
// "N*compressed_fraction" bytes and return a Slice that references
// the generated data.
<<<<<<< HEAD
extern Slice CompressibleString(Random* rnd, double compressed_fraction,
                                size_t len, std::string* dst);
=======
Slice CompressibleString(Random* rnd, double compressed_fraction, size_t len,
                         std::string* dst);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

// A wrapper that allows injection of errors.
class ErrorEnv : public EnvWrapper {
 public:
  bool writable_file_error_;
  int num_writable_file_errors_;

<<<<<<< HEAD
  ErrorEnv() : EnvWrapper(Env::Default()),
               writable_file_error_(false),
               num_writable_file_errors_(0) { }

  virtual Status NewWritableFile(const std::string& fname,
                                 WritableFile** result) {
    if (writable_file_error_) {
      ++num_writable_file_errors_;
      *result = NULL;
=======
  ErrorEnv()
      : EnvWrapper(NewMemEnv(Env::Default())),
        writable_file_error_(false),
        num_writable_file_errors_(0) {}
  ~ErrorEnv() override { delete target(); }

  Status NewWritableFile(const std::string& fname,
                         WritableFile** result) override {
    if (writable_file_error_) {
      ++num_writable_file_errors_;
      *result = nullptr;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
      return Status::IOError(fname, "fake error");
    }
    return target()->NewWritableFile(fname, result);
  }

<<<<<<< HEAD
  virtual Status NewAppendableFile(const std::string& fname,
                                   WritableFile** result) {
    if (writable_file_error_) {
      ++num_writable_file_errors_;
      *result = NULL;
=======
  Status NewAppendableFile(const std::string& fname,
                           WritableFile** result) override {
    if (writable_file_error_) {
      ++num_writable_file_errors_;
      *result = nullptr;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
      return Status::IOError(fname, "fake error");
    }
    return target()->NewAppendableFile(fname, result);
  }
};

}  // namespace test
}  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_TESTUTIL_H_
