#include <thread>
#include <vector>
#include <iostream>
#include <algorithm> 

int main(){
    const unsigned N = 1000;
    std::vector<int> out(N);


    unsigned num_threads = std::thread::hardware_concurrency(); // may be 0 if unknown
    if (num_threads == 0) num_threads = 4; // friendly fallback
    std::cout << "Hardware threads available (num_threads): " << num_threads << "\n";

    const unsigned chunk = (N + num_threads - 1) /num_threads;

    std::vector<std::thread> workers;
    workers.reserve(num_threads);

    for (int n=0; n<num_threads; n++){
        int start = n * chunk;
        int end = std::min(N, start + chunk);
        workers.emplace_back([start, end, &out] {
            for (int i=start; i<end; i++){
                out[i] = i * i;
            }
        });
    }

    std::cout << "std::thread workers started: " << workers.size() << "\n";

    for (auto& w : workers) w.join(); // Main thread waits for "w" to finish

}


brew install llvm libomp