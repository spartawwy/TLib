// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TLib/tool/tsystem_generic_param.proto

#ifndef PROTOBUF_TLib_2ftool_2ftsystem_5fgeneric_5fparam_2eproto__INCLUDED
#define PROTOBUF_TLib_2ftool_2ftsystem_5fgeneric_5fparam_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include "TLib/core/tsystem_return_code.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_TLib_2ftool_2ftsystem_5fgeneric_5fparam_2eproto();
void protobuf_AssignDesc_TLib_2ftool_2ftsystem_5fgeneric_5fparam_2eproto();
void protobuf_ShutdownFile_TLib_2ftool_2ftsystem_5fgeneric_5fparam_2eproto();

class GenericParam;

enum GenericParam_DataType {
  GenericParam_DataType_UINT = 0,
  GenericParam_DataType_INT = 1,
  GenericParam_DataType_DOUBLE = 2,
  GenericParam_DataType_STRING = 3
};
bool GenericParam_DataType_IsValid(int value);
const GenericParam_DataType GenericParam_DataType_DataType_MIN = GenericParam_DataType_UINT;
const GenericParam_DataType GenericParam_DataType_DataType_MAX = GenericParam_DataType_STRING;
const int GenericParam_DataType_DataType_ARRAYSIZE = GenericParam_DataType_DataType_MAX + 1;

enum GenericParam_SizeType {
  GenericParam_SizeType_SCALAR = 0,
  GenericParam_SizeType_ARRAY = 1
};
bool GenericParam_SizeType_IsValid(int value);
const GenericParam_SizeType GenericParam_SizeType_SizeType_MIN = GenericParam_SizeType_SCALAR;
const GenericParam_SizeType GenericParam_SizeType_SizeType_MAX = GenericParam_SizeType_ARRAY;
const int GenericParam_SizeType_SizeType_ARRAYSIZE = GenericParam_SizeType_SizeType_MAX + 1;

// ===================================================================

class GenericParam : public ::google::protobuf::MessageLite {
 public:
  GenericParam();
  virtual ~GenericParam();

  GenericParam(const GenericParam& from);

  inline GenericParam& operator=(const GenericParam& from) {
    CopyFrom(from);
    return *this;
  }

  static const GenericParam& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const GenericParam* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(GenericParam* other);

  // implements Message ----------------------------------------------

  GenericParam* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const GenericParam& from);
  void MergeFrom(const GenericParam& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  typedef GenericParam_DataType DataType;
  static const DataType UINT = GenericParam_DataType_UINT;
  static const DataType INT = GenericParam_DataType_INT;
  static const DataType DOUBLE = GenericParam_DataType_DOUBLE;
  static const DataType STRING = GenericParam_DataType_STRING;
  static inline bool DataType_IsValid(int value) {
    return GenericParam_DataType_IsValid(value);
  }
  static const DataType DataType_MIN =
    GenericParam_DataType_DataType_MIN;
  static const DataType DataType_MAX =
    GenericParam_DataType_DataType_MAX;
  static const int DataType_ARRAYSIZE =
    GenericParam_DataType_DataType_ARRAYSIZE;

  typedef GenericParam_SizeType SizeType;
  static const SizeType SCALAR = GenericParam_SizeType_SCALAR;
  static const SizeType ARRAY = GenericParam_SizeType_ARRAY;
  static inline bool SizeType_IsValid(int value) {
    return GenericParam_SizeType_IsValid(value);
  }
  static const SizeType SizeType_MIN =
    GenericParam_SizeType_SizeType_MIN;
  static const SizeType SizeType_MAX =
    GenericParam_SizeType_SizeType_MAX;
  static const int SizeType_ARRAYSIZE =
    GenericParam_SizeType_SizeType_ARRAYSIZE;

  // accessors -------------------------------------------------------

  // optional .GenericParam.DataType data_type = 1 [default = INT];
  inline bool has_data_type() const;
  inline void clear_data_type();
  static const int kDataTypeFieldNumber = 1;
  inline ::GenericParam_DataType data_type() const;
  inline void set_data_type(::GenericParam_DataType value);

  // optional .GenericParam.SizeType size_type = 2 [default = SCALAR];
  inline bool has_size_type() const;
  inline void clear_size_type();
  static const int kSizeTypeFieldNumber = 2;
  inline ::GenericParam_SizeType size_type() const;
  inline void set_size_type(::GenericParam_SizeType value);

  // optional uint32 val_uint32 = 3 [default = 0];
  inline bool has_val_uint32() const;
  inline void clear_val_uint32();
  static const int kValUint32FieldNumber = 3;
  inline ::google::protobuf::uint32 val_uint32() const;
  inline void set_val_uint32(::google::protobuf::uint32 value);

  // repeated uint32 array_uint32 = 4 [packed = true];
  inline int array_uint32_size() const;
  inline void clear_array_uint32();
  static const int kArrayUint32FieldNumber = 4;
  inline ::google::protobuf::uint32 array_uint32(int index) const;
  inline void set_array_uint32(int index, ::google::protobuf::uint32 value);
  inline void add_array_uint32(::google::protobuf::uint32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      array_uint32() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_array_uint32();

  // optional int32 val_int32 = 5 [default = 0];
  inline bool has_val_int32() const;
  inline void clear_val_int32();
  static const int kValInt32FieldNumber = 5;
  inline ::google::protobuf::int32 val_int32() const;
  inline void set_val_int32(::google::protobuf::int32 value);

  // repeated int32 array_int32 = 6 [packed = true];
  inline int array_int32_size() const;
  inline void clear_array_int32();
  static const int kArrayInt32FieldNumber = 6;
  inline ::google::protobuf::int32 array_int32(int index) const;
  inline void set_array_int32(int index, ::google::protobuf::int32 value);
  inline void add_array_int32(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      array_int32() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_array_int32();

  // optional double val_double = 7 [default = 0];
  inline bool has_val_double() const;
  inline void clear_val_double();
  static const int kValDoubleFieldNumber = 7;
  inline double val_double() const;
  inline void set_val_double(double value);

  // repeated double array_double = 8 [packed = true];
  inline int array_double_size() const;
  inline void clear_array_double();
  static const int kArrayDoubleFieldNumber = 8;
  inline double array_double(int index) const;
  inline void set_array_double(int index, double value);
  inline void add_array_double(double value);
  inline const ::google::protobuf::RepeatedField< double >&
      array_double() const;
  inline ::google::protobuf::RepeatedField< double >*
      mutable_array_double();

  // optional string val_string = 9 [default = ""];
  inline bool has_val_string() const;
  inline void clear_val_string();
  static const int kValStringFieldNumber = 9;
  inline const ::std::string& val_string() const;
  inline void set_val_string(const ::std::string& value);
  inline void set_val_string(const char* value);
  inline void set_val_string(const char* value, size_t size);
  inline ::std::string* mutable_val_string();
  inline ::std::string* release_val_string();
  inline void set_allocated_val_string(::std::string* val_string);

  // repeated string array_string = 10;
  inline int array_string_size() const;
  inline void clear_array_string();
  static const int kArrayStringFieldNumber = 10;
  inline const ::std::string& array_string(int index) const;
  inline ::std::string* mutable_array_string(int index);
  inline void set_array_string(int index, const ::std::string& value);
  inline void set_array_string(int index, const char* value);
  inline void set_array_string(int index, const char* value, size_t size);
  inline ::std::string* add_array_string();
  inline void add_array_string(const ::std::string& value);
  inline void add_array_string(const char* value);
  inline void add_array_string(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& array_string() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_array_string();

  // @@protoc_insertion_point(class_scope:GenericParam)
 private:
  inline void set_has_data_type();
  inline void clear_has_data_type();
  inline void set_has_size_type();
  inline void clear_has_size_type();
  inline void set_has_val_uint32();
  inline void clear_has_val_uint32();
  inline void set_has_val_int32();
  inline void clear_has_val_int32();
  inline void set_has_val_double();
  inline void clear_has_val_double();
  inline void set_has_val_string();
  inline void clear_has_val_string();

  int data_type_;
  int size_type_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > array_uint32_;
  mutable int _array_uint32_cached_byte_size_;
  ::google::protobuf::uint32 val_uint32_;
  ::google::protobuf::int32 val_int32_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > array_int32_;
  mutable int _array_int32_cached_byte_size_;
  double val_double_;
  ::google::protobuf::RepeatedField< double > array_double_;
  mutable int _array_double_cached_byte_size_;
  ::std::string* val_string_;
  ::google::protobuf::RepeatedPtrField< ::std::string> array_string_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(10 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_TLib_2ftool_2ftsystem_5fgeneric_5fparam_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_TLib_2ftool_2ftsystem_5fgeneric_5fparam_2eproto();
  #endif
  friend void protobuf_AssignDesc_TLib_2ftool_2ftsystem_5fgeneric_5fparam_2eproto();
  friend void protobuf_ShutdownFile_TLib_2ftool_2ftsystem_5fgeneric_5fparam_2eproto();

  void InitAsDefaultInstance();
  static GenericParam* default_instance_;
};
// ===================================================================


// ===================================================================

// GenericParam

// optional .GenericParam.DataType data_type = 1 [default = INT];
inline bool GenericParam::has_data_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GenericParam::set_has_data_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GenericParam::clear_has_data_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GenericParam::clear_data_type() {
  data_type_ = 1;
  clear_has_data_type();
}
inline ::GenericParam_DataType GenericParam::data_type() const {
  return static_cast< ::GenericParam_DataType >(data_type_);
}
inline void GenericParam::set_data_type(::GenericParam_DataType value) {
  assert(::GenericParam_DataType_IsValid(value));
  set_has_data_type();
  data_type_ = value;
}

// optional .GenericParam.SizeType size_type = 2 [default = SCALAR];
inline bool GenericParam::has_size_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GenericParam::set_has_size_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GenericParam::clear_has_size_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GenericParam::clear_size_type() {
  size_type_ = 0;
  clear_has_size_type();
}
inline ::GenericParam_SizeType GenericParam::size_type() const {
  return static_cast< ::GenericParam_SizeType >(size_type_);
}
inline void GenericParam::set_size_type(::GenericParam_SizeType value) {
  assert(::GenericParam_SizeType_IsValid(value));
  set_has_size_type();
  size_type_ = value;
}

// optional uint32 val_uint32 = 3 [default = 0];
inline bool GenericParam::has_val_uint32() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GenericParam::set_has_val_uint32() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GenericParam::clear_has_val_uint32() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GenericParam::clear_val_uint32() {
  val_uint32_ = 0u;
  clear_has_val_uint32();
}
inline ::google::protobuf::uint32 GenericParam::val_uint32() const {
  return val_uint32_;
}
inline void GenericParam::set_val_uint32(::google::protobuf::uint32 value) {
  set_has_val_uint32();
  val_uint32_ = value;
}

// repeated uint32 array_uint32 = 4 [packed = true];
inline int GenericParam::array_uint32_size() const {
  return array_uint32_.size();
}
inline void GenericParam::clear_array_uint32() {
  array_uint32_.Clear();
}
inline ::google::protobuf::uint32 GenericParam::array_uint32(int index) const {
  return array_uint32_.Get(index);
}
inline void GenericParam::set_array_uint32(int index, ::google::protobuf::uint32 value) {
  array_uint32_.Set(index, value);
}
inline void GenericParam::add_array_uint32(::google::protobuf::uint32 value) {
  array_uint32_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
GenericParam::array_uint32() const {
  return array_uint32_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
GenericParam::mutable_array_uint32() {
  return &array_uint32_;
}

// optional int32 val_int32 = 5 [default = 0];
inline bool GenericParam::has_val_int32() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void GenericParam::set_has_val_int32() {
  _has_bits_[0] |= 0x00000010u;
}
inline void GenericParam::clear_has_val_int32() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void GenericParam::clear_val_int32() {
  val_int32_ = 0;
  clear_has_val_int32();
}
inline ::google::protobuf::int32 GenericParam::val_int32() const {
  return val_int32_;
}
inline void GenericParam::set_val_int32(::google::protobuf::int32 value) {
  set_has_val_int32();
  val_int32_ = value;
}

// repeated int32 array_int32 = 6 [packed = true];
inline int GenericParam::array_int32_size() const {
  return array_int32_.size();
}
inline void GenericParam::clear_array_int32() {
  array_int32_.Clear();
}
inline ::google::protobuf::int32 GenericParam::array_int32(int index) const {
  return array_int32_.Get(index);
}
inline void GenericParam::set_array_int32(int index, ::google::protobuf::int32 value) {
  array_int32_.Set(index, value);
}
inline void GenericParam::add_array_int32(::google::protobuf::int32 value) {
  array_int32_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
GenericParam::array_int32() const {
  return array_int32_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
GenericParam::mutable_array_int32() {
  return &array_int32_;
}

// optional double val_double = 7 [default = 0];
inline bool GenericParam::has_val_double() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void GenericParam::set_has_val_double() {
  _has_bits_[0] |= 0x00000040u;
}
inline void GenericParam::clear_has_val_double() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void GenericParam::clear_val_double() {
  val_double_ = 0;
  clear_has_val_double();
}
inline double GenericParam::val_double() const {
  return val_double_;
}
inline void GenericParam::set_val_double(double value) {
  set_has_val_double();
  val_double_ = value;
}

// repeated double array_double = 8 [packed = true];
inline int GenericParam::array_double_size() const {
  return array_double_.size();
}
inline void GenericParam::clear_array_double() {
  array_double_.Clear();
}
inline double GenericParam::array_double(int index) const {
  return array_double_.Get(index);
}
inline void GenericParam::set_array_double(int index, double value) {
  array_double_.Set(index, value);
}
inline void GenericParam::add_array_double(double value) {
  array_double_.Add(value);
}
inline const ::google::protobuf::RepeatedField< double >&
GenericParam::array_double() const {
  return array_double_;
}
inline ::google::protobuf::RepeatedField< double >*
GenericParam::mutable_array_double() {
  return &array_double_;
}

// optional string val_string = 9 [default = ""];
inline bool GenericParam::has_val_string() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void GenericParam::set_has_val_string() {
  _has_bits_[0] |= 0x00000100u;
}
inline void GenericParam::clear_has_val_string() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void GenericParam::clear_val_string() {
  if (val_string_ != &::google::protobuf::internal::kEmptyString) {
    val_string_->clear();
  }
  clear_has_val_string();
}
inline const ::std::string& GenericParam::val_string() const {
  return *val_string_;
}
inline void GenericParam::set_val_string(const ::std::string& value) {
  set_has_val_string();
  if (val_string_ == &::google::protobuf::internal::kEmptyString) {
    val_string_ = new ::std::string;
  }
  val_string_->assign(value);
}
inline void GenericParam::set_val_string(const char* value) {
  set_has_val_string();
  if (val_string_ == &::google::protobuf::internal::kEmptyString) {
    val_string_ = new ::std::string;
  }
  val_string_->assign(value);
}
inline void GenericParam::set_val_string(const char* value, size_t size) {
  set_has_val_string();
  if (val_string_ == &::google::protobuf::internal::kEmptyString) {
    val_string_ = new ::std::string;
  }
  val_string_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* GenericParam::mutable_val_string() {
  set_has_val_string();
  if (val_string_ == &::google::protobuf::internal::kEmptyString) {
    val_string_ = new ::std::string;
  }
  return val_string_;
}
inline ::std::string* GenericParam::release_val_string() {
  clear_has_val_string();
  if (val_string_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = val_string_;
    val_string_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void GenericParam::set_allocated_val_string(::std::string* val_string) {
  if (val_string_ != &::google::protobuf::internal::kEmptyString) {
    delete val_string_;
  }
  if (val_string) {
    set_has_val_string();
    val_string_ = val_string;
  } else {
    clear_has_val_string();
    val_string_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// repeated string array_string = 10;
inline int GenericParam::array_string_size() const {
  return array_string_.size();
}
inline void GenericParam::clear_array_string() {
  array_string_.Clear();
}
inline const ::std::string& GenericParam::array_string(int index) const {
  return array_string_.Get(index);
}
inline ::std::string* GenericParam::mutable_array_string(int index) {
  return array_string_.Mutable(index);
}
inline void GenericParam::set_array_string(int index, const ::std::string& value) {
  array_string_.Mutable(index)->assign(value);
}
inline void GenericParam::set_array_string(int index, const char* value) {
  array_string_.Mutable(index)->assign(value);
}
inline void GenericParam::set_array_string(int index, const char* value, size_t size) {
  array_string_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* GenericParam::add_array_string() {
  return array_string_.Add();
}
inline void GenericParam::add_array_string(const ::std::string& value) {
  array_string_.Add()->assign(value);
}
inline void GenericParam::add_array_string(const char* value) {
  array_string_.Add()->assign(value);
}
inline void GenericParam::add_array_string(const char* value, size_t size) {
  array_string_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
GenericParam::array_string() const {
  return array_string_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
GenericParam::mutable_array_string() {
  return &array_string_;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_TLib_2ftool_2ftsystem_5fgeneric_5fparam_2eproto__INCLUDED
