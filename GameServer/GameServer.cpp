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

// Condition Variable

mutex m;
queue<int32> q;
HANDLE handle;

// 참고) CV는 User-Level Object (커널오브젝트X)
// 유저레벨의 오브젝트는 동일한 프로그램에서만 사용가능
// 커널오브젝트는 다른 프로그램에서도 사용 가능하다.
condition_variable cv;

/*
* Q. 숫자가 계속해서 증가함...
A. cv.notify_one()을 한 다음에 친절히 상대방에 꺼내가는 것을 기다리진 않기에
절묘하게 다음 while(true) 루프에서 또  lock을 먼저 획득하면 연이어서 숫자가 늘어날 수 있음

멀티쓰레드 환경은 공유자원을 차지하려고 격렬히 경합을 하는 것

그렇다면 매번 데이터를 produce 하는 쪽만 승리하면 어쩌지? 싶겠지만 이렇게 while (true)로 데이터를 밀어넣는 경우는 현실상에 없어 큰 문제 없다고 한다.
*/

void Producer()
{
	while (true)
	{
		// 1) Lock을 잡고
		// 2) 공유 변수 값을 수정
		// 3) Lock을 풀고
		// 4) 조건변수를 통해 다른 쓰레드에게 통지

		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		cv.notify_one(); // wait 중인 쓰레드가 있으면 딱 1개를 깨운다.
	}
}

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() {return q.empty() == false; }); // wait(lock, 조건)
		// 1) Lock을 잡고
		// 2) 조건 확인
		// - 만족O => 빠져나와서 이어서 코드를 진행 (이후 코드 진행은 Lock인 상태)
		// - 만족X => Lock을 풀어주고 대기 상태로 전환

		// 그런데 notify_one을 했으면 항상 조건식을 만족하는거 아닐까?
		// spurious Wakeup (가짜 기상?)
		// notify_one 할 때 lock을 잡고 있는 것이 아니기 때문에 다른곳에서 사용해서 없을 수 있다.

		//while (q.empty() == false) // 위에서 체크 하기 때문에 체크할 필요 없음.
		{
			int32 data = q.front();
			q.pop();
			cout << q.size() << endl;
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