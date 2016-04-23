#include <thread>
#include <hcc/hc.hpp>

using namespace hc;

class Adder {
 public:
  int *start_sig, *end_sig, *exit_sig;

  int x[256], y[256], z[256];

  array_view<int> d_start = array_view<int>(1, &start_sig);
  array_view<int> d_end = array_view<int>(1, &end_sig);
  array_view<int> d_exit = array_view<int>(1, &exit_sig);

  array_view<int> d_x = array_view<int>(256, x);
  array_view<int> d_y = array_view<int>(256, y);
  array_view<int> d_z = array_view<int>(256, z);

  std::thread kernel;

  Adder() {

    d_start[0] = 0;
    d_end[0] = 0;
    d_exit[0] = 0;
    std::cout << "Exit signal: " << d_exit[0] << "\n";

    for(int i = 0; i < 256; i++) {
      d_x[i] = 0;
    }
  }

  ~Adder() {
  }

  void start() {
    kernel = std::thread(&Adder::kernel_thread, d_x, d_exit);
  }

  static void kernel_thread(array_view<int> x, array_view<int> exit_sig) {
    printf("Starting kernel.\n");
    std::cout << "Exit signal: " << exit_sig[0] << "\n";
    parallel_for_each(extent<1>(256),
      [=](index<1> idx) [[hc]] {
        while(exit_sig[0] == 0) {
          x[idx]++;
        }
      });

    for(int i = 0; i < 256; i++) {
      std::cout << "x[" << i << "] = " << x[i] << "\n";
    }
    printf("Kernel ended.\n");
  }

  void exit() {
    std::cout << "Exiting kernel.";
    d_exit[0] = 1;
    kernel.join();
    d_exit[0] = 0;

    print_x();
  }

  void print_x() {
    d_x.synchronize();
    for(int i = 0; i < 256; i++) {
      std::cout << "x[" << i << "] = " << d_x[i] << "\n";
    }
  }

};
