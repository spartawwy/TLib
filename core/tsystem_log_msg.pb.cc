// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TLib/core/tsystem_log_msg.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "TLib/core/tsystem_log_msg.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

void protobuf_ShutdownFile_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto() {
  delete LogMessage::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  ::protobuf_AddDesc_TLib_2fcore_2ftsystem_5ftime_2eproto();
  LogMessage::default_instance_ = new LogMessage();
  LogMessage::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto_once_);
void protobuf_AddDesc_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto() {
  ::google::protobuf::::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto_once_,
                 &protobuf_AddDesc_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto {
  StaticDescriptorInitializer_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto() {
    protobuf_AddDesc_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto();
  }
} static_descriptor_initializer_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto_;
#endif

// ===================================================================

bool LogMessage_WarnLevel_IsValid(int value) {
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
const LogMessage_WarnLevel LogMessage::LOG;
const LogMessage_WarnLevel LogMessage::TRIVIAL;
const LogMessage_WarnLevel LogMessage::VITAL;
const LogMessage_WarnLevel LogMessage::LETHAL;
const LogMessage_WarnLevel LogMessage::WarnLevel_MIN;
const LogMessage_WarnLevel LogMessage::WarnLevel_MAX;
const int LogMessage::WarnLevel_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int LogMessage::kPidFieldNumber;
const int LogMessage::kTimestampFieldNumber;
const int LogMessage::kLevelFieldNumber;
const int LogMessage::kKeyFieldNumber;
const int LogMessage::kMsgFieldNumber;
#endif  // !_MSC_VER

LogMessage::LogMessage()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void LogMessage::InitAsDefaultInstance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  timestamp_ = const_cast< ::Time*>(
      ::Time::internal_default_instance());
#else
  timestamp_ = const_cast< ::Time*>(&::Time::default_instance());
#endif
}

LogMessage::LogMessage(const LogMessage& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void LogMessage::SharedCtor() {
  _cached_size_ = 0;
  pid_ = 0u;
  timestamp_ = NULL;
  level_ = 0;
  key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  msg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LogMessage::~LogMessage() {
  SharedDtor();
}

void LogMessage::SharedDtor() {
  if (key_ != &::google::protobuf::internal::kEmptyString) {
    delete key_;
  }
  if (msg_ != &::google::protobuf::internal::kEmptyString) {
    delete msg_;
  }
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
    delete timestamp_;
  }
}

void LogMessage::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const LogMessage& LogMessage::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_TLib_2fcore_2ftsystem_5flog_5fmsg_2eproto();
#endif
  return *default_instance_;
}

LogMessage* LogMessage::default_instance_ = NULL;

LogMessage* LogMessage::New() const {
  return new LogMessage;
}

void LogMessage::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    pid_ = 0u;
    if (has_timestamp()) {
      if (timestamp_ != NULL) timestamp_->::Time::Clear();
    }
    level_ = 0;
    if (has_key()) {
      if (key_ != &::google::protobuf::internal::kEmptyString) {
        key_->clear();
      }
    }
    if (has_msg()) {
      if (msg_ != &::google::protobuf::internal::kEmptyString) {
        msg_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool LogMessage::MergePartialFromCodedStream(
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
        if (input->ExpectTag(24)) goto parse_level;
        break;
      }

      // optional .LogMessage.WarnLevel level = 3 [default = LOG];
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_level:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::LogMessage_WarnLevel_IsValid(value)) {
            set_level(static_cast< ::LogMessage_WarnLevel >(value));
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_key;
        break;
      }

      // required string key = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_key:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_key()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_msg;
        break;
      }

      // required string msg = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_msg:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_msg()));
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

void LogMessage::SerializeWithCachedSizes(
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

  // optional .LogMessage.WarnLevel level = 3 [default = LOG];
  if (has_level()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      3, this->level(), output);
  }

  // required string key = 4;
  if (has_key()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->key(), output);
  }

  // required string msg = 5;
  if (has_msg()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->msg(), output);
  }

}

int LogMessage::ByteSize() const {
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

    // optional .LogMessage.WarnLevel level = 3 [default = LOG];
    if (has_level()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->level());
    }

    // required string key = 4;
    if (has_key()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->key());
    }

    // required string msg = 5;
    if (has_msg()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->msg());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void LogMessage::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const LogMessage*>(&from));
}

void LogMessage::MergeFrom(const LogMessage& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_pid()) {
      set_pid(from.pid());
    }
    if (from.has_timestamp()) {
      mutable_timestamp()->::Time::MergeFrom(from.timestamp());
    }
    if (from.has_level()) {
      set_level(from.level());
    }
    if (from.has_key()) {
      set_key(from.key());
    }
    if (from.has_msg()) {
      set_msg(from.msg());
    }
  }
}

void LogMessage::CopyFrom(const LogMessage& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LogMessage::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000019) != 0x00000019) return false;

  return true;
}

void LogMessage::Swap(LogMessage* other) {
  if (other != this) {
    std::swap(pid_, other->pid_);
    std::swap(timestamp_, other->timestamp_);
    std::swap(level_, other->level_);
    std::swap(key_, other->key_);
    std::swap(msg_, other->msg_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string LogMessage::GetTypeName() const {
  return "LogMessage";
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
