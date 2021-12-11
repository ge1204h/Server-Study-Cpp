#include "pch.h"
#include <thread>
#include <atomic>
#include <mutex>

// 스레드는 남을 기다려주지 않음. 같은 메모리에 접근했는데 해당 부분이 없으면 문제 발생 => 그래서 락을 건다. atomic은 자료구조의 기능을 사용 불가능
// [1][2][3]
// [1][2][3][][][]
vector<int32> v;

// Mutual Exclusive (상호배타적)
mutex m;

// 고려해야 할 문제
// 1. 재귀적으로 락 거는게 더 재사용하기 좋음. 하지만 mutex는 재귀적으로 호출 불가능
// 2. unlock을 안했을 시 발생 할 수 있는 문제

// RAII (Resource Acquisition is Initialization)
// 생성자에서 잠그고 소멸자에서 풀어주고 하는 패턴
template<typename T>
class LockGuard
{
public:
	// 생성자
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}

	// 소멸자
	~LockGuard()
	{
		_mutex->unlock();
	}
private:
	T* _mutex;
};

void push()
{
	// 락 거는 범위에 따라 많은 것들이 달라짐
	// ex. 함수 맨 앞 or for문 안

	for (int32 i = 0; i < 10000; i++)
	{
		// 자물쇠 잠그기 - unlock전까지 다른곳에서 접근 불가능. 한번에 한개만 접근 가능하기때문에 이 부분에서는 싱글 스레드로 동작한다고 볼 수 있음
		//m.lock();
		//m.lock(); -- 이렇게 재귀적으로 호출 불가능. 재귀적으로 호출 할 수 있는 mutex는 따로있다.
		LockGuard<mutex> lockGuard(m); // lockguard 객체가 생성될때 락. 소멸될때 언락 std::lock_guard<T> 있음
		unique_lock<mutex> uniqueLock(m, defer_lock); // lock_guard에 옵션 추가 가능. defer_lock: 잠기는 시점을 뒤로 미룰 수 있음 (uniqueLock.lock()), lock_guard보다는 조금 더 부하존재

		v.push_back(i);

		// 자물쇠 풀기
		//m.unlock();
		//m.unlock();
	}
}

int main()
{
	thread t1(push);
	thread t2(push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}