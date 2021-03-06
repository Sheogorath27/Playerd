#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>

char *logfile; 
SERVICE_STATUS ServiceStatus; 
SERVICE_STATUS_HANDLE hStatus; 
 
void ServiceMain(); 
void ControlHandler(DWORD request); 
int InitService();

int write(char *message)
{	
	FILE *log;
	log = fopen(logfile,"a+");
	fprintf(log,"Playerd %s\n",message);
	fclose(log);
	return 1;
}

void main(int argc, char *argv[],char *envp[])
{
	SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = "PlayerdService";
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;

	StartServiceCtrlDispatcher(ServiceTable);
}

void ServiceMain()
{    int error;
     
    ServiceStatus.dwServiceType        = SERVICE_WIN32; 
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode      = 0; 
    ServiceStatus.dwServiceSpecificExitCode = 0; 
    ServiceStatus.dwCheckPoint         = 0; 
    ServiceStatus.dwWaitHint           = 0; 
 
    hStatus = RegisterServiceCtrlHandler(
		"PlayerdService", 
		(LPHANDLER_FUNCTION)ControlHandler);    
    
    ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
    SetServiceStatus (hStatus, &ServiceStatus);
    
    logfile = getenv("SystemDrive");
    strcat( logfile,"\\Playerd\\logs.txt");
    write("starting");
    
    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
    	write("working");
        Sleep(6000);
    }
}

void ControlHandler(DWORD request) 
{ 
    switch(request) 
    { 
        case SERVICE_CONTROL_STOP: 
             
             write("STOP");
            ServiceStatus.dwWin32ExitCode = 0; 
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            SetServiceStatus (hStatus, &ServiceStatus);
            return; 
 
        case SERVICE_CONTROL_SHUTDOWN: 
            write("stopped");

            ServiceStatus.dwWin32ExitCode = 0; 
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            SetServiceStatus (hStatus, &ServiceStatus);
            return; 
        
        default:
            break;
    } 
 
    SetServiceStatus (hStatus,  &ServiceStatus);
 
    return; 
} 
