#pragma once

#include <exception>
#include <string>

namespace model::exceptions {
    /**
     * Ocurre cuando hay un error al realizar una operación con una base de datos SQL usando
     * SQLite.
     */
    class SQLiteException : std::exception {
        private:
            std::string msg;

        public:
            explicit SQLiteException(const std::string& msg) : msg(msg) {}

            const char* what() const noexcept override {
                return msg.c_str();
            }
    };
}