#include "pch.h"
#include <thread>
#include <atomic>
#include <mutex>

//DeadLock
#include "AccountManager.h"
#include "UserManager.h"

void Func1()
{
	for (int32 i = 0; i < 100; i++)
	{
		UserManager::Instance()->ProcessSave();
	}
}

void Func2()
{
	for (int32 i = 0; i < 100; i++)
	{
		AccountManager::Instance()->ProcessLogin();
	}
}

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