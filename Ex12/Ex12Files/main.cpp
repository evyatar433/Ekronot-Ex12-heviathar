#include "threads.h"



int main()
{
	//call_I_Love_Threads();

	std::vector<int> primes1;


	callWritePrimesMultipleThreads(1, 100000, "C:/Users/Cyber_User/OneDrive/Magshimim/c++/Ekronot-Ex12-heviathar/Ex12/Ex12Files/primes2.txt", 2);

	system("pause");
	return 0;
}