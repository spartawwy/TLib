// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TLib/tool/tsystem_connection_handshake.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "TLib/tool/tsystem_connection_handshake.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

void protobuf_ShutdownFile_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto() {
  delete HandShake::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  HandShake::default_instance_ = new HandShake();
  HandShake::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto_once_);
void protobuf_AddDesc_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto() {
  ::google::protobuf::::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto_once_,
                 &protobuf_AddDesc_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto {
  StaticDescriptorInitializer_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto() {
    protobuf_AddDesc_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto();
  }
} static_descriptor_initializer_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto_;
#endif

// ===================================================================

#ifndef _MSC_VER
const int HandShake::kPidFieldNumber;
#endif  // !_MSC_VER

HandShake::HandShake()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void HandShake::InitAsDefaultInstance() {
}

HandShake::HandShake(const HandShake& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void HandShake::SharedCtor() {
  _cached_size_ = 0;
  pid_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

HandShake::~HandShake() {
  SharedDtor();
}

void HandShake::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void HandShake::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const HandShake& HandShake::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_TLib_2ftool_2ftsystem_5fconnection_5fhandshake_2eproto();
#endif
  return *default_instance_;
}

HandShake* HandShake::default_instance_ = NULL;

HandShake* HandShake::New() const {
  return new HandShake;
}

void HandShake::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    pid_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool HandShake::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 pid = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &pid_)));
          set_has_pid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void HandShake::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 pid = 1;
  if (has_pid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->pid(), output);
  }

}

int HandShake::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 pid = 1;
    if (has_pid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->pid());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void HandShake::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const HandShake*>(&from));
}

void HandShake::MergeFrom(const HandShake& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_pid()) {
      set_pid(from.pid());
    }
  }
}

void HandShake::CopyFrom(const HandShake& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool HandShake::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void HandShake::Swap(HandShake* other) {
  if (other != this) {
    std::swap(pid_, other->pid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string HandShake::GetTypeName() const {
  return "HandShake";
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)