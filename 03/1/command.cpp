#include <iostream>
#include <fstream>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) {
        std::cout << "[LOG] " << message << std::endl;
    }
};

class FileLogCommand : public LogCommand {
public:
    FileLogCommand(const std::string file_name) 
        : output(file_name) {
    }

    void print(const std::string& message) {
        output << "[LOG] " << message << std::endl; 
    }

private:
    std::ofstream output;
};

void print(LogCommand& cmd, const std::string msg) {
    cmd.print(msg);
}


int main(int argc, char *argv[]) {
    FileLogCommand fcmd("tmp.txt");
    ConsoleLogCommand ccmd;
    print(ccmd, "hello, console");
    print(fcmd, "hello, file");
     
    return 0;
}
