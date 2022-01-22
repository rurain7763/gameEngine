#pragma once

#include "iType.h"

#include "iSceneManager.h"
#include "iWindows.h"
#include "iString.h"

class iChatServer;
class iServer;

class ServerScene : public iScene
{
public:
	virtual void load(iArray* recvInfo);
	virtual void update(float dt);
	virtual void free();

public:
	iChatServer* chatServer;
	iServer* serv;
};

#define SERVER_BUFFER_SIZE 1024
#define CLIENT_BUFFER_SIZE 1024

#define CLIENT_REQUEST		'0'
#define CLIENT_ANSWER		'1'
#define CLIENT_CLOSINGSIGN  'q'

class iBinarySearchTree;

struct iChatServer
{
public:
	iChatServer();
	virtual ~iChatServer();

	static void* service(void* server);
	static void serviceEnd(void* result);

private:
	static void* recvUserMsg(void* chatUser);
	static void answerToUser(void* chatUser);

public:
	int servSock;

	int step;
	const char* msg[10];
	char buff[SERVER_BUFFER_SIZE];

	iArray allRooms;

	iArray _allUser;
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
	iChatRoom* currRoom;

	iString nickName;
	iString passWord;

	uint64 socket;
	sockaddr_in clientAddr;

	char buff[CLIENT_BUFFER_SIZE];
};


