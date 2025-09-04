#include "MonoCtrl.h"
#include <unistd.h>
#include <iostream>
static MonoCtrl * g_hMonoCtrl = new MonoCtrl;

int main(int argc, char* argv[])
{
    g_hMonoCtrl->InitMono("./conf/init_conf.conf");
    if(g_hMonoCtrl->startMono("./conf/modbus02.json"))
    {
        while(1)
        {
            std::string strStatus = g_hMonoCtrl->getCurStatus();    
            std::string strReadData = g_hMonoCtrl->getDevData(strStatus);
            if(strReadData.empty())
            {
                sleep(1);
                continue;
            } 
            else
            {
                std::cout << strReadData << std::endl;
                sleep(1);
            }
           
        }
    }

    if(g_hMonoCtrl) 
    {
        delete g_hMonoCtrl;
        g_hMonoCtrl = nullptr;
    } 
}