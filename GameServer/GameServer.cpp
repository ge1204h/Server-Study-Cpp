#include "pch.h"
#include <thread>
#include <atomic>
#include <mutex>

//Lock 구현 이론
/*
* 1. 존버메타 - SpinLock
* 2. 랜덤메타 - Sleep
* 3. 갑질메타 - Event
*/

// SpinLock
class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare-And-Swap)

		bool expected = false; // _locked 최초 값이길 기대하는 값
		bool desired = true; // _locked 바뀌길 원하는 값

		// CAS 의사 코드
		//if (_locked == expected)
		//{
		//	expected = _locked;
		//	_locked = desired;
		//	return true;
		//}
		//else
		//{
		//	expected = _locked;
		//	return false;
		//}

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
		}
	}

	void unlock()
	{
		//_locked = false;
		_locked.store(false);
	}

private:
	// volatile
	// cpp: 컴파일러한테 최적화를 하지 말아달라고함
	// cshprp: 컴파일러 최적화 & 메모리 베리어 & 가시성
	//volatile bool _locked = false;
	
	atomic<bool> _locked = false;
};

int32 sum = 0;
mutex m;

SpinLock spinLock;

void Add()
{
	for (int32 i = 0; i < 10'0000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 10'0000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{
	// volatile 유무에 따라서 어셈블리 처리가 달라진다.
	//volatile int32 a = 0;
	//a = 1;
	//a = 2;
	//a = 3;
	//a = 4;
	//cout << a << endl;

	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}