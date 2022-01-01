#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	// userLock
	lock_guard<mutex> guard(_mutex); // ����� �߻�, ������, ����: AccountManager���� accountmanager�� ��װ� ���� ���� ���� �����ؼ� ��� ����ϱ� ������.. �Ʒ�ó�� account�� �� ��� ������ ���߸� �ȴ�.

	// AccountLock
	Account* account = AccountManager::Instance()->GetAccount(100);

	// userLock
	lock_guard<mutex> guard(_mutex);
}