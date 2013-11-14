#ifndef _LOCKED_FOLDER_H_
#define _LOCKED_FOLDER_H_

class CLockedFolder
{
public:
	CLockedFolder();
	explicit CLockedFolder(const char* dir_path);
	~CLockedFolder();

public:
	void			setFolder(const char* dir_path);
	std::string		getFolder();

	bool create();
	bool lock();
	void unlock();
	bool clear();

private:
	std::string m_dir_path;
};

#endif _LOCKED_FOLDER_H_