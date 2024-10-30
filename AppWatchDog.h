#pragma once
class AppWatchDog
{
public:
	AppWatchDog();
	~AppWatchDog();
	bool InitWatchDog();
	bool stopWatchDog();
	bool startWatchDog();
	bool setTimeout(int iSec);
	void keepAlive();
	void closeWatchDog();
private:
	int m_hWatchDog;
};