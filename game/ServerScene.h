#pragma once

#include "iType.h"

#include "iSceneManager.h"

#include "iHashTable.h"
#include "iServer.h"

class iChatServer;

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

#define CLIENT_REQUEST		'0'
#define CLIENT_ANSWER		'1'
#define CHATROOM_HISTORY_SIZE 10

struct iChatRoom;
struct iChatUser;

class iChatServer : public iServer
{
public:
	iChatServer(const char* ip, uint16 port);
	virtual ~iChatServer();

	virtual void eventServStart();
	virtual void eventUserIn(iServerUser* user);
	virtual void eventUserOut(iServerUser* user);
	virtual void eventUserRequest(iServerUser* user, const char* msg, int len);
	virtual void eventServExit();

	virtual void eventError(iServErrCode code);

private:
	char* getSerialNumber(iServerUser* user);
	void userToRoom(iString roomName, iChatUser* user);
	void userToRoom(uint64 roomNumber, iChatUser* user);

public:
	iList rooms;
	iHashTable users;
};

struct iChatRoom
{
	iString roomName;
	uint64 roomNumber;

	iArray users;
};

struct iChatUser
{
	iServerUser* info;

	iString nickName;
	iString passWord;

	iChatRoom* currRoom;
};


