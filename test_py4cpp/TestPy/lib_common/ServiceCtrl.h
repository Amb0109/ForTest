#ifndef _SERVICE_CONTROL_H_
#define _SERVICE_CONTROL_H_

class ServiceCtrl
{
public:
	ServiceCtrl();
	~ServiceCtrl();

public:
	bool OpenManager();
	bool installService(const char* sServiceName,
						const char* sSvrDisplayName,
						const char* sServicePath);

	bool removeService(const char* sServiceName);

	bool startService(const char* sServiceName);
	bool stopService(const char* sServiceName);

	bool ctrlService(const char* sServiceName, unsigned nCtrl);
	
private:
	const int		c_try_cnt;

	SC_HANDLE		m_sc_manager;
	SC_HANDLE		m_sc_service;
};

#endif // _SERVICE_CONTROL_H_