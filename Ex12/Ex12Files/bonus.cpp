//#include <iostream>
//#include <thread>
//#include <vector>
//
//// bonus 1
//
//// colorst in ascii from google
//const std::string colors[] = 
//{
//    "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m",
//    "\033[36m", "\033[91m", "\033[92m", "\033[93m", "\033[94m",
//    "\033[95m", "\033[96m", "\033[97m", "\033[90m", "\033[37m"
//};
//
//
//void printColor(int id) 
//{
//    std::string color = colors[id % 15]; 
//    for (int i = 0; i < 10; ++i) 
//    {
//        std::cout << color << "Color from Thread " << id + 1 << std::endl;
//    }
//}
//
//int main() 
//{
//    std::vector<std::thread> threads;
//
//
//    for (int i = 0; i < 15; ++i) 
//    {
//        threads.emplace_back(printColor, i);
//    }
//
//    for (auto& t : threads) 
//    {
//        t.join();
//    }
//
//
//
//
//
//    return 0;
//}
