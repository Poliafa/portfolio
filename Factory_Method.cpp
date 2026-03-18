#include <iostream>
#include <fstream>
#include <string>
#include <memory>

class Logger{

    public:
    virtual void log(const std::string& msg) = 0;

    virtual ~ Logger() = default;
};
//логгеры
class ConsoleLogger: public Logger{
    public:
    void log(const std::string& msg) override{
        std::cout << "[LOG]" << msg << std::endl;
    }
};

class FileLogger: public Logger{
    private:
    std::ofstream file_;

    public:
    FileLogger(const std::string& filename) : file_(filename) {}

    void log(const std::string& msg){
        file_ << "[LOG]" << msg << std::endl;
    }
};

class NullLogger: public Logger{

    public:
    void log(const std::string& msg){ }
};

//фабрика

class LoggerFactory{
    public:
    virtual std::unique_ptr<Logger> createLogger() = 0;
};

class ConsoleLoggerFactory: public LoggerFactory{
    public:
    std::unique_ptr<Logger> createLogger(){
    return std::make_unique<ConsoleLogger>();
    }
};

class FileLoggerFactory: public LoggerFactory{
    private:
    std::string filename_;

    public:
    FileLoggerFactory(const std::string& filename) : filename_(filename) {}


    std::unique_ptr<Logger> createLogger(){
    return std::make_unique<FileLogger>(filename_);
    }
};

class NullLoggerFactory: public LoggerFactory{
    public:
    std::unique_ptr<Logger> createLogger() override {
        return std::make_unique<NullLogger>();
    }
};


class Application{
     std::unique_ptr<LoggerFactory> factory_;
    public:
     Application(std::unique_ptr<LoggerFactory> f) : factory_(std::move(f)) {}

     void run(){
        auto logger = factory_->createLogger();
        logger->log("Application started"); 
     }
    
};

int main(){
Application app1(std::make_unique<ConsoleLoggerFactory>());
app1.run();

Application app2(std::make_unique<FileLoggerFactory>("log.txt"));
app2.run();


Application app3(std::make_unique<NullLoggerFactory>());
app3.run();
}