#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	// userLock
	lock_guard<mutex> guard(_mutex); // 데드락 발생, 간헐적, 이유: AccountManager에서 accountmanager를 잠그고 서로 락한 곳에 접근해서 계속 대기하기 때문에.. 아래처럼 account와 락 잡는 순서를 맞추면 된다.

	// AccountLock
	Account* account = AccountManager::Instance()->GetAccount(100);

	// userLock
	lock_guard<mutex> guard(_mutex);
}