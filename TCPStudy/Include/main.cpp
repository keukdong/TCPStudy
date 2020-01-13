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

	// ���α׷����� �䱸�ϴ� ������ ������ ������ �˸���, �ش� ������ �����ϴ� ���̺귯���� �ʱ�ȭ �۾�
	// LPWSADATA = WSADATA*
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");
	else
		print("WSAStartup()");

	//////////////////////////////// ���� ���α׷��� /////////////////////////////////////////
	// ���� ����
	hServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (hServSock == INVALID_SOCKET)
		error_handling("Invalid socket");
	else
		print("Socket Create");

	// ���Ͽ� ��Ʈ��ȣ, IP�� �Ҵ��Ѵ�.
#ifdef _WINSOCKET
	// window ����
	// 1. LPTSTR ip�� port ��ȣ����ִ� ���ڿ� �ּ�
	// 2. int addressFamily 
	// 3. LPWSAPROTOCOL_INFO �������� ������ ����, �Ϲ������� NULL
	// 4. LPSOCKADDR �ּ� ������ ���� ����ü ������ �ּ�
	// 5. LPINT 4���� ���ڰ� ����Ű�� ���� ũ�⸦ ����ִ� ������ �ּҰ�

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
	tServAddr.sin_family = AF_INET;		// �ּ�ü��
	tServAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// 32��Ʈ IP�ּ� : INADDR_ANY 

	const char *host = "127.0.0.1";
	unsigned long conv_addr = inet_addr(host);

	if (conv_addr == INADDR_NONE)
		error_handling("convert failed");
	else
		printf("networkd ordered integer addr :  %#lx \n", conv_addr);

	tServAddr.sin_port = htons(atoi("5001"));		// 16��Ʈ ��Ʈ��ȣ  
	// tServAddr.sin_zero  // sin_zero[8] : �Ⱦ����� 0���� �ʱ�ȭ �س�����

	if (bind(hServSock, (SOCKADDR*)&tServAddr, sizeof(tServAddr)) == SOCKET_ERROR)
		error_handling("Bind failed");
#endif

	// listen : �������ϰ� 5���� Ŭ���̾�Ʈ ��û�� �޾Ƶ��δ�. 
	// 7->10
	if (listen(hServSock, 10) == SOCKET_ERROR)
		error_handling("Listen failed");
	else
		print("Socket is Listening");

	while (1)
	{
		// accept
		// Ŭ���̾�Ʈ ���α׷������� ���� ��û�� ������ �� ȣ���ϴ� �Լ�
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

	//������ ���̺귯���� ������ �ü���� ��ȯ, ���Ӱ��� �Լ��� ȣ���� �Ұ���������.
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