#include <linux/watchdog.h>
#include <sys/types.h>
 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include "AppWatchDog.h"
#define DEV_WATCH_DOG "/dev/watchdog"
AppWatchDog::AppWatchDog()
{
	m_hWatchDog = false;
}


AppWatchDog::~AppWatchDog()
{
}

bool AppWatchDog::InitWatchDog()
{
	bool fRet = false;
	if(m_hWatchDog)
		return true;
	m_hWatchDog = open(DEV_WATCH_DOG, O_RDWR);
	if(m_hWatchDog == -1)
	{
		//fprintf("Open Watchdog ERROR!\n");
		return fRet;
	}
	stopWatchDog();
	fRet = true;
	return fRet;
}


bool AppWatchDog::stopWatchDog()
{
	int setOption = WDIOS_DISABLECARD;
 	if (!m_hWatchDog || 0 > ioctl(m_hWatchDog, WDIOC_SETOPTIONS, &setOption))
	{
		//fprintf(stderr, "ioctl error: WDIOC_SETOPTIONS: %s\n", strerror(errno));
		closeWatchDog();
		return false;
	}
	return true;
}


bool AppWatchDog::startWatchDog()
{
	int setOption = WDIOS_ENABLECARD;
	if (!m_hWatchDog ||0 > ioctl(m_hWatchDog, WDIOC_SETOPTIONS , &setOption))
	{
		//fprintf(stderr, "ioctl error: WDIOC_SETOPTIONS: %s\n", strerror(errno));
		closeWatchDog();
		return false;
	}
	return true;
}


bool AppWatchDog::setTimeout(int iSec)
{
/* 设置超时时间 */
 	if (!m_hWatchDog || 0 > ioctl(m_hWatchDog, WDIOC_SETTIMEOUT, &iSec))
	{
		//fprintf(stderr, "ioctl error: WDIOC_SETTIMEOUT: %s\n", strerror(errno));
		closeWatchDog();
		return false;
	}
	return true;
}

void  AppWatchDog::keepAlive()
{
	if(!m_hWatchDog)
		return ;
	ioctl(m_hWatchDog, WDIOC_KEEPALIVE, NULL);
}

void AppWatchDog::closeWatchDog()
{
	if(!m_hWatchDog)
		return ;	
	stopWatchDog();
	close(m_hWatchDog);	
	m_hWatchDog = -1;
}