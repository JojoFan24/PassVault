#include "secure_buffer.hpp"

#include <sodium.h>

#include <new>
#include <utility>

namespace passvault {

SecureBuffer::SecureBuffer(std::size_t size) : size_(size) {
    if (size_ == 0) {
        return;
    }
    data_ = static_cast<unsigned char*>(sodium_malloc(size_));
    if (data_ == nullptr) {
        throw std::bad_alloc();
    }
}

SecureBuffer::~SecureBuffer() {
    sodium_free(data_);
}

SecureBuffer::SecureBuffer(SecureBuffer&& other) noexcept
    : data_(std::exchange(other.data_, nullptr)),
      size_(std::exchange(other.size_, 0)) {}

SecureBuffer& SecureBuffer::operator=(SecureBuffer&& other) noexcept {
    if (this != &other) {
        sodium_free(data_);
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
    }
    return *this;
}

char* SecureBuffer::chars() noexcept {
    return reinterpret_cast<char*>(data_);
}

std::string_view SecureBuffer::view() const noexcept {
    return {reinterpret_cast<const char*>(data_), size_};
}

}  // namespace passvault
