#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include "threadfuncs.h"
#include <future>

int main() {
  about();

  // Open log file
  Logger logger("output.log");

  std::cout << "main: pid = " << getThreadID()
            << ", opened file: 'output.log'\n";

  // args for thread
  ThreadArgs args[COUNT_THREADS];
  for(int i=0;i< COUNT_THREADS;++i){
	std::ostringstream oss;
	oss <<"T"<<i;
	args[i].id =i;
	args[i].tag =oss.str();
  }

  std::promise<std::string> prom;
    std::future<std::string> fut = prom.get_future();

    std::thread t(funcThreadWithPromise, std::cref(args[0]), std::ref(logger), std::move(prom));

    
    std::string result = fut.get();
    std::cout << "Результат из потока: " << result << std::endl;

   if (t.joinable()) {
        t.join();
    }

 
  

  // close file automatically
  logger.writeLine("main: all threads finished, file closed\n");
  return 0;
}
