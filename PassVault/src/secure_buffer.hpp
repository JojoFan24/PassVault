
#pragma once

#include <cstddef>
#include <string_view>

namespace passvault {

class SecureBuffer {
public:
    explicit SecureBuffer(std::size_t size);
    ~SecureBuffer();
    
    // Ban copies
    SecureBuffer(const SecureBuffer&) = delete;
    SecureBuffer& operator=(const SecureBuffer&) = delete;
    
    SecureBuffer(SecureBuffer&& other) noexcept;
    SecureBuffer& operator=(SecureBuffer&& other) noexcept;
    
    unsigned char* data() noexcept { return data_; }
    const unsigned char* data() const noexcept { return data_; }
    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    
    char* chars() noexcept;
    std::string_view view() const noexcept;
    
private:
    unsigned char* data_ = nullptr;
    std::size_t size_ = 0;
};

} // namespace passvault
