// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TLib/tool/tsystem_heartbeat_msg.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "TLib/tool/tsystem_heartbeat_msg.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

void protobuf_ShutdownFile_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto() {
  delete Heartbeat::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  ::protobuf_AddDesc_TLib_2fcore_2ftsystem_5ftime_2eproto();
  Heartbeat::default_instance_ = new Heartbeat();
  Heartbeat::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto_once_);
void protobuf_AddDesc_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto() {
  ::google::protobuf::::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto_once_,
                 &protobuf_AddDesc_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto {
  StaticDescriptorInitializer_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto() {
    protobuf_AddDesc_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto();
  }
} static_descriptor_initializer_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto_;
#endif

// ===================================================================

bool Heartbeat_Status_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const Heartbeat_Status Heartbeat::OFF_LINE;
const Heartbeat_Status Heartbeat::INITIATE;
const Heartbeat_Status Heartbeat::ON_LINE;
const Heartbeat_Status Heartbeat::PANIC;
const Heartbeat_Status Heartbeat::Status_MIN;
const Heartbeat_Status Heartbeat::Status_MAX;
const int Heartbeat::Status_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int Heartbeat::kPidFieldNumber;
const int Heartbeat::kTimestampFieldNumber;
const int Heartbeat::kStatusFieldNumber;
const int Heartbeat::kInfoFieldNumber;
#endif  // !_MSC_VER

Heartbeat::Heartbeat()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void Heartbeat::InitAsDefaultInstance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  timestamp_ = const_cast< ::Time*>(
      ::Time::internal_default_instance());
#else
  timestamp_ = const_cast< ::Time*>(&::Time::default_instance());
#endif
}

Heartbeat::Heartbeat(const Heartbeat& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void Heartbeat::SharedCtor() {
  _cached_size_ = 0;
  pid_ = 0u;
  timestamp_ = NULL;
  status_ = 0;
  info_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Heartbeat::~Heartbeat() {
  SharedDtor();
}

void Heartbeat::SharedDtor() {
  if (info_ != &::google::protobuf::internal::kEmptyString) {
    delete info_;
  }
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
    delete timestamp_;
  }
}

void Heartbeat::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const Heartbeat& Heartbeat::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_TLib_2ftool_2ftsystem_5fheartbeat_5fmsg_2eproto();
#endif
  return *default_instance_;
}

Heartbeat* Heartbeat::default_instance_ = NULL;

Heartbeat* Heartbeat::New() const {
  return new Heartbeat;
}

void Heartbeat::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    pid_ = 0u;
    if (has_timestamp()) {
      if (timestamp_ != NULL) timestamp_->::Time::Clear();
    }
    status_ = 0;
    if (has_info()) {
      if (info_ != &::google::protobuf::internal::kEmptyString) {
        info_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool Heartbeat::MergePartialFromCodedStream(
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
        if (input->ExpectTag(18)) goto parse_timestamp;
        break;
      }

      // optional .Time timestamp = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_timestamp:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_timestamp()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_status;
        break;
      }

      // optional .Heartbeat.Status status = 3 [default = OFF_LINE];
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_status:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::Heartbeat_Status_IsValid(value)) {
            set_status(static_cast< ::Heartbeat_Status >(value));
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_info;
        break;
      }

      // optional string info = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_info:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_info()));
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

void Heartbeat::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 pid = 1;
  if (has_pid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->pid(), output);
  }

  // optional .Time timestamp = 2;
  if (has_timestamp()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      2, this->timestamp(), output);
  }

  // optional .Heartbeat.Status status = 3 [default = OFF_LINE];
  if (has_status()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      3, this->status(), output);
  }

  // optional string info = 4;
  if (has_info()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->info(), output);
  }

}

int Heartbeat::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 pid = 1;
    if (has_pid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->pid());
    }

    // optional .Time timestamp = 2;
    if (has_timestamp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->timestamp());
    }

    // optional .Heartbeat.Status status = 3 [default = OFF_LINE];
    if (has_status()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->status());
    }

    // optional string info = 4;
    if (has_info()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->info());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Heartbeat::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const Heartbeat*>(&from));
}

void Heartbeat::MergeFrom(const Heartbeat& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_pid()) {
      set_pid(from.pid());
    }
    if (from.has_timestamp()) {
      mutable_timestamp()->::Time::MergeFrom(from.timestamp());
    }
    if (from.has_status()) {
      set_status(from.status());
    }
    if (from.has_info()) {
      set_info(from.info());
    }
  }
}

void Heartbeat::CopyFrom(const Heartbeat& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Heartbeat::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void Heartbeat::Swap(Heartbeat* other) {
  if (other != this) {
    std::swap(pid_, other->pid_);
    std::swap(timestamp_, other->timestamp_);
    std::swap(status_, other->status_);
    std::swap(info_, other->info_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string Heartbeat::GetTypeName() const {
  return "Heartbeat";
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
