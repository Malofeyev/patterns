#include <iostream>
#include <fstream>
#include <memory>

class LogMessage {
public:
    enum class Type {
        WARNING,
        ERROR,
        FATAL_ERROR,
        UNDEFINED_MSG
    };
    LogMessage(Type type, std::string message) 
        : type_(type), message_(std::move(message)) {}

    Type type() const {
        return type_;
    }

    const std::string& message() const {
        return message_;
    }

private:
    Type type_;
    std::string message_;

};

class Handler {
public:
    Handler(std::unique_ptr<Handler> next_handler) 
        : next_handler(std::move(next_handler)) {}

    virtual void handle(const LogMessage& log) = 0;

    Handler* next() {
        return next_handler.get();
    }

    virtual ~Handler() = default;

private:
    std::unique_ptr<Handler> next_handler;
};


class UndefinedHandler : public Handler {
public:
    UndefinedHandler() : Handler(nullptr) {}

    void handle(const LogMessage& log) {
        throw std::runtime_error("Undefined message: " + log.message());
    }

};


class FatalErrorHandler : public Handler {
public:
    FatalErrorHandler() : Handler(std::make_unique<UndefinedHandler>()) {}

    void handle(const LogMessage& log) {
        if (log.type() == LogMessage::Type::FATAL_ERROR) {
            throw std::runtime_error("[Fatal Error] " + log.message());
        } else {
            this->next()->handle(log);
        }
    }
};

class ErrorHandler : public Handler {
public:
    ErrorHandler(const std::string& path) 
        : Handler(std::make_unique<FatalErrorHandler>())
        , log_file(path) {}

    void handle(const LogMessage& log) {
        if (log.type() == LogMessage::Type::ERROR) {
            log_file << "[ERROR] " << log.message() << std::endl;
        } else {
            this->next()->handle(log);
        }
    }
private:
    std::ofstream log_file;

};

class WarningHandler : public Handler {
public:
    WarningHandler(const std::string& path)
        : Handler(std::make_unique<ErrorHandler>(path)) {} 

    void handle(const LogMessage& log) {
        if (log.type() == LogMessage::Type::WARNING) {
            std::cout << "[WARNING] " << log.message() << std::endl;
        } else {
            this->next()->handle(log);
        }

    }
};

int main(int argc, char *argv[]) {
    WarningHandler handler("tmp.txt"); 
    handler.handle(LogMessage(LogMessage::Type::ERROR, "first"));
    handler.handle(LogMessage(LogMessage::Type::WARNING, "second"));
    handler.handle(LogMessage(LogMessage::Type::FATAL_ERROR, "third"));
    return 0;
}
