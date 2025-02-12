#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <mutex>

std::mutex fileMutex;

void writePrimesToFile(int start, int end, const std::string filePath)
{
    std::ofstream file(filePath, std::ios::app); 
    if (file.is_open())
    {
        for (int num = start; num <= end; ++num)
        {
            bool isPrime = true;
            if (num < 2) isPrime = false;
            for (int i = 2; i * i <= num; ++i)
            {
                if (num % i == 0)
                {
                    isPrime = false;
                    break;
                }
            }
            if (isPrime)
            {
                std::lock_guard<std::mutex> lock(fileMutex); // check only one thread writes at a time
                file << num << std::endl;
            }
        }
        file.close();
    }
}



void callWritePrimesMultipleThreads(int begin, int end, const std::string filePath, int N)
{
    std::vector<std::pair<int, int>> ranges;
    int step = (end - begin) / N;

    for (int i = 0; i < N; ++i)
    {
        int rangeStart = begin + i * step;
        int rangeEnd = (i == N - 1) ? end : begin + (i + 1) * step;  
        ranges.push_back({ rangeStart, rangeEnd });
    }

    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& range : ranges)
    {
        threads.push_back(std::thread(writePrimesToFile, range.first, range.second, filePath));
    }

    for (auto& t : threads)
    {
        t.join();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "Run time: " << duration.count() << " ms" << std::endl;
}
