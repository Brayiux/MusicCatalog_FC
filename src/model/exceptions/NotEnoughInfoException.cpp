#include <exception>
#include <string>

/**
 * Ocurre cuando no existe información suficiente dentro de un paquete.
 */
class NotEnoughInfoException : std::exception {
    private:
        std::string msg;

    public:
        explicit NotEnoughInfoException(const std::string& msg) : msg(msg) {}

        const char* what() const noexcept override {
            return msg.c_str();
        }
};
