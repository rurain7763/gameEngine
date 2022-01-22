#pragma once

#include "iType.h"

#ifdef _WIN32
#include "iWindows.h"
#endif

#include "iList.h"

#define iSERVER_THREAD_NUM 3

#define iUSERMANGER_WORKING			1
#define iUSERMANGER_DONE			2
#define iUSERMANGER_READY_TO_DIE	3

#define iSERVER_WORKING_SIGN	0
#define iSERVER_EXIT_SIGN		1
#define iSERVER_READY_TO_DIE	2

enum iServErrCode
{
	iServErrCodeNoErr = 0,
	iServErrCodeListen,
	iServErrCodeAccept,
};

struct iServerUser;
struct iUserManager;

class iServer
{
public:
	iServer(const char* ip, uint16 port);
	virtual ~iServer();

	static void* run(void* server);

	virtual void eventServStart();
	virtual void eventUserIn(iServerUser* client);
	virtual void eventUserWait(iServerUser* client);
	virtual void eventUserOut(iServerUser* client);
	virtual void eventUserRequest(iServerUser* client);
	virtual void eventServExit();

	virtual void eventError(iServErrCode code);

private:
	static void* recvAllUserMsgs(void* userMg);
	static void cbRecvAllUserMsgs(void* userMg);

private:
	int err;

public:
	uint8 sign;
	iList manager;
	uint64 servSock;
};

struct iServerUser
{
	uint64 socket;
	sockaddr_in addr;
	char* msg;
};

struct iUserManager
{
	uint8 flag;

	iServer* server;
	iList user;
};



