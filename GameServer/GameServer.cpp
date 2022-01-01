#include "pch.h"
#include <thread>
#include <atomic>
#include <mutex>

//Lock 구현 이론
/*
* 1. 존버메타 - SpinLock
* 2. 랜덤메타 - 
* 3. 갑질메타 - 
*/

int main()
{
	thread t1(Func1);
	thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Jobs Done" << endl;

	// 참고
	mutex m1;
	mutex m2;

	std::lock(m1, m2); // 알아서 내부적으로 일괄적인 순서를 가지고 락을 잡음

	// adopt_lock : 이미 lock된 상태니까, 나중에 소멸될때 풀어주기만 해
	lock_guard<mutex> g1(m1, std::adopt_lock);
	lock_guard<mutex> g2(m2, std::adopt_lock);
}