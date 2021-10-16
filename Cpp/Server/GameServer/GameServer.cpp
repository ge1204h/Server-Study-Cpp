#include "pch.h"
#include <thread>
#include <atomic>

// atomic : All-Or-Nothing
atomic<int32> sum = 0;
void Add()
{
	for (int32 i = 0; i < 100'0000; i++)
	{
		sum.fetch_add(1);
		//sum++;
		// atomic을 사용하지 않은 상황에서의 레지스터 의사 코드
		//int32 eax = sum;
		//eax = eax + 1;
		//sum = eax;
	}
}
void Sub()
{
	for (int32 i = 0; i < 100'0000; i++)
	{
		sum.fetch_add(-1);
		//sum--;
		//int32 eax = sum;
		//eax = eax - 1;
		//sum = eax;
	}
}
int main()
{
	Add();
	Sub();
	cout << sum << endl;

	thread t1(Add);
	thread t2(Sub);
	t1.join();
	t2.join();
	cout << sum << endl;
}