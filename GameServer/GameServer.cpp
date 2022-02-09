#include "pch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

//Lock 구현 이론
/*
* 1. 존버메타 - SpinLock
* 2. 랜덤메타 - Sleep
* 3. 갑질메타 - Event
*/

// Auto Reset Event - 문이 자동으로 잠긴다.
// Manual Reset Event - 문이 수동으로 잠긴다.

mutex m;
queue<int32> q;
HANDLE handle;

void Producer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		q.push(100);

		::SetEvent(handle); // handle 커널 오브젝트를 시그널 상태로 바꿔주세요.
		this_thread::sleep_for(10000ms);
	}
}

void Consumer()
{
	while (true)
	{
		::WaitForSingleObject(handle, INFINITE); // Non-Signal 상태라면 쿨쿨 재움, 해당 스레드 실행하지 않음
		// ResetEvent(handle) : Signal 상태 실행 후 Non-Signal 상태로 변경 (Auto (자동))

		unique_lock<mutex> lock(m);
		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	// 커널 오브젝트(커널에서 관리하는 오브젝트). - 다른 프로그램끼리도 쓸 수 있음
	// HANDLE = 어떤 이벤트인지 구별하는 식별자
	// Usage Count = 이 오브젝트를 몇명이 사용하고 있는지
	// Signal (파란불) - 켜진상태 / Non-Signal (빨간불)-꺼진상태
	// Auto (자동리셋) / Manual (수동리셋)
	handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualReset*/, FALSE/*bInitialState*/, NULL /*Name*/); // 사용하면 cpu점유율이 0으로 내려감

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}