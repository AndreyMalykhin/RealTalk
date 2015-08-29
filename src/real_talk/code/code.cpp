
#include <boost/numeric/conversion/cast.hpp>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
#include "real_talk/code/code.h"
#include "real_talk/util/endianness.h"
#include "real_talk/util/errors.h"

namespace real_talk {
namespace code {

static_assert(std::numeric_limits<double>::is_iec559,
              "IEEE 754 standard must be used for 'double'");
static_assert(sizeof(double) == 8, "Size of 'double' must be 8 bytes");

Code::Code(): size_(UINT32_C(0)),
              capacity_(UINT32_C(0)),
              bytes_(new unsigned char[capacity_]),
              current_byte_(bytes_.get()) {}

Code::Code(uint32_t size): size_(size),
                           capacity_(size),
                           bytes_(new unsigned char[size]),
                           current_byte_(bytes_.get()) {}

Code::Code(const unsigned char *bytes, uint32_t size)
    : size_(size),
      capacity_(size),
      bytes_(new unsigned char[size]),
      current_byte_(bytes_.get()) {
  assert(bytes);
  std::memcpy(bytes_.get(), bytes, size);
}

Code::Code(std::istream &stream) {
  stream.exceptions(std::ios::failbit | std::ios::badbit);

  try {
    stream.seekg(0, std::ios::end);
    std::streamoff stream_size = stream.tellg();

    if (stream_size > std::numeric_limits<uint32_t>::max()) {
      throw CodeSizeOverflowError("Code size exceeds 32 bits");
    }

    capacity_ = static_cast<uint32_t>(stream_size);
    bytes_.reset(new unsigned char[capacity_]);
    stream.seekg(0);
    stream.read(reinterpret_cast<char*>(bytes_.get()), capacity_);
    size_ = capacity_;
    current_byte_ = bytes_.get();
  } catch (const std::ios::failure &e) {
    throw real_talk::util::IOError(e.what());
  }
}

unsigned char *Code::GetData() noexcept {
  return bytes_.get();
}

const unsigned char *Code::GetData() const noexcept {
  return bytes_.get();
}

uint32_t Code::GetSize() const noexcept {
  return size_;
}

uint32_t Code::GetPosition() const noexcept {
  assert(current_byte_ - bytes_.get() >= 0);
  return static_cast<uint32_t>(current_byte_ - bytes_.get());
}

void Code::SetPosition(uint32_t position) noexcept {
  assert(position <= size_);
  current_byte_ = bytes_.get() + position;
}

void Code::MovePosition(int32_t offset) noexcept {
  assert((offset >= INT32_C(0)
          && static_cast<uint32_t>(offset) <= size_ - GetPosition())
         || (offset < INT32_C(0)
          && GetPosition() >= static_cast<uint32_t>(-offset)));
  current_byte_ += offset;
}

unsigned char *Code::GetDataAtPosition() noexcept {
  return current_byte_;
}

const unsigned char *Code::GetDataAtPosition() const noexcept {
  return current_byte_;
}

void Code::Skip(uint32_t bytes_count) {
  EnsureCapacity(bytes_count);
  AfterWrite(bytes_count);
}

template<> uint8_t Code::Read<uint8_t>() {
  assert(HasEnoughSize(sizeof(uint8_t)));
  const uint8_t value = *reinterpret_cast<uint8_t*>(current_byte_);
  current_byte_ += sizeof(value);
  return value;
}

template<> void Code::Write<uint8_t>(const uint8_t &value) {
  EnsureCapacity(sizeof(value));
  *reinterpret_cast<uint8_t*>(current_byte_) = value;
  AfterWrite(sizeof(value));
}

template<> uint32_t Code::Read<uint32_t>() {
  assert(HasEnoughSize(sizeof(uint32_t)));
  const uint32_t value = real_talk::util::FromLittleEndian32(
      *reinterpret_cast<uint32_t*>(current_byte_));
  current_byte_ += sizeof(value);
  return value;
}

template<> void Code::Write<uint32_t>(const uint32_t &value) {
  EnsureCapacity(sizeof(value));
  *reinterpret_cast<uint32_t*>(current_byte_) =
      real_talk::util::ToLittleEndian32(value);
  AfterWrite(sizeof(value));
}

template<> uint64_t Code::Read<uint64_t>() {
  assert(HasEnoughSize(sizeof(uint64_t)));
  const uint64_t value = real_talk::util::FromLittleEndian64(
      *reinterpret_cast<uint64_t*>(current_byte_));
  current_byte_ += sizeof(value);
  return value;
}

template<> void Code::Write<uint64_t>(const uint64_t &value) {
  EnsureCapacity(sizeof(value));
  *reinterpret_cast<uint64_t*>(current_byte_) =
      real_talk::util::ToLittleEndian64(value);
  AfterWrite(sizeof(value));
}

template<> int32_t Code::Read<int32_t>() {
  const uint32_t unsigned_value = Read<uint32_t>();
  return *reinterpret_cast<const int32_t*>(&unsigned_value);
}

template<> void Code::Write<int32_t>(const int32_t &value) {
  const uint32_t unsigned_value = *reinterpret_cast<const uint32_t*>(&value);
  Write<uint32_t>(unsigned_value);
}

template<> int64_t Code::Read<int64_t>() {
  const uint64_t unsigned_value = Read<uint64_t>();
  return *reinterpret_cast<const int64_t*>(&unsigned_value);
}

template<> void Code::Write<int64_t>(const int64_t &value) {
  const uint64_t unsigned_value = *reinterpret_cast<const uint64_t*>(&value);
  Write<uint64_t>(unsigned_value);
}

template<> double Code::Read<double>() {
  const uint64_t unsigned_value = Read<uint64_t>();
  const unsigned char *bytes =
      reinterpret_cast<const unsigned char*>(&unsigned_value);
  return *reinterpret_cast<const double*>(bytes);
}

template<> void Code::Write<double>(const double &value) {
  const unsigned char *bytes = reinterpret_cast<const unsigned char*>(&value);
  const uint64_t unsigned_value = *reinterpret_cast<const uint64_t*>(bytes);
  Write<uint64_t>(unsigned_value);
}

template<> bool Code::Read<bool>() {
  return static_cast<bool>(Read<uint8_t>());
}

template<> void Code::Write<bool>(const bool &value) {
  Write<uint8_t>(static_cast<uint8_t>(value));
}

template<> char Code::Read<char>() {
  const uint8_t c = Read<uint8_t>();
  assert(c <= std::numeric_limits<char>::max());
  return static_cast<char>(c);
}

template<> void Code::Write<char>(const char &value) {
  assert(value >= 0);
  assert(value <= std::numeric_limits<uint8_t>::max());
  Write<uint8_t>(static_cast<uint8_t>(value));
}

void Code::Write(const unsigned char *bytes, uint32_t count) {
  assert(bytes);
  EnsureCapacity(count);
  std::memcpy(current_byte_, bytes, count);
  AfterWrite(count);
}

template<> std::string Code::Read<std::string>() {
  const uint32_t size = Read<uint32_t>();
  assert(HasEnoughSize(size));
  const std::string str(reinterpret_cast<char*>(current_byte_), size);
  current_byte_ += size;
  return str;
}

template<> void Code::Write<std::string>(const std::string &str) {
  assert(str.size() <= std::numeric_limits<uint32_t>::max());
  const uint32_t size = static_cast<uint32_t>(str.size());
  Write<uint32_t>(size);
  Write(reinterpret_cast<const unsigned char*>(str.data()), size);
}

template<> CmdId Code::Read<CmdId>() {
  return static_cast<CmdId>(Read<uint8_t>());
}

template<> void Code::Write<CmdId>(const CmdId &id) {
  Write<uint8_t>(static_cast<uint8_t>(id));
}

template<> IdAddresses Code::Read<IdAddresses>() {
  const std::string &id = Read<std::string>();
  std::vector<uint32_t> addresses;
  const uint32_t size = Read<uint32_t>();
  addresses.reserve(size / sizeof(uint32_t));
  const unsigned char * const addresses_end = current_byte_ + size;

  while (current_byte_ != addresses_end) {
    addresses.push_back(Read<uint32_t>());
  }

  return IdAddresses(id, addresses);
}

template<> void Code::Write<IdAddresses>(const IdAddresses &id_addresses) {
  Write<std::string>(id_addresses.GetId());
  assert(id_addresses.GetAddresses().size()
         <= std::numeric_limits<uint32_t>::max());
  const uint32_t size = static_cast<uint32_t>(
      id_addresses.GetAddresses().size()) * sizeof(uint32_t);
  assert(size / sizeof(uint32_t) == id_addresses.GetAddresses().size());
  Write<uint32_t>(size);

  for (const uint32_t address: id_addresses.GetAddresses()) {
    Write<uint32_t>(address);
  }
}

template<> IdSize Code::Read<IdSize>() {
  const std::string &id = Read<std::string>();
  const DataTypeSize size = static_cast<DataTypeSize>(Read<uint8_t>());
  return IdSize(id, size);
}

template<> void Code::Write<IdSize>(const IdSize &id_size) {
  Write<std::string>(id_size.GetId());
  Write<uint8_t>(static_cast<uint8_t>(id_size.GetSize()));
}

template<> IdAddress Code::Read<IdAddress>() {
  const std::string &id = Read<std::string>();
  const uint32_t address = Read<uint32_t>();
  return IdAddress(id, address);
}

template<> void Code::Write<IdAddress>(const IdAddress &id_address) {
  Write<std::string>(id_address.GetId());
  Write<uint32_t>(id_address.GetAddress());
}

bool Code::HasEnoughCapacity(uint32_t bytes_count) const noexcept {
  return capacity_ - GetPosition() >= bytes_count;
}

bool Code::HasEnoughSize(uint32_t bytes_count) const noexcept {
  return size_ - GetPosition() >= bytes_count;
}

void Code::EnsureCapacity(uint32_t bytes_count) {
  if (HasEnoughCapacity(bytes_count)) {
    return;
  }

  const size_t new_capacity = (size_ + bytes_count) * 2U;

  if (new_capacity > std::numeric_limits<uint32_t>::max()
      || new_capacity <= capacity_) {
    throw CodeSizeOverflowError("Code size exceeds 32 bits");
  }

  const uint32_t position = GetPosition();
  std::unique_ptr<unsigned char[]> new_bytes_(new unsigned char[new_capacity]);
  std::memcpy(new_bytes_.get(), bytes_.get(), capacity_);
  capacity_ = static_cast<uint32_t>(new_capacity);
  bytes_ = move(new_bytes_);
  current_byte_ = bytes_.get() + position;
  assert(capacity_ >= size_ + bytes_count);
}

void Code::AfterWrite(uint32_t written_bytes_count) noexcept {
  assert(size_ >= GetPosition());
  const uint32_t available_size = size_ - GetPosition();

  if (written_bytes_count > available_size) {
    size_ += written_bytes_count - available_size;
  }

  assert(size_ <= capacity_);
  current_byte_ += written_bytes_count;
}

bool operator==(const Code &lhs, const Code &rhs) {
  return lhs.GetPosition() == rhs.GetPosition()
      && lhs.size_ == rhs.size_
      && std::memcmp(lhs.bytes_.get(), rhs.bytes_.get(), lhs.size_) == 0;
}

Code::CodeSizeOverflowError::CodeSizeOverflowError(
    const std::string &msg): runtime_error(msg) {
}
}
}
