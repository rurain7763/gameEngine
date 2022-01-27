#include "ServerScene.h"
#include "Game.h"
#include "iStd.h"

void ServerScene::load(iArray* recvInfo)
{
	chatServer = new iChatServer("192.168.50.84", 9600);
	iThreadPool::share()->addJob(iServer::run, chatServer);
	//serv = new iServer("192.168.50.84", 9600);
	//iThreadPool::share()->addJob(iServer::run, serv);
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
	//delete serv;
	delete chatServer;
}

iChatServer::iChatServer(const char* ip, uint16 port)
	:iServer(ip, port)
{
}

iChatServer::~iChatServer()
{
	
}

void iChatServer::eventServStart()
{
	iChatRoom* room = new iChatRoom;

	room->roomName = "Main";
	room->roomNumber = 0;

	rooms.push_back(room);

	iServer::eventServStart();
}

void iChatServer::eventUserIn(iServerUser* u)
{
	iChatUser* user = new iChatUser;

	char* str = toString(random() % 10000);

	user->info = u;
	user->nickName = "Temp";
	user->nickName += str;
	user->passWord = "Temp";

	delete[] str;
	str = getSerialNumber(u);

	printf("hello %s\n", user->nickName.str);

	iString sm = "--Sys msg : Hello! this msg from chat server you are ";
	sm += user->nickName;
	sm += "--";

	sendMsgToUser(u, sm.str, sm.len);
	userToRoom(0, user);
	users.insert(str, user);

	sm.clear();
	sm += "--Sys msg : ";
	sm += user->nickName;
	sm += " in--";

	iChatRoom* room = user->currRoom;

	for (int i = 0; i < room->users.dataNum; i++)
	{
		iChatUser* u = (iChatUser*)room->users[i];

		if (u->nickName == user->nickName) continue;

		sendMsgToUser(u->info, sm.str, sm.len);
	}

	delete[] str;
}

void iChatServer::eventUserOut(iServerUser* u)
{
	char* sn = getSerialNumber(u);

	iChatUser* user = (iChatUser*)users[sn];
	iArray* roomUsers = &user->currRoom->users;

	iString sm = "--Sys msg : ";
	sm += user->nickName;
	sm += " out--";

	for (int i = 0; i < roomUsers->dataNum; i++)
	{
		iChatUser* u = (iChatUser*)(*roomUsers)[i];

		if (user == u)
		{
			roomUsers->erase(i);
			i--;
		}
		else
		{
			sendMsgToUser(u->info, sm.str, sm.len);
		}
	}

	users.remove(sn);

	printf("good bye %s\n", user->nickName.str);

	delete user;
	delete[] sn;
}

void iChatServer::eventUserRequest(iServerUser* u, const char* msg, int len)
{
	char* sn = getSerialNumber(u);

	char flag = msg[0];

	iChatUser* user = (iChatUser*)users[sn];
	iChatRoom* room = (iChatRoom*)user->currRoom;

	iArray* users = &room->users;

	iString sm = user->nickName + " : ";
	sm += msg;

	for (int i = 0; i < users->dataNum; i++)
	{
		iChatUser* cu = (iChatUser*)(*users)[i];

		if (cu->nickName == user->nickName) continue;

		sendMsgToUser(cu->info, sm.str, sm.len);
	}

	delete[] sn;
}

void iChatServer::eventServExit()
{
	for (iHashTable::iIterator itr = users.begin();
		itr != users.end(); itr++)
	{
		iChatUser* user = (iChatUser*)itr->data;

		delete user;
	}

	for (int i = 0; i < rooms.num; i++)
	{
		iChatRoom* room = (iChatRoom*)rooms[i];

		delete room;
	}	
}

void iChatServer::eventError(iServErrCode code)
{
	if (code == iServErrCodeSelect)
	{
		printf("select() error\n");
	}
	else if (code == iServErrCodeSend)
	{
		printf("send() error\n");
	}
	else if (code == iServErrCodeConnectionOut)
	{
		printf("user out\n");
	}
}

char* iChatServer::getSerialNumber(iServerUser* user)
{
	char* r = toString((long long)user->socket);
	return r;
}

void iChatServer::userToRoom(iString rn, iChatUser* u)
{
	for (int i = 0; i < rooms.num; i++)
	{
		iChatRoom* room = (iChatRoom*)rooms[i];

		if (room->roomName == rn)
		{
			u->currRoom = room;
			room->users.push_back(u);
		}
	}
}

void iChatServer::userToRoom(uint64 rn, iChatUser* u)
{
	for (int i = 0; i < rooms.num; i++)
	{
		iChatRoom* room = (iChatRoom*)rooms[i];

		if (room->roomNumber == rn)
		{
			u->currRoom = room;
			room->users.push_back(u);
		}
	}
}

