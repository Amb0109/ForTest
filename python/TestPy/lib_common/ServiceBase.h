#ifndef _SERVICE_BASE_H_
#define _SERVICE_BASE_H_

#include "stdafx.h"

class ServiceBase
{
public:
	typedef bool (*InitServerFunc)();
	typedef void (*StopServerFunc)();

public:
	ServiceBase();
	~ServiceBase();

	static ServiceBase* getInstance();

public:
	bool initService(const char* sServiceName);
	void setServerInitFunc(InitServerFunc fInitServer);
	void setServerStopFunc(StopServerFunc fStopServer);

	bool startService();
	void stopService();
	void servControl(unsigned nRequest);

protected:
	static void WINAPI ServiceMain(int argc, char** argv);
	static void WINAPI CtrlHandler(unsigned nRequest);

private:
	std::string				m_service_name;


	SERVICE_STATUS			m_status;
	ServiceHandleT			m_status_hnd;
	SERVICE_TABLE_ENTRY		m_table_entry[2];

	InitServerFunc			m_func_init;
	StopServerFunc			m_func_stop;
};

#endif // _SERVICE_BASE_H_