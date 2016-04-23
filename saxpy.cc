#include <iostream>
#include <thread>
#include "adder.h"

int main() {
  
  Adder adder;
  
  adder.start();

  // Do something else
  for (int i = 0; i < 3; i++) {
    std::cout << "CPU Looping: " << i << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  adder.exit();
  
  for (int i = 0; i < 256; i++) {
    std::cout << adder.d_x[i] << std::endl;
  }
  
}
