#pragma once
#include <string>
#include <mutex>
class MonoCtrl
{
public:
	MonoCtrl();
	~MonoCtrl();
	bool InitMono(const char *szFilePath);
	bool startMono(const char *szFilePath);
	bool stopMono();
	std::string getDevData(std::string);
	std::string getCurStatus();
	void* getClassObj() {return m_hClassObj;}
	void* getContext() {return m_hContext;}
	void unInitMono();
	std::string getDevLog(const char *);
	std::string getLastData(const char *);
private:
	std::string sendReqData(std::string strCmd, std::string strParam);
private:
	std::mutex m_mtxLock;
	void *m_hMonoDomain;
	void *m_hClassObj;
	void *m_hContext;
	void *m_hClientSock;
};