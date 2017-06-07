#include <QApplication>

#include "Mirror_Proc.h"
#include "fes/platformwrapper.h"
#include <ace/OS.h>

int main(int argc, char* argv[])
{

	Mirror_Proc mirror_proc;

	if (!Proc_manager::proc_exist("fesmirror"))
	{
		ACE_OS::printf("֪ͨ����������� exit\n");
		exit(0);
	}

	Proc_manager::start_proc("fesmirror", "�������ݾ������","supernb");
	mirror_proc.start();

	while(true)
	{
		if (!Proc_manager::check_parent())
		{
			ACE_OS::sleep(2);
		}
		else
		{
			break;
		}
	}

//  	ACE_Thread_Manager::instance()->wait();

	Proc_manager::end_proc();

	return 0;
}