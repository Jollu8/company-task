//
// Created by Jollu Emil .
//

#pragma once


#include <fmt/core.h>  // Для fmt::format

namespace http {

class Error {
public:
    explicit Error(std::string const& what) : what_(what) {}

    [[nodiscard]] std::string const& what() const { return what_; }

    // Новый метод для форматирования сообщения об ошибке с использованием fmt::format
    template <typename... Args>
    static Error Format(const char* format, Args&&... args) {
        return Error(fmt::format(format, std::forward<Args>(args)...));
    }

private:
    std::string what_;
};

}  // namespace http

