#include "pch.h"

#include <thread>

void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread_2( int32 num )
{
	cout << num << endl;
}

int main()
{
	vector<thread> vec;

	for (int32 i = 0; i < 10; i++)
	{
		vec.push_back(thread(HelloThread_2, i));
	}

	for (int32 i = 0; i < 10; i++)
	{
		if (vec[i].joinable())
			vec[i].join();
	}

	//thread te(HelloThread);
	//thread t;
	//auto id = t.get_id(); // id = 0

	//t = thread(HelloThread);
	//auto id = t.get_id(); // id 배정


	//int32 count = t.hardware_concurrency(); // CPU 코어 개수, 정보를 추출 할 수 없다면 0을 출력할 때도 있음
	//auto id = t.get_id(); // 쓰레드마다 id, 쓰레드 사이에는 안 겹친다는 법칙

	//t.join(); // 쓰레드가 끝날때까지 main에서 기다려줌
	//t.detach(); // std::thread 객체에서 실제 쓰레드를 분리, 독립적으로 쓰레드가 동작 할 수 있도록 해줌. 이 경우 추출해둔 쓰레드 정보를 알 수 없음 (보통 사용 x)
	//t.joinable(); // 실질적으로 객체가 관리하고 있는 쓰레드가 살아 있는지 없는지

	//if (t.joinable())
	//	t.join();

	cout << "Hello Main" << endl;
}