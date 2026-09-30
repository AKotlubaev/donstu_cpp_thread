#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include "threadfuncs.h"
#include <future>
#include <mutex>
#include <condition_variable>

std::mutex g_cv_mutex;
std::condition_variable g_cv;
int g_data_buffer = 0;
bool g_ready = false;     //  есть новые данные для чтения
bool g_processed = false; 

void producer(Logger& logger) {
    for (int i = 1; i <= 10; ++i) {
        {
            std::unique_lock<std::mutex> lock(g_cv_mutex);
            
            
            g_cv.wait(lock, [] { return !g_ready; });

            g_data_buffer = i;
            g_ready = true;
            logger.writeLine("Producer: sent value " + std::to_string(i));
        }
       
        g_cv.notify_one();
    }
}

void consumer(Logger& logger) {
    for (int i = 1; i <= 10; ++i) {
        std::unique_lock<std::mutex> lock(g_cv_mutex);

      
        g_cv.wait(lock, [] { return g_ready; });

        logger.writeLine("Consumer: received value " + std::to_string(g_data_buffer));
        std::cout << "Consumer received: " << g_data_buffer << std::endl;

        g_ready = false;
        
        
        lock.unlock();
        g_cv.notify_one();
    }
}

int main() {
  about();

  // Open log file
  Logger logger("output.log");

  std::cout << "main: pid = " << getThreadID()
            << ", opened file: 'output.log'\n";

    std::thread prod(producer, std::ref(logger));
    std::thread cons(consumer, std::ref(logger));

    prod.join();
    cons.join();

  // close file automatically
  logger.writeLine("main: all threads finished, file closed\n");
  std::cout << "main: all done!\n";
  return 0;
}
