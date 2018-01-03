// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TLib/core/tsystem_return_code.proto

#ifndef PROTOBUF_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto__INCLUDED
#define PROTOBUF_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto__INCLUDED

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
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();
void protobuf_AssignDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();
void protobuf_ShutdownFile_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();

class ReturnCode;
class ReturnPrint;
class RequestAck;

// ===================================================================

class ReturnCode : public ::google::protobuf::MessageLite {
 public:
  ReturnCode();
  virtual ~ReturnCode();

  ReturnCode(const ReturnCode& from);

  inline ReturnCode& operator=(const ReturnCode& from) {
    CopyFrom(from);
    return *this;
  }

  static const ReturnCode& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const ReturnCode* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(ReturnCode* other);

  // implements Message ----------------------------------------------

  ReturnCode* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const ReturnCode& from);
  void MergeFrom(const ReturnCode& from);
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

  // accessors -------------------------------------------------------

  // optional int32 code = 1 [default = 0];
  inline bool has_code() const;
  inline void clear_code();
  static const int kCodeFieldNumber = 1;
  inline ::google::protobuf::int32 code() const;
  inline void set_code(::google::protobuf::int32 value);

  // optional string error_msg = 2 [default = ""];
  inline bool has_error_msg() const;
  inline void clear_error_msg();
  static const int kErrorMsgFieldNumber = 2;
  inline const ::std::string& error_msg() const;
  inline void set_error_msg(const ::std::string& value);
  inline void set_error_msg(const char* value);
  inline void set_error_msg(const char* value, size_t size);
  inline ::std::string* mutable_error_msg();
  inline ::std::string* release_error_msg();
  inline void set_allocated_error_msg(::std::string* error_msg);

  // @@protoc_insertion_point(class_scope:ReturnCode)
 private:
  inline void set_has_code();
  inline void clear_has_code();
  inline void set_has_error_msg();
  inline void clear_has_error_msg();

  ::std::string* error_msg_;
  ::google::protobuf::int32 code_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();
  #endif
  friend void protobuf_AssignDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();
  friend void protobuf_ShutdownFile_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();

  void InitAsDefaultInstance();
  static ReturnCode* default_instance_;
};
// -------------------------------------------------------------------

class ReturnPrint : public ::google::protobuf::MessageLite {
 public:
  ReturnPrint();
  virtual ~ReturnPrint();

  ReturnPrint(const ReturnPrint& from);

  inline ReturnPrint& operator=(const ReturnPrint& from) {
    CopyFrom(from);
    return *this;
  }

  static const ReturnPrint& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const ReturnPrint* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(ReturnPrint* other);

  // implements Message ----------------------------------------------

  ReturnPrint* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const ReturnPrint& from);
  void MergeFrom(const ReturnPrint& from);
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

  // accessors -------------------------------------------------------

  // repeated string std_out = 1;
  inline int std_out_size() const;
  inline void clear_std_out();
  static const int kStdOutFieldNumber = 1;
  inline const ::std::string& std_out(int index) const;
  inline ::std::string* mutable_std_out(int index);
  inline void set_std_out(int index, const ::std::string& value);
  inline void set_std_out(int index, const char* value);
  inline void set_std_out(int index, const char* value, size_t size);
  inline ::std::string* add_std_out();
  inline void add_std_out(const ::std::string& value);
  inline void add_std_out(const char* value);
  inline void add_std_out(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& std_out() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_std_out();

  // repeated string std_err = 2;
  inline int std_err_size() const;
  inline void clear_std_err();
  static const int kStdErrFieldNumber = 2;
  inline const ::std::string& std_err(int index) const;
  inline ::std::string* mutable_std_err(int index);
  inline void set_std_err(int index, const ::std::string& value);
  inline void set_std_err(int index, const char* value);
  inline void set_std_err(int index, const char* value, size_t size);
  inline ::std::string* add_std_err();
  inline void add_std_err(const ::std::string& value);
  inline void add_std_err(const char* value);
  inline void add_std_err(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& std_err() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_std_err();

  // @@protoc_insertion_point(class_scope:ReturnPrint)
 private:

  ::google::protobuf::RepeatedPtrField< ::std::string> std_out_;
  ::google::protobuf::RepeatedPtrField< ::std::string> std_err_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();
  #endif
  friend void protobuf_AssignDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();
  friend void protobuf_ShutdownFile_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();

  void InitAsDefaultInstance();
  static ReturnPrint* default_instance_;
};
// -------------------------------------------------------------------

class RequestAck : public ::google::protobuf::MessageLite {
 public:
  RequestAck();
  virtual ~RequestAck();

  RequestAck(const RequestAck& from);

  inline RequestAck& operator=(const RequestAck& from) {
    CopyFrom(from);
    return *this;
  }

  static const RequestAck& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const RequestAck* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(RequestAck* other);

  // implements Message ----------------------------------------------

  RequestAck* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const RequestAck& from);
  void MergeFrom(const RequestAck& from);
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

  // accessors -------------------------------------------------------

  // optional uint32 request_id = 1 [default = 0];
  inline bool has_request_id() const;
  inline void clear_request_id();
  static const int kRequestIdFieldNumber = 1;
  inline ::google::protobuf::uint32 request_id() const;
  inline void set_request_id(::google::protobuf::uint32 value);

  // optional .ReturnCode result = 2;
  inline bool has_result() const;
  inline void clear_result();
  static const int kResultFieldNumber = 2;
  inline const ::ReturnCode& result() const;
  inline ::ReturnCode* mutable_result();
  inline ::ReturnCode* release_result();
  inline void set_allocated_result(::ReturnCode* result);

  // optional .ReturnPrint print = 3;
  inline bool has_print() const;
  inline void clear_print();
  static const int kPrintFieldNumber = 3;
  inline const ::ReturnPrint& print() const;
  inline ::ReturnPrint* mutable_print();
  inline ::ReturnPrint* release_print();
  inline void set_allocated_print(::ReturnPrint* print);

  // @@protoc_insertion_point(class_scope:RequestAck)
 private:
  inline void set_has_request_id();
  inline void clear_has_request_id();
  inline void set_has_result();
  inline void clear_has_result();
  inline void set_has_print();
  inline void clear_has_print();

  ::ReturnCode* result_;
  ::ReturnPrint* print_;
  ::google::protobuf::uint32 request_id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();
  #endif
  friend void protobuf_AssignDesc_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();
  friend void protobuf_ShutdownFile_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto();

  void InitAsDefaultInstance();
  static RequestAck* default_instance_;
};
// ===================================================================


// ===================================================================

// ReturnCode

// optional int32 code = 1 [default = 0];
inline bool ReturnCode::has_code() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReturnCode::set_has_code() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReturnCode::clear_has_code() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReturnCode::clear_code() {
  code_ = 0;
  clear_has_code();
}
inline ::google::protobuf::int32 ReturnCode::code() const {
  return code_;
}
inline void ReturnCode::set_code(::google::protobuf::int32 value) {
  set_has_code();
  code_ = value;
}

// optional string error_msg = 2 [default = ""];
inline bool ReturnCode::has_error_msg() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ReturnCode::set_has_error_msg() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ReturnCode::clear_has_error_msg() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ReturnCode::clear_error_msg() {
  if (error_msg_ != &::google::protobuf::internal::kEmptyString) {
    error_msg_->clear();
  }
  clear_has_error_msg();
}
inline const ::std::string& ReturnCode::error_msg() const {
  return *error_msg_;
}
inline void ReturnCode::set_error_msg(const ::std::string& value) {
  set_has_error_msg();
  if (error_msg_ == &::google::protobuf::internal::kEmptyString) {
    error_msg_ = new ::std::string;
  }
  error_msg_->assign(value);
}
inline void ReturnCode::set_error_msg(const char* value) {
  set_has_error_msg();
  if (error_msg_ == &::google::protobuf::internal::kEmptyString) {
    error_msg_ = new ::std::string;
  }
  error_msg_->assign(value);
}
inline void ReturnCode::set_error_msg(const char* value, size_t size) {
  set_has_error_msg();
  if (error_msg_ == &::google::protobuf::internal::kEmptyString) {
    error_msg_ = new ::std::string;
  }
  error_msg_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ReturnCode::mutable_error_msg() {
  set_has_error_msg();
  if (error_msg_ == &::google::protobuf::internal::kEmptyString) {
    error_msg_ = new ::std::string;
  }
  return error_msg_;
}
inline ::std::string* ReturnCode::release_error_msg() {
  clear_has_error_msg();
  if (error_msg_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = error_msg_;
    error_msg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ReturnCode::set_allocated_error_msg(::std::string* error_msg) {
  if (error_msg_ != &::google::protobuf::internal::kEmptyString) {
    delete error_msg_;
  }
  if (error_msg) {
    set_has_error_msg();
    error_msg_ = error_msg;
  } else {
    clear_has_error_msg();
    error_msg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// ReturnPrint

// repeated string std_out = 1;
inline int ReturnPrint::std_out_size() const {
  return std_out_.size();
}
inline void ReturnPrint::clear_std_out() {
  std_out_.Clear();
}
inline const ::std::string& ReturnPrint::std_out(int index) const {
  return std_out_.Get(index);
}
inline ::std::string* ReturnPrint::mutable_std_out(int index) {
  return std_out_.Mutable(index);
}
inline void ReturnPrint::set_std_out(int index, const ::std::string& value) {
  std_out_.Mutable(index)->assign(value);
}
inline void ReturnPrint::set_std_out(int index, const char* value) {
  std_out_.Mutable(index)->assign(value);
}
inline void ReturnPrint::set_std_out(int index, const char* value, size_t size) {
  std_out_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ReturnPrint::add_std_out() {
  return std_out_.Add();
}
inline void ReturnPrint::add_std_out(const ::std::string& value) {
  std_out_.Add()->assign(value);
}
inline void ReturnPrint::add_std_out(const char* value) {
  std_out_.Add()->assign(value);
}
inline void ReturnPrint::add_std_out(const char* value, size_t size) {
  std_out_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
ReturnPrint::std_out() const {
  return std_out_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
ReturnPrint::mutable_std_out() {
  return &std_out_;
}

// repeated string std_err = 2;
inline int ReturnPrint::std_err_size() const {
  return std_err_.size();
}
inline void ReturnPrint::clear_std_err() {
  std_err_.Clear();
}
inline const ::std::string& ReturnPrint::std_err(int index) const {
  return std_err_.Get(index);
}
inline ::std::string* ReturnPrint::mutable_std_err(int index) {
  return std_err_.Mutable(index);
}
inline void ReturnPrint::set_std_err(int index, const ::std::string& value) {
  std_err_.Mutable(index)->assign(value);
}
inline void ReturnPrint::set_std_err(int index, const char* value) {
  std_err_.Mutable(index)->assign(value);
}
inline void ReturnPrint::set_std_err(int index, const char* value, size_t size) {
  std_err_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ReturnPrint::add_std_err() {
  return std_err_.Add();
}
inline void ReturnPrint::add_std_err(const ::std::string& value) {
  std_err_.Add()->assign(value);
}
inline void ReturnPrint::add_std_err(const char* value) {
  std_err_.Add()->assign(value);
}
inline void ReturnPrint::add_std_err(const char* value, size_t size) {
  std_err_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
ReturnPrint::std_err() const {
  return std_err_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
ReturnPrint::mutable_std_err() {
  return &std_err_;
}

// -------------------------------------------------------------------

// RequestAck

// optional uint32 request_id = 1 [default = 0];
inline bool RequestAck::has_request_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RequestAck::set_has_request_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RequestAck::clear_has_request_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RequestAck::clear_request_id() {
  request_id_ = 0u;
  clear_has_request_id();
}
inline ::google::protobuf::uint32 RequestAck::request_id() const {
  return request_id_;
}
inline void RequestAck::set_request_id(::google::protobuf::uint32 value) {
  set_has_request_id();
  request_id_ = value;
}

// optional .ReturnCode result = 2;
inline bool RequestAck::has_result() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RequestAck::set_has_result() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RequestAck::clear_has_result() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RequestAck::clear_result() {
  if (result_ != NULL) result_->::ReturnCode::Clear();
  clear_has_result();
}
inline const ::ReturnCode& RequestAck::result() const {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  return result_ != NULL ? *result_ : *default_instance().result_;
#else
  return result_ != NULL ? *result_ : *default_instance_->result_;
#endif
}
inline ::ReturnCode* RequestAck::mutable_result() {
  set_has_result();
  if (result_ == NULL) result_ = new ::ReturnCode;
  return result_;
}
inline ::ReturnCode* RequestAck::release_result() {
  clear_has_result();
  ::ReturnCode* temp = result_;
  result_ = NULL;
  return temp;
}
inline void RequestAck::set_allocated_result(::ReturnCode* result) {
  delete result_;
  result_ = result;
  if (result) {
    set_has_result();
  } else {
    clear_has_result();
  }
}

// optional .ReturnPrint print = 3;
inline bool RequestAck::has_print() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RequestAck::set_has_print() {
  _has_bits_[0] |= 0x00000004u;
}
inline void RequestAck::clear_has_print() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void RequestAck::clear_print() {
  if (print_ != NULL) print_->::ReturnPrint::Clear();
  clear_has_print();
}
inline const ::ReturnPrint& RequestAck::print() const {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  return print_ != NULL ? *print_ : *default_instance().print_;
#else
  return print_ != NULL ? *print_ : *default_instance_->print_;
#endif
}
inline ::ReturnPrint* RequestAck::mutable_print() {
  set_has_print();
  if (print_ == NULL) print_ = new ::ReturnPrint;
  return print_;
}
inline ::ReturnPrint* RequestAck::release_print() {
  clear_has_print();
  ::ReturnPrint* temp = print_;
  print_ = NULL;
  return temp;
}
inline void RequestAck::set_allocated_print(::ReturnPrint* print) {
  delete print_;
  print_ = print;
  if (print) {
    set_has_print();
  } else {
    clear_has_print();
  }
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_TLib_2fcore_2ftsystem_5freturn_5fcode_2eproto__INCLUDED
