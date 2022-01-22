#include "ServerScene.h"
#include "Game.h"
#include "iStd.h"

void ServerScene::load(iArray* recvInfo)
{
	//chatServer = new iChatServer();
	serv = new iServer("192.168.50.84", 9600);
	iThreadPool::share()->addJob(iServer::run, serv);
}

void ServerScene::update(float dt)
{
	if (inputMg->keyOnce & KEY_1)
	{
		sceneMg->changeScene((uint32)0);
	}
	else if (inputMg->keyOnce & KEY_2)
	{
		sceneMg->changeScene((uint32)1);
	}
	else if (inputMg->keyOnce & KEY_3)
	{
		sceneMg->changeScene((uint32)2);
	}
}

void ServerScene::free()
{
	//delete chatServer;
	delete serv;
}

iChatServer::iChatServer()
{
	step = 0;

	msg[0] = "Id Password";
	msg[1] = "";

	memset(buff, 0, sizeof(char) * SERVER_BUFFER_SIZE);

	servSock = createSocket("192.168.50.84", 9600);

	iThreadPool::share()->addJob(service, this, serviceEnd);
}

iChatServer::~iChatServer()
{
	for (int i = 0; i < _allUser.dataNum; i++)
	{
		iChatUser* user = (iChatUser*)_allUser[i];

		closesocket(user->socket);
		delete user;
	}

	closesocket(servSock);
}

void* iChatServer::service(void* server)
{
	iChatServer* serv = (iChatServer*)server;
	int* servResult = new int(0);

	*servResult = listen(serv->servSock, SOMAXCONN);

	if (servResult < 0)
	{
		*servResult = 1;

		return servResult;
	}

	int allUser = 0;
	iChatRoom room;
	serv->allRooms.push_back(&room);

	while (1)
	{
		serv->step = 0;

		sockaddr_in strangerAddr;
		int addrLen = sizeof(sockaddr_in);

		int stranger = accept(serv->servSock, (sockaddr*)&strangerAddr, &addrLen);

		if (stranger < 0)
		{
			*servResult = 2;

			return servResult;
		}
		else
		{
			printf("hello %s\n", inet_ntoa(strangerAddr.sin_addr));
		}

		iChatUser* user = new iChatUser;
		user->currRoom = &room;
		user->socket = stranger;
		user->clientAddr = strangerAddr;
		user->nickName = "Temp";
		user->nickName += toString(allUser++);
		user->passWord = "Temp";

		room.users.push_back(user);

		iThreadPool::share()->addJob(recvUserMsg, user, answerToUser);
	}

	*servResult = 0;

	return servResult;
}

void iChatServer::serviceEnd(void* r)
{
	int* result = (int*)r;

	if (*result == 1)
	{
		printf("listen() failed with error\n");
	}
	else if (*result == 2)
	{
		printf("accept() failed with error\n");
	}

	delete result;
}

void* iChatServer::recvUserMsg(void* chatUser)
{
	iChatUser* user = (iChatUser*)chatUser;

	char* msg = irecv(user->socket);

	if (msg)
	{
		int len = strlen(msg);
		memcpy(user->buff, msg, sizeof(char) * len + 1);
		delete[] msg;
	}
	else
	{
		user->buff[0] = 0;
	}

	return chatUser;
}

void iChatServer::answerToUser(void* chatUser)
{
	iChatUser* user = (iChatUser*)chatUser;

	char flag = user->buff[0];

	if (flag != 0)
	{
		if (flag == CLIENT_REQUEST)
		{
			iArray* users = &user->currRoom->users;

			for (int i = 0; i < users->dataNum; i++)
			{
				iChatUser* u = (iChatUser*)(*users)[i];

				if (u->nickName == user->nickName) continue;

				isend(u->socket, &user->buff[1]);
			}
		}
		else if (flag == CLIENT_ANSWER)
		{

		}
		else if (flag == CLIENT_CLOSINGSIGN)
		{
			printf("good bye %s\n", inet_ntoa(user->clientAddr.sin_addr));
			closesocket(user->socket);
			delete user;

			return;
		}
		else
		{
			printf("answerToUser() message format error\n");
			closesocket(user->socket);
			delete user;

			return;
		}
	}

	iThreadPool::share()->addJob(recvUserMsg, user, answerToUser);
}


