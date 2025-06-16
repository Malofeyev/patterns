#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>

class Observer {
public:
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
    virtual ~Observer() = default;
};



class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) {
        std::cout << "[WARNING] " << message << std::endl;
    }
};

class ErrorObserver : public Observer {
public:
    ErrorObserver(const std::string& path) : log_file(path, std::ios::app) {}
    void onError(const std::string& message) {
        log_file << "[ERROR] " << message << std::endl;
    }
private:
    std::ofstream log_file;

};

class FatalErrorObserver : public Observer {
public:
    FatalErrorObserver(const std::string& path) : log_file(path, std::ios::app) {}
    void onFatalError(const std::string& message) {
        log_file << "[FATAL ERROR] " << message << std::endl;
        std::cout << "[FATAL ERROR] " << message << std::endl;
    }
private:
    std::ofstream log_file;
};

class System {
public:

    typedef uint64_t Id;

    bool remove_observer(Id id) {
        auto cnt = observers.erase(id);    
        return cnt != 0;
    }

    Id add_observer(const std::shared_ptr<Observer>& owner) {
       observers[++next_id] = owner; 
       return next_id;
    }



    void warning(const std::string& message) const {
        for (auto& elem : observers) {
            auto& observer = elem.second;
            if (!observer.expired()) {
                observer.lock()->onWarning(message);
            }
        }
    }

    void error(const std::string& message) const {
        for (auto& elem : observers) {
            auto& observer = elem.second;
            if (!observer.expired()) {
                observer.lock()->onError(message);
            }
        }
    }

    void fatalError(const std::string& message) const {
        for (auto& elem : observers) {
            auto& observer = elem.second;
            if (!observer.expired()) {
                observer.lock()->onFatalError(message);
            }
        }
    }

private:
    Id next_id = 0;

    std::unordered_map<Id, std::weak_ptr<Observer>> observers;

};

int main(int argc, char *argv[]) {
    auto warning = std::make_shared<WarningObserver>();
    auto error = std::make_shared<ErrorObserver>("tmp.txt");
    auto fatal = std::make_shared<FatalErrorObserver>("tmp.txt");
    System sys;
    sys.add_observer(warning);
    sys.add_observer(error);
    sys.add_observer(fatal);
    sys.warning("warning 1");    
    sys.warning("warning 2");    
    sys.error("error");
    sys.fatalError("fatal error");

    return 0;
}
