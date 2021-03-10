// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: messages_robocup_ssl_refbox_log.proto

#include "messages_robocup_ssl_refbox_log.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
constexpr Log_Frame::Log_Frame(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : refbox_cmd_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , frame_(nullptr){}
struct Log_FrameDefaultTypeInternal {
  constexpr Log_FrameDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~Log_FrameDefaultTypeInternal() {}
  union {
    Log_Frame _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT Log_FrameDefaultTypeInternal _Log_Frame_default_instance_;
constexpr Refbox_Log::Refbox_Log(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : log_(){}
struct Refbox_LogDefaultTypeInternal {
  constexpr Refbox_LogDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~Refbox_LogDefaultTypeInternal() {}
  union {
    Refbox_Log _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT Refbox_LogDefaultTypeInternal _Refbox_Log_default_instance_;
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_messages_5frobocup_5fssl_5frefbox_5flog_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_messages_5frobocup_5fssl_5frefbox_5flog_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_messages_5frobocup_5fssl_5frefbox_5flog_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_messages_5frobocup_5fssl_5frefbox_5flog_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::Log_Frame, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::Log_Frame, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::Log_Frame, frame_),
  PROTOBUF_FIELD_OFFSET(::Log_Frame, refbox_cmd_),
  1,
  0,
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Refbox_Log, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::Refbox_Log, log_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 7, sizeof(::Log_Frame)},
  { 9, -1, sizeof(::Refbox_Log)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::_Log_Frame_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::_Refbox_Log_default_instance_),
};

const char descriptor_table_protodef_messages_5frobocup_5fssl_5frefbox_5flog_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n%messages_robocup_ssl_refbox_log.proto\032"
  "$messages_robocup_ssl_detection.proto\"C\n"
  "\tLog_Frame\022\"\n\005frame\030\001 \002(\0132\023.SSL_Detectio"
  "nFrame\022\022\n\nrefbox_cmd\030\002 \002(\t\"%\n\nRefbox_Log"
  "\022\027\n\003log\030\001 \003(\0132\n.Log_Frame"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_messages_5frobocup_5fssl_5frefbox_5flog_2eproto_deps[1] = {
  &::descriptor_table_messages_5frobocup_5fssl_5fdetection_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_messages_5frobocup_5fssl_5frefbox_5flog_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_messages_5frobocup_5fssl_5frefbox_5flog_2eproto = {
  false, false, 185, descriptor_table_protodef_messages_5frobocup_5fssl_5frefbox_5flog_2eproto, "messages_robocup_ssl_refbox_log.proto", 
  &descriptor_table_messages_5frobocup_5fssl_5frefbox_5flog_2eproto_once, descriptor_table_messages_5frobocup_5fssl_5frefbox_5flog_2eproto_deps, 1, 2,
  schemas, file_default_instances, TableStruct_messages_5frobocup_5fssl_5frefbox_5flog_2eproto::offsets,
  file_level_metadata_messages_5frobocup_5fssl_5frefbox_5flog_2eproto, file_level_enum_descriptors_messages_5frobocup_5fssl_5frefbox_5flog_2eproto, file_level_service_descriptors_messages_5frobocup_5fssl_5frefbox_5flog_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK ::PROTOBUF_NAMESPACE_ID::Metadata
descriptor_table_messages_5frobocup_5fssl_5frefbox_5flog_2eproto_metadata_getter(int index) {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_messages_5frobocup_5fssl_5frefbox_5flog_2eproto);
  return descriptor_table_messages_5frobocup_5fssl_5frefbox_5flog_2eproto.file_level_metadata[index];
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_messages_5frobocup_5fssl_5frefbox_5flog_2eproto(&descriptor_table_messages_5frobocup_5fssl_5frefbox_5flog_2eproto);

// ===================================================================

class Log_Frame::_Internal {
 public:
  using HasBits = decltype(std::declval<Log_Frame>()._has_bits_);
  static const ::SSL_DetectionFrame& frame(const Log_Frame* msg);
  static void set_has_frame(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_refbox_cmd(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000003) ^ 0x00000003) != 0;
  }
};

const ::SSL_DetectionFrame&
Log_Frame::_Internal::frame(const Log_Frame* msg) {
  return *msg->frame_;
}
void Log_Frame::clear_frame() {
  if (frame_ != nullptr) frame_->Clear();
  _has_bits_[0] &= ~0x00000002u;
}
Log_Frame::Log_Frame(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:Log_Frame)
}
Log_Frame::Log_Frame(const Log_Frame& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  refbox_cmd_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_refbox_cmd()) {
    refbox_cmd_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_refbox_cmd(), 
      GetArena());
  }
  if (from._internal_has_frame()) {
    frame_ = new ::SSL_DetectionFrame(*from.frame_);
  } else {
    frame_ = nullptr;
  }
  // @@protoc_insertion_point(copy_constructor:Log_Frame)
}

void Log_Frame::SharedCtor() {
refbox_cmd_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
frame_ = nullptr;
}

Log_Frame::~Log_Frame() {
  // @@protoc_insertion_point(destructor:Log_Frame)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Log_Frame::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  refbox_cmd_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete frame_;
}

void Log_Frame::ArenaDtor(void* object) {
  Log_Frame* _this = reinterpret_cast< Log_Frame* >(object);
  (void)_this;
}
void Log_Frame::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Log_Frame::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Log_Frame::Clear() {
// @@protoc_insertion_point(message_clear_start:Log_Frame)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      refbox_cmd_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000002u) {
      GOOGLE_DCHECK(frame_ != nullptr);
      frame_->Clear();
    }
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Log_Frame::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // required .SSL_DetectionFrame frame = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_frame(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string refbox_cmd = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_refbox_cmd();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          #ifndef NDEBUG
          ::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "Log_Frame.refbox_cmd");
          #endif  // !NDEBUG
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Log_Frame::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Log_Frame)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required .SSL_DetectionFrame frame = 1;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        1, _Internal::frame(this), target, stream);
  }

  // required string refbox_cmd = 2;
  if (cached_has_bits & 0x00000001u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_refbox_cmd().data(), static_cast<int>(this->_internal_refbox_cmd().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "Log_Frame.refbox_cmd");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_refbox_cmd(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Log_Frame)
  return target;
}

size_t Log_Frame::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:Log_Frame)
  size_t total_size = 0;

  if (_internal_has_refbox_cmd()) {
    // required string refbox_cmd = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_refbox_cmd());
  }

  if (_internal_has_frame()) {
    // required .SSL_DetectionFrame frame = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *frame_);
  }

  return total_size;
}
size_t Log_Frame::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Log_Frame)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x00000003) ^ 0x00000003) == 0) {  // All required fields are present.
    // required string refbox_cmd = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_refbox_cmd());

    // required .SSL_DetectionFrame frame = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *frame_);

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Log_Frame::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:Log_Frame)
  GOOGLE_DCHECK_NE(&from, this);
  const Log_Frame* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Log_Frame>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:Log_Frame)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:Log_Frame)
    MergeFrom(*source);
  }
}

void Log_Frame::MergeFrom(const Log_Frame& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Log_Frame)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      _internal_set_refbox_cmd(from._internal_refbox_cmd());
    }
    if (cached_has_bits & 0x00000002u) {
      _internal_mutable_frame()->::SSL_DetectionFrame::MergeFrom(from._internal_frame());
    }
  }
}

void Log_Frame::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:Log_Frame)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Log_Frame::CopyFrom(const Log_Frame& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Log_Frame)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Log_Frame::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_has_bits_)) return false;
  if (_internal_has_frame()) {
    if (!frame_->IsInitialized()) return false;
  }
  return true;
}

void Log_Frame::InternalSwap(Log_Frame* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  refbox_cmd_.Swap(&other->refbox_cmd_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  swap(frame_, other->frame_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Log_Frame::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

class Refbox_Log::_Internal {
 public:
};

Refbox_Log::Refbox_Log(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  log_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:Refbox_Log)
}
Refbox_Log::Refbox_Log(const Refbox_Log& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      log_(from.log_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:Refbox_Log)
}

void Refbox_Log::SharedCtor() {
}

Refbox_Log::~Refbox_Log() {
  // @@protoc_insertion_point(destructor:Refbox_Log)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Refbox_Log::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void Refbox_Log::ArenaDtor(void* object) {
  Refbox_Log* _this = reinterpret_cast< Refbox_Log* >(object);
  (void)_this;
}
void Refbox_Log::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Refbox_Log::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Refbox_Log::Clear() {
// @@protoc_insertion_point(message_clear_start:Refbox_Log)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  log_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Refbox_Log::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // repeated .Log_Frame log = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_log(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Refbox_Log::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Refbox_Log)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .Log_Frame log = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_log_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, this->_internal_log(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Refbox_Log)
  return target;
}

size_t Refbox_Log::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Refbox_Log)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .Log_Frame log = 1;
  total_size += 1UL * this->_internal_log_size();
  for (const auto& msg : this->log_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Refbox_Log::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:Refbox_Log)
  GOOGLE_DCHECK_NE(&from, this);
  const Refbox_Log* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Refbox_Log>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:Refbox_Log)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:Refbox_Log)
    MergeFrom(*source);
  }
}

void Refbox_Log::MergeFrom(const Refbox_Log& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Refbox_Log)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  log_.MergeFrom(from.log_);
}

void Refbox_Log::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:Refbox_Log)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Refbox_Log::CopyFrom(const Refbox_Log& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Refbox_Log)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Refbox_Log::IsInitialized() const {
  if (!::PROTOBUF_NAMESPACE_ID::internal::AllAreInitialized(log_)) return false;
  return true;
}

void Refbox_Log::InternalSwap(Refbox_Log* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  log_.InternalSwap(&other->log_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Refbox_Log::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::Log_Frame* Arena::CreateMaybeMessage< ::Log_Frame >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Log_Frame >(arena);
}
template<> PROTOBUF_NOINLINE ::Refbox_Log* Arena::CreateMaybeMessage< ::Refbox_Log >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Refbox_Log >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
