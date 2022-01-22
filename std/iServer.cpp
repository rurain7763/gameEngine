#include "iServer.h"
#include "iStd.h"

iServer::iServer(const char* ip, uint16 port)
{
	servSock = createSocket(ip, port);

#ifdef __unix__
	timeval tv = {  };
#elif _WIN32
	DWORD tv = 0.1f * 1000.f;
#endif

	setsockopt(servSock, SOL_SOCKET, SO_RCVTIMEO,
			   (char*)&tv, sizeof(tv));

	err = listen(servSock, SOMAXCONN);

	if (err < 0)
	{
		eventError(iServErrCodeListen);
		closeSocket(servSock);
	}

	for (int i = 0; i < iSERVER_THREAD_NUM; i++)
	{
		iUserManager* mg = new iUserManager;

		mg->server = this;
		iThreadPool::share()->addJob(recvAllUserMsgs, mg, cbRecvAllUserMsgs);
		manager.push_back(mg);
	}

	sign = iSERVER_WORKING_SIGN;
}

iServer::~iServer()
{
	sign = iSERVER_EXIT_SIGN;
	closeSocket(servSock);

	for (int i = 0; i < manager.num; i++)
	{
		iUserManager* um = (iUserManager*)manager[i];

		if (um->flag != iUSERMANGER_DONE)
		{
			i--;
			continue;
		}

		for (int j = 0; j < um->user.num; j++)
		{
			iServerUser* su = (iServerUser*)um->user[j];
			closeSocket(su->socket);
			delete su;

			um->user.erase(0);
			j--;
		}

		um->flag = iUSERMANGER_READY_TO_DIE;
	}

	while (sign != iSERVER_READY_TO_DIE) {}

	for (int i = 0; i < manager.num; i++)
	{
		iUserManager* um = (iUserManager*)manager[i];
		delete um;
		
		manager.erase(0);
		i--;
	}
}

void* iServer::run(void* server)
{
	iServer* serv = (iServer*)server;

	int result = 0;
	sockaddr_in userAddr;
	int addrLen = sizeof(sockaddr_in);

	serv->eventServStart();

	while (serv->sign == iSERVER_WORKING_SIGN)
	{
		result = accept(serv->servSock, (sockaddr*)&userAddr, &addrLen);

		if (result < 0)
		{
			serv->eventError(iServErrCodeAccept);
			continue;
		}

		iServerUser* user = new iServerUser;
		user->socket = result;
		user->addr = userAddr;

		serv->eventUserWait(user);

		iUserManager* rcmdUm = NULL;
		uint32 min = 0xffffffff;

		for (int i = 0; i < serv->manager.num; i++)
		{
			iUserManager* um = (iUserManager*)serv->manager[i];

			if (um->user.num < min)
			{
				rcmdUm = um;
				min = um->user.num;
			}
		}

		rcmdUm->user.push_back(user);
		serv->eventUserIn(user);
	}

	serv->eventServExit();

	for (int i = 0; i < serv->manager.num; i++)
	{
		iUserManager* um = (iUserManager*)serv->manager[i];

		if (um->flag != iUSERMANGER_READY_TO_DIE)
		{
			i--;
		}
	}

	serv->sign = iSERVER_READY_TO_DIE;

	return NULL;
}

void iServer::eventServStart()
{
	printf("hello world\n");
}

void iServer::eventUserIn(iServerUser* client)
{
	printf("hello %s\n", inet_ntoa(client->addr.sin_addr));
}

void iServer::eventUserWait(iServerUser* client)
{
	printf("%s waiting\n", inet_ntoa(client->addr.sin_addr));
}

void iServer::eventUserOut(iServerUser* client)
{
	printf("goodbye %s\n", inet_ntoa(client->addr.sin_addr));
}

void iServer::eventUserRequest(iServerUser* client)
{
	printf("%s request\n", inet_ntoa(client->addr.sin_addr));
}

void iServer::eventError(iServErrCode code)
{
	printf("%d error occured\n", code);
}

void iServer::eventServExit()
{
	printf("goodbye world\n");
}

void* iServer::recvAllUserMsgs(void* userMg)
{
	iUserManager* um = (iUserManager*)userMg;

	char* msg;
	uint64 prev;
	float delta;

	for (int i = 0; i < um->user.num; i++)
	{
		iServerUser* user = (iServerUser*)um->user[i];
	
		prev = iTime::share()->getMilliSec();
		msg = irecv(user->socket);
		delta = (iTime::share()->getMilliSec() - prev) / 1000.f;

		if (msg)
		{
			user->msg = msg;
			um->server->eventUserRequest(user);
			delete[] msg;
		}
		else
		{
			if (delta < 0.1f)
			{
				um->server->eventUserOut(user);
				um->user.erase(i);
				closeSocket(user->socket);
				delete user;
				i--;
			}
		}
	}
	um->flag = iUSERMANGER_DONE;

	return um;
}

void iServer::cbRecvAllUserMsgs(void* userMg)
{
	iUserManager* um = (iUserManager*)userMg;

	if (um->server->sign != iSERVER_EXIT_SIGN)
	{
		um->flag = iUSERMANGER_WORKING;
		iThreadPool::share()->addJob(recvAllUserMsgs, um, cbRecvAllUserMsgs);
	}
}

