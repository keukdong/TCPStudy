#pragma once
#include "Server.h"

#define _WINSOCKET

void error_handling(const char *message);
void print(const char *message);

int main(int argc, char* argv[])
{	
	//LPWSADATA
	WSADATA wsaData;
	SOCKET hServSock, hClientSock;

	SOCKADDR_IN tServAddr, tClinetAddr;

	//char mesagge[] = "Hello World";
	const char* mesagge = "HelloWorld";

	// 프로그램에서 요구하는 윈도우 소켓의 버전을 알리고, 해당 버전을 지원하는 라이브러리의 초기화 작업
	// LPWSADATA = WSADATA*
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");
	else
		print("WSAStartup()");

	//////////////////////////////// 소켓 프로그래밍 /////////////////////////////////////////
	// 소켓 생성
	hServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (hServSock == INVALID_SOCKET)
		error_handling("Invalid socket");
	else
		print("Socket Create");

	// 소켓에 포트번호, IP를 할당한다.
#ifdef _WINSOCKET
	// window 종속
	// 1. LPTSTR ip와 port 번호담고있는 문자열 주소
	// 2. int addressFamily 
	// 3. LPWSAPROTOCOL_INFO 프로토콜 제공자 설정, 일반적으로 NULL
	// 4. LPSOCKADDR 주소 정보를 담을 구조체 변수의 주소
	// 5. LPINT 4번의 인자가 가르키는 변수 크기를 담고있는 변수의 주소값

	// LPWSTR  = WCHAR*
	wchar_t host[50] = { 0, };
	lstrcpy(host, TEXT("127.0.0.1:5001"));

	memset(&tServAddr, 0, sizeof(SOCKADDR_IN));
	int size = sizeof(tServAddr);
	if (WSAStringToAddress(host, AF_INET, NULL, (SOCKADDR*)&tServAddr, &size) == SOCKET_ERROR)
		error_handling("Convert Address failed : char to uint32");
	else
		print("Network Address Converted to uint32");

	if (bind(hServSock, (SOCKADDR*)&tServAddr, sizeof(tServAddr)) == SOCKET_ERROR)
		error_handling("Bind failed");
	else
		print("Socket Bind");

#else
	// hton, ntoh : endian conversion
	memset(&tServAddr, 0, sizeof(SOCKADDR_IN));
	tServAddr.sin_family = AF_INET;		// 주소체계
	tServAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// 32비트 IP주소 : INADDR_ANY 

	const char *host = "127.0.0.1";
	unsigned long conv_addr = inet_addr(host);

	if (conv_addr == INADDR_NONE)
		error_handling("convert failed");
	else
		printf("networkd ordered integer addr :  %#lx \n", conv_addr);

	tServAddr.sin_port = htons(atoi("5001"));		// 16비트 포트번호  
	// tServAddr.sin_zero  // sin_zero[8] : 안쓰지만 0으로 초기화 해놔야함

	if (bind(hServSock, (SOCKADDR*)&tServAddr, sizeof(tServAddr)) == SOCKET_ERROR)
		error_handling("Bind failed");
#endif

	// listen : 서버소켓과 5개의 클라이언트 요청을 받아들인다. 
	if (listen(hServSock, 7) == SOCKET_ERROR)
		error_handling("Listen failed");
	else
		print("Socket is Listening");

	while (1)
	{
		// accept
		// 클라이언트 프로그램에서의 연결 요청을 수락할 때 호출하는 함수
		int clientsize = sizeof(tClinetAddr);
		hClientSock = accept(hServSock, (SOCKADDR *)&tClinetAddr, &clientsize);

		if (hClientSock == INVALID_SOCKET)
			error_handling("Accept failed");
		else
			print("Client Accepted");

		// send
		if (send(hClientSock, mesagge, strlen(mesagge), 0) == SOCKET_ERROR)
			error_handling("Send Error");
		else
			print("Send Message to Client");

	}
	system("pause");

	// close socket
	if(closesocket(hClientSock) ==0 && closesocket(hServSock) == 0)
		print("Close Socket");

	//윈도우 라이브러리를 윈도우 운영체제에 반환, 윈속관련 함수의 호출이 불가능해진다.
	if (WSACleanup() == SOCKET_ERROR)
		error_handling("WSACleanup Failed");
	else
		print("WSACleanup");

	return 0;
}

void error_handling(const char *message)
{
	std::cout << message << "\n";	
}
void print(const char *message)
{
	std::cout << message << "\n";
}