#pragma once
#include "Client.h"

void error_handling(const char *message);
void print(const char *message);
//typedef error_handling print;
// 함수포인터 typedef 어떻게 하지?

int main()
{
	// 43p
	WSADATA wsaData = { 0, };
	SOCKET hSocket;
	SOCKADDR_IN tServAddr;

	char message[30] = { 0, };
	int iMessageLen = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup failed");
	else
		print("WSAStartup Success");

	//////////////////////////////// 소켓 프로그래밍 ////////////////////////////////////////
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		error_handling("ClientSocket Create Failed");
	else
		print("ClientSocket Create Success");


	// 연결할 서버의 정보를 채운다. 
	wchar_t host[50] = { 0, };
	lstrcpy(host, TEXT("127.0.0.1:5001"));

	memset(&tServAddr, 0, sizeof(SOCKADDR_IN));
	int size = sizeof(tServAddr);
	if (WSAStringToAddress(host, AF_INET, NULL, (SOCKADDR*)&tServAddr, &size) == SOCKET_ERROR)
		error_handling("Convert Address failed : char to uint32");
	else
		print("Network Address Converted to uint32");

	// connect 연결한 서버의 정보를 채운뒤 연결한다.
	if (connect(hSocket, (SOCKADDR *)&tServAddr, sizeof(tServAddr)) == SOCKET_ERROR)
		error_handling("Socket Connect Failed");
	else
		print("Socket Connect Success");

	// 서버로부터 정보를 기다린다. 
	iMessageLen = recv(hSocket, message, sizeof(message) - 1, 0);
	if (iMessageLen == -1)
		error_handling("Recv/Read Error");
	else
		print("Recv Success");

	printf("Message from server : %s \n", message);

	system("pause");

	closesocket(hSocket);
	WSACleanup();

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