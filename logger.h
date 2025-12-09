
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <string>

class Logger {
private:
    static std::unique_ptr<Logger> instance;
    static std::mutex init_mtx;
    static std::mutex write_mtx;
    std::ofstream logFile;

    Logger(const std::string& filename) {
        logFile.open(filename, std::ios::trunc);
        log("[Init] Logger started.");
    }


public:
    static Logger* getInstance(const std::string& filename = "Test/output2.txt") {
        if (!instance) {
            std::lock_guard<std::mutex> guard(init_mtx);
            if (!instance) {
                instance.reset(new Logger(filename));
            }
        }
        return instance.get();
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> guard(write_mtx);
        if(logFile.is_open()){
            logFile<<message<<std::endl;
        }
    }

    ~Logger(){
        log("[Shutdown] Logger closed.");
        if(logFile.is_open()){
            logFile.close();
        }
    }

};

std::unique_ptr<Logger> Logger::instance;
std::mutex Logger::init_mtx;
std::mutex Logger::write_mtx;

