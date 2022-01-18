#pragma once

#include "iType.h"

#include "iSceneManager.h"
#include "iWindows.h"
#include "iString.h"

class iChatServer;

class ServerScene : public iScene
{
public:
	virtual void load(iArray* recvInfo);
	virtual void update(float dt);
	virtual void free();

public:
	iChatServer* chatServer;
};

#define SERVER_BUFFER_SIZE 1024

class iBinarySearchTree;

struct iChatServer
{
public:
	iChatServer();
	virtual ~iChatServer();

	static void* service(void* server);

	static bool userEqualMethod(void* left, void* right);
	static void* userMinMethod(void* left, void* right);

public:
	int servSock;

	int step;
	const char* msg[10];
	char buffer[SERVER_BUFFER_SIZE];

	iArray allRooms;

	iArray _allUser;
	iBinarySearchTree* allUser;
};

struct iChatRoom
{
	iString roomName;
	uint64 roomNumber;

	iArray users;
	char* chatHistory[10];
};

struct iChatUser
{
	iString nickName;
	iString passWord;

	uint64 socket;
	sockaddr_in clientAddr;
};

void* serverWork();
void* service(void* clientSock);

int createSocket(const char* servIp, uint16 servPort, IPPROTO protocol);

bool isend(uint64 socket, const char* msg);
char* irecv(uint64 socket);
