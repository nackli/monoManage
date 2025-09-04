#include <iostream>
#include <fstream>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include<string.h>
#endif
//#include <filesystem>

#ifdef WIN32
#pragma comment(lib,"Bcrypt")
#pragma comment(lib,"Version")
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"Winmm")
#pragma comment(lib,"libmono-static-sgen")
#endif

#include <mono/jit/jit.h>
#include <mono/metadata/profiler.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/tokentype.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>
// debug
#include <mono/metadata/mono-debug.h>
#include <mono/utils/mono-logger.h>
#include <mono/metadata/threads.h>
#include <zmq.h>
#include "rapidjson/document.h"
#include "MonoCtrl.h"

static MonoClass* OnInstantiateClassPtr(MonoImage* image, const char* namespaceName, const char* className)
{
	//获取MonoClass,类似于反射
	MonoClass* main_class = mono_class_from_name(image, namespaceName, className);
	//MonoObject* classInstance = mono_object_new(g_pMonoDomain, main_class);

	//if (classInstance == nullptr)
	//{
	//	// Log error
	//}
	//mono_runtime_object_init(classInstance);
//	mono_runtime_class_init(main_class)
	return main_class;
}

static MonoClass* OnInitMonoInfo(MonoDomain* hMonoDomain)
{
	//	const char* managed_binary_path = "E:\\gitRepositoty\\TestDotNetExe\\TestFrameLib\\bin\\Debug\\TestFrameLib.dll";


	if(!hMonoDomain)
		return nullptr;

	const char* managed_binary_path = "XIFWCore.dll";



	//if (true)
	//{
	//	const char* options[] =
	//	{
	//		"--soft-breakpoints",
	//		"--debugger-agent=transport=dt_socket,address=127.0.0.1:10000,server=y"
	//	};
	//	mono_jit_parse_options(sizeof(options) / sizeof(char*), (char**)options);
	//	mono_debug_init(MONO_DEBUG_FORMAT_MONO);
	//}

	//const char* managed_binary_path = "E:\\gitRepositoty\\TestDotNetExe\\SelfTest\\bin\\Debug\\net6.0\\SelfTest.dll";

	//const char* managed_binary_path = "E:/gitRepositoty/TestDotNetExe/Debug/XIFWCore.dll";
	//const char* managed_binary_path = "../Debug/TestFrameLib.dll";
	//获取应用域



	//mono_trace_set_level_string("debug");
	//mono_trace_set_print_handler(CallbackMonoPrint);
#ifdef WIN32
	mono_set_dirs("C:/Program Files (x86)/Mono/lib", "C:/Program Files (x86)/Mono/etc/mono");
	mono_config_parse("./config");
#else
#if 1
	mono_set_dirs("/usr/local/lib", "/usr/local/etc/mono");
	mono_config_parse("/usr/local/etc/mono/config");
#else
	mono_set_dirs("/usr/lib", "/etc/mono");
	mono_config_parse("/etc/mono/config");
#endif	
#endif

	//mono_set_assemblies_path("./v4.8");

	//	加载程序集ManagedLibrary.dll
	MonoAssembly* assembly = mono_domain_assembly_open(hMonoDomain, managed_binary_path);
	if (!assembly)
	{
		std::cout << "C# " << managed_binary_path << " file not find!\n";
		return nullptr;
	}

	MonoImage* image = mono_assembly_get_image(assembly);
	if (!image)
	{
		std::cout << "Get assembly fail \n";
		return nullptr;
	}
		
	// std::cout << "00000000000000000 \n";

	// const char* newDomainName = "TestDomainApp";
	// MonoDomain* newDomain = mono_domain_create_appdomain((char*)newDomainName, NULL);
	// if (!mono_domain_set(newDomain, true)) {
	// 	// Cannot set domain!
	// }

	// // 2. Load image. (Works fine)
	// const char* dllFile = managed_binary_path;
	// MonoImageOpenStatus status;
	// MonoImage* newImage = mono_image_open(dllFile, &status); //**Problem here : Shows the same address as before and that means that Mono use same.dll.**

	// // 3. Load new assembly. (Works fine).
	// MonoAssembly* assemblyNew = mono_assembly_load_from(newImage, mono_image_get_name(newImage), &status);
	// if (status != MonoImageOpenStatus::MONO_IMAGE_OK) {
	// 	///Cannot load assembly!
	// }

	// assemblyNew = mono_assembly_open(dllFile, &status);

	// newImage = mono_assembly_get_image(assemblyNew);
	// if (newImage) {
	// 	//AddInternalFunctionsAndScriptClasses();
	// }
 	
	MonoClass* instClassPtr = OnInstantiateClassPtr(image, "XIFWCore.Services", "CoreServices");
	//MonoObject* instClassPtr = InstantiateObjectPtr(newImage, "SelfForm.SelfTest", "TestFrameWork");
	if (!instClassPtr)
		std::cout << "namespace or class name is error\n";

	std::cout << "Init Success\n";
	return instClassPtr;
}

static std::string CallFun1ParamRetString(MonoClass* hObject, const char* szFunName, const char* szParam)
{
	if (!hObject || !szFunName)
		return "";

	MonoClass* objectClass = hObject;//mono_object_get_class(hObject);

	
	MonoString* pStrTemp = mono_string_new_wrapper(szParam);
	// 获取类中函数的指针
	MonoMethod* method = mono_class_get_method_from_name(objectClass, szFunName, 1);
	if (method == nullptr)
	{
		std::cout << "Get method " << szFunName << szParam << "fail" << std::endl;
		return "";
	}


	// 调用objectInstance实例的这个方法，并处理可能出现的异常

	//MonoObject* exc = NULL;
	void* params[1] = { 0 };
	params[0] = pStrTemp;
	std::string strReadData;
	//mono_thread_attach (mono_get_root_domain ());
	
	MonoString* monoRetString = (MonoString*)mono_runtime_invoke(method, objectClass, params, NULL);
	if (monoRetString)
	{
		char* pUtf8Data = mono_string_to_utf8(monoRetString);
		if (pUtf8Data)
		{
			strReadData = pUtf8Data;
			mono_free(pUtf8Data);
		}
	}
	return strReadData;
}

static int CallFun1ParamRetInt(MonoClass* hObject, const char* szFunName, const char* szParam)
{
	int iRet = 0;
	if (!hObject || !szFunName)
		return iRet;
	
	MonoString* pStrTemp = NULL;
	if(szParam)
		pStrTemp = mono_string_new_wrapper(szParam);
	MonoClass* objectClass = hObject;//mono_object_get_class(hObject);
	// 获取类中函数的指针
	MonoMethod* method = mono_class_get_method_from_name(objectClass, szFunName, 1);
	if (method == nullptr)
	{
		//std::cout << "Get method " << szFunName << szParam << "fail" << std::endl;
		return iRet;
	}

	MonoObject* returnObj = nullptr;
	// 调用objectInstance实例的这个方法，并处理可能出现的异常
	if(pStrTemp)
	{
		void* params[1] = { 0 };
		params[0] = pStrTemp;
		//MonoMethod* mmethod = mono_object_get_virtual_method(hObject, method);
		//mono_thread_attach (g_pMonoDomain);
		returnObj = mono_runtime_invoke(method, objectClass, params, NULL);
	}
	else
		returnObj = mono_runtime_invoke(method, objectClass, nullptr, NULL);

	if (returnObj)
	{
		int* result = (int*)mono_object_unbox(returnObj);
		if (result)
			iRet = *result;
	}
	return iRet;
}
 /*
static std::string OnReadDevData(MonoClass* hObject, const char* szParam)
{
	if (!szParam || !hObject)
		return "failure";
	return CallFun1ParamRetString(hObject, "ReadData", szParam);
}


static std::string OnGetDevStatus(MonoClass* hObject, const char* szParam)
{
	if (!hObject)
		return "";
	
	return CallFun1ParamRetString(hObject, "GetStatus", szParam);
}
*/

static bool OnSendMsg(void *hSock, const void *pData, uint16_t iSize)
{
	if(!pData || !iSize)
		return false;
	zmq_msg_t msgSend;
	int rc = zmq_msg_init_size(&msgSend, iSize);
	if(rc)
		return false;
	memcpy(zmq_msg_data(&msgSend), pData, iSize);
	if(zmq_msg_send(&msgSend, hSock,  ZMQ_DONTWAIT)) 
	{
		zmq_msg_close(&msgSend);
		return true;
	}
	zmq_msg_close(&msgSend);
	return false;
}
static void GetDataThreadFun (void *pParam)
{
	if(!pParam)
		return;
	MonoCtrl *pMonoCtrl = (MonoCtrl*)pParam;

	void *pCtx = pMonoCtrl->getContext();

    void *hSockRep = zmq_socket(pCtx,ZMQ_REP);

    zmq_bind(hSockRep,"inproc://mono_proc");
    zmq_msg_t msgRecv;
    zmq_msg_init(&msgRecv);
	
	while(hSockRep)
	{

        if(zmq_msg_recv(&msgRecv, hSockRep, 0) < 0)
			continue;

        //size_t szLen = zmq_msg_size(&msgRecv);

        char* data = static_cast<char*>(zmq_msg_data(&msgRecv));
		if(data)
		{
			char *pCmd = strtok((char *)data, ":");
			char * pParam =	(char *)data + strlen(pCmd) + 1;	

			if(!strcasecmp(pCmd, "exit"))
				break;

			std::string strRet = CallFun1ParamRetString((MonoClass*)pMonoCtrl->getClassObj(), pCmd, pParam);

			OnSendMsg(hSockRep, strRet.c_str(), strRet.length() + 1);
		}
	}
	std::cout << "read exit" << std::endl;
	zmq_close(hSockRep);
	hSockRep = nullptr;
}

static bool OnGetDeviceCode(const char *szFilePath, std::string &strDevCode)
{
	if(!szFilePath)
		return false;

	//std::filesystem::path filePath(szFilePath);
	std::ifstream streamContext(szFilePath);
	if(streamContext.is_open())
	{
		std::string ctxJson((std::istreambuf_iterator<char>(streamContext)), std::istreambuf_iterator<char>());
		rapidjson::Document document;
		if(document.Parse(ctxJson.c_str()).HasParseError())
		{
			std::cout << "json parse error" <<std::endl;
            return false;
		}
		if (!document.IsObject()) 
        {
            std::cout << "error json is not object " <<std::endl;
            return false;
        }

		 if (document.HasMember("deviceCode") && document["deviceCode"].IsString())
		 {
			strDevCode = document["deviceCode"].GetString();
			return true;
		 }
		 	
	}
	else
		std::cout << "file no exist" <<std::endl;
	return true;	
}

MonoCtrl::MonoCtrl():m_hMonoDomain(nullptr),m_hClassObj(nullptr)
{
}


MonoCtrl::~MonoCtrl()
{
}

bool MonoCtrl::InitMono(const char *szFilePath)
{
	bool fRet = false;
	if(!szFilePath)
		return fRet;
	m_hMonoDomain = mono_jit_init("xgMono");
	if (!m_hMonoDomain)
		return false;

	std::ifstream inputFile(szFilePath);
	if (!inputFile.is_open()) {
		std::cout << "init_conf.conf open error!" << std::endl;
		return fRet;
	}

	std::string init_content((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

	inputFile.close();

	m_hClassObj = OnInitMonoInfo((MonoDomain*)m_hMonoDomain);

	m_hContext = zmq_ctx_new();

	mono_thread_create((MonoDomain*)m_hMonoDomain, (void *)GetDataThreadFun, this);

    m_hClientSock = zmq_socket(m_hContext, ZMQ_REQ);
    zmq_connect(m_hClientSock,"inproc://mono_proc");

    // struct timeval recvTimeout;
    // recvTimeout.tv_sec = 0; // 超时时间为1秒
    // recvTimeout.tv_usec = 500;
	// int iRet = zmq_setsockopt(m_hClientSock, ZMQ_RCVTIMEO, &recvTimeout, sizeof(recvTimeout));
    // if (iRet != 0) {
	// 	EINVAL
    //     std::cerr << "Error setting receive timeout " << iRet << errno <<std::endl;
    //     return -1;
    // }

	if(m_hClientSock)
	{
		sendReqData("Init", init_content.c_str());
		fRet = true;
	}
	return fRet;
}

bool MonoCtrl::startMono(const char *szFilePath)
{
	if(!szFilePath || !m_hClassObj)
		return false;
	if(OnGetDeviceCode(szFilePath, m_strDevCode))
	{
		sendReqData("Start", szFilePath);
		return true;
	}
	return false;

}

std::string MonoCtrl::getDevData(std::string strStatus)
{
	return sendReqData("ReadData", strStatus);
}

void MonoCtrl::unInitMono()
{
	if(m_hMonoDomain)
	{
		mono_jit_cleanup((MonoDomain*)m_hMonoDomain);	
		m_hMonoDomain = nullptr;
	}

	sendReqData("exit", "");
	if(m_hClientSock)
	{
		zmq_close(m_hClientSock);
		m_hClientSock = nullptr;
	}

	if(m_hContext)
	{
		zmq_ctx_shutdown(m_hContext);
		m_hContext = nullptr;
	}	 

}

bool MonoCtrl::stopMono()
{
	if(!m_hClassObj)
		return false;
	sendReqData("Stop", "");	
	return true;
}

std::string MonoCtrl::getCurStatus()
{
	return sendReqData("GetStatus", "");	;
}

std::string MonoCtrl::getDevLog(const char *szParam)
{
	if (!m_hClassObj)
		return "";
	
	return sendReqData("GetLogs", szParam);
}

std::string MonoCtrl::getLastData(const char *szParam)
{
	if (!m_hClassObj)
		return "";
	
	return sendReqData("GetLastData", szParam);
}

std::string MonoCtrl::sendReqData(std::string strCmd, std::string strParam)
{
	if(strCmd.empty() || !m_hClientSock)
		return std::string();
	std::lock_guard<std::mutex> lock(m_mtxLock);
	std::string strCmdParam = strCmd + ":" + strParam;//usr struct 

	OnSendMsg(m_hClientSock, strCmdParam.c_str(), strCmdParam.length() + 1);

    zmq_msg_t msgRecv;
    int rc = zmq_msg_init(&msgRecv);
	if(rc)
		return std::string();

    if(zmq_msg_recv(&msgRecv, m_hClientSock, 0) < 0)
	{
		zmq_msg_close(&msgRecv);
		return std::string();
	}
	
	char* data = static_cast<char*>(zmq_msg_data(&msgRecv));

	std::string strReply;
	if(data && data[0] && strcasecmp(data, "null"))
		strReply = data;

	zmq_msg_close(&msgRecv);
	return strReply;
}