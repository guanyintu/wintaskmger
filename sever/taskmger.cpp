#include <stdio.h>  
#include <winsock2.h>  
  
#pragma comment(lib,"ws2_32.lib")
using namespace std;  
  string sumname,sumpid,sumnumber,sumpath;
int main(int argc, char* argv[])  
{  
    //初始化WSA  
    WORD sockVersion = MAKEWORD(2,2);  
    WSADATA wsaData;  
    if(WSAStartup(sockVersion, &wsaData)!=0)  
    {  
        return 0;  
    }  
  
    //创建套接字  
    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
    if(slisten == INVALID_SOCKET)  
    {  
        printf("socket error !");  
        return 0;  
    }  
  
    //绑定IP和端口  
    sockaddr_in sin;  
    sin.sin_family = AF_INET;  
    sin.sin_port = htons(8888);  
    sin.sin_addr.S_un.S_addr = INADDR_ANY;   
    if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)  
    {  
        printf("bind error !");  
    }  
  
    //开始监听  
    if(listen(slisten, 5) == SOCKET_ERROR)  
    {  
        printf("listen error !");  
        return 0;  
    }  
  
    //循环接收数据  
    SOCKET sClient;  
    sockaddr_in remoteAddr;  
    int nAddrlen = sizeof(remoteAddr);  
    char revData[255];   
    HANDLE hThread;
	DWORD dwThreadId;
	
	hThread = CreateThread( NULL	// 默认安全属性
						, NULL		// 默认堆栈大小
						, gongnengshixian // 线程入口地址
						, NULL	//传递给线程函数的参数
						, 0		// 指定线程立即运行
						, &dwThreadId	//线程ID号
						);


    while (true)  
    {   
        sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);  
        if(sClient == INVALID_SOCKET)  
        {  
            printf("accept error !");  
            continue;  
        }  
        printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));  
          
        //接收数据  
        int ret = recv(sClient, revData, 255, 0);         
        if(ret > 0)  
        {  
            revData[ret] = 0x00;  
            printf(revData);  
        }  
  
        //发送数据  
        //const char * sendData = "你好，TCP客户端！\n";  
        send(sClient, sendData, strlen(sendData), 0);  
        closesocket(sClient);  
    }  
      
    closesocket(slisten);  
    WSACleanup();  
    return 0;  
} 

DWORD WINAPI gongnengshixian(LPVOID lpParam)
{
	int nItem = 0;	// 项计数

	MODULEENTRY32 Mod32;
	HANDLE hSnapshot;
	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) }; 
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return; 
	}
	if(Process32First(hProcessSnap, &pe32)) 
	{ 
		do 
		{   TCHAR szProcessName[40];
        	TCHAR szProcessPID[20];
        	TCHAR szThreadNum[20];
           	TCHAR szProcessPath[256];
        	TCHAR szSystemPath[256];  //不知道怎么回事，系统进程取不到全路径
	        TCHAR szWindowPath[256];
            TCHAR szItem[20];
	        ZeroMemory(szSystemPath,sizeof(szSystemPath)/sizeof(TCHAR));
	        ZeroMemory(szWindowPath,sizeof(szWindowPath)/sizeof(TCHAR));

	        GetSystemDirectory(szSystemPath,sizeof(szSystemPath)/sizeof(TCHAR));
	        GetWindowsDirectory(szWindowPath,sizeof(szWindowPath)/sizeof(TCHAR));
			ZeroMemory(szProcessName,sizeof(szProcessName)/sizeof(TCHAR));
			ZeroMemory(szProcessPID,sizeof(szProcessPID)/sizeof(TCHAR));
			ZeroMemory(szThreadNum,sizeof(szThreadNum)/sizeof(TCHAR));
			ZeroMemory(szProcessPath,sizeof(szProcessPath)/sizeof(TCHAR));

			wsprintf(szProcessName,"%s",pe32.szExeFile);
			wsprintf(szProcessPID, "%i", pe32.th32ProcessID);
			wsprintf(szThreadNum,"%i",pe32.cntThreads);
			
			
			hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE,pe32.th32ProcessID );
			Module32First( hSnapshot,&Mod32 );

			if (NULL == strrchr(Mod32.szExePath,'\\'))
			{
				if (0 == lstrcmp(Mod32.szExePath,"System"))
				{
					wsprintf(szProcessPath,"%s\\%s",szWindowPath,Mod32.szExePath);
				}
				else
				{
					wsprintf(szProcessPath,"%s\\%s",szSystemPath,Mod32.szExePath);
				}
			}
			else
			{
				wsprintf(szProcessPath,"%s",Mod32.szExePath);
			}
			sumname=sumname+szProcessName;
            sumpid=sumpid+szProcessPID;
            sumnumber=sumnumber+szThreadNum;
            sumpath=sumpath+szProcessPath;
			nItem++;
			
		} 
		while(Process32Next(hProcessSnap, &pe32));
	}
	CloseHandle(hProcessSnap);

	ZeroMemory(szItem,sizeof(szItem)/sizeof(TCHAR));
	wsprintf(szItem,"线程数: %i",nItem);
}
