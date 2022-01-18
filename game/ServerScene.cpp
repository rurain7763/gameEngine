#include "ServerScene.h"
#include "Game.h"
#include "iStd.h"

void ServerScene::load(iArray* recvInfo)
{
	chatServer = new iChatServer();
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
	delete chatServer;
}

int clientSock[2];

void* service(void* cs)
{
	int idx = *(int*)cs - 1;

	char buffer[SERVER_BUFFER_SIZE];

	int funcResult = 1;

	while (funcResult > 0)
	{
		funcResult = recv(clientSock[idx], buffer, SERVER_BUFFER_SIZE, 0);

		if (funcResult > 0)
		{
			buffer[funcResult] = 0;
			printf("%s\n", buffer);
		}
		else if (funcResult == 0)
		{
			printf("Connection closing...\n");
			closesocket(clientSock[idx]);
			break;
	}
		else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(clientSock[idx]);
			break;
		}

		funcResult = 1;
	}
}

iChatServer::iChatServer()
{
	msg[0] = "Id Password";
	msg[1] = "";

	allUser = new iBinarySearchTree(userEqualMethod, userMinMethod);

	servSock = createSocket("192.168.50.84", 9600, IPPROTO_TCP);

	listen(servSock, SOMAXCONN);

	iThreadPool::share()->addJob(service, this);
}

iChatServer::~iChatServer()
{
	closesocket(servSock);

	for (int i = 0; i < _allUser.dataNum; i++)
	{
		iChatUser* user = (iChatUser*)_allUser[i];

		closesocket(user->socket);
		delete user;
	}

	delete allUser;
}

void* iChatServer::service(void* server)
{
	iChatServer* serv = (iChatServer*)server;
	int funcResult;

	while (1)
	{
		serv->step = 0;

		sockaddr_in strangerAddr;
		int addrLen = sizeof(sockaddr_in);

		int stranger = accept(serv->servSock, (sockaddr*)&strangerAddr, &addrLen);

		if (stranger < 0)
		{
			printf("accept failed with error: %d\n", WSAGetLastError());
			continue;
		}
		else
		{
			printf("hello %s\n", inet_ntoa(strangerAddr.sin_addr));
		}

		iChatUser user;
		char* r;
		int recvLen = 0;
		
		//id password
		isend(stranger, serv->msg[serv->step]);
		serv->step++;

		r = irecv(stranger);
		recvLen = strlen(r);
		bool tick = true;

		for (int i = 0; i < recvLen; i++)
		{
			if (r[i] == ' ')
			{
				tick = false;
				continue;
			}

			if (tick)
			{
				user.nickName += r[i];
			}
			else
			{
				user.passWord += r[i];
			}
		}

		delete[] r;

		if (!serv->allUser->find(&user))
		{
			//new
			iChatUser* u = new iChatUser(user);
			u->socket = stranger;
			u->clientAddr = strangerAddr;

			serv->allUser->insert(u);
			serv->_allUser.push_back(u);
		}
		else
		{

		}

		printf("current user population %d\n", serv->allUser->num);
		closesocket(stranger);
	}

	return nullptr;
}

bool iChatServer::userEqualMethod(void* left, void* right)
{
	iChatUser* user1 = (iChatUser*)left;
	iChatUser* user2 = (iChatUser*)right;

	if (user1->nickName == user2->nickName)
	{
		if (user1->passWord == user2->passWord)
		{
			return true;
		}
	}

	return false;
}

void* iChatServer::userMinMethod(void* left, void* right)
{
	iChatUser* user1 = (iChatUser*)left;
	iChatUser* user2 = (iChatUser*)right;

	int user1Len = user1->nickName.len + user1->passWord.len;
	int user2Len = user2->nickName.len + user2->passWord.len;

	if (user1Len == user2Len)
	{
		int len = min(user1->nickName.len, user2->nickName.len);

		for (int i = 0; i < len; i++)
		{
			char l = user1->nickName.at(i);
			char r = user2->nickName.at(i);

			if (l == r) continue;

			return l < r ? left : right;
		}

		return len == user1->nickName.len ? left : right;
	}

	return user1Len > user2Len ? left : right;
}

int createSocket(const char* si, uint16 sp, IPPROTO prot)
{
	int servSock = socket(AF_INET,
		prot == IPPROTO_TCP ? SOCK_STREAM : SOCK_DGRAM,
		prot);

	if (servSock < 0)
	{
		printf("socket() error\n");
		return -1;
	}

	sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(sockaddr_in));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(si);
	servAddr.sin_port = htons(sp);

	int funcResult;

	funcResult = bind(servSock, (sockaddr*)&servAddr, sizeof(servAddr));

	if (funcResult != 0)
	{
		printf("bind() error\n");

#ifdef __unix__
		close(servSock);
#elif _WIN32
		closesocket(servSock);
#endif

		return -1;
	}

	return servSock;
}

bool isend(uint64 socket, const char* m)
{
	uint16 len = strlen(m);

	char* msg = new char[len + 3];

	for (int i = 1; i > -1; i--)
	{
		uint8 c = ((uint8*)&len)[i];
		msg[1 - i] = c;
	}

	memcpy(&msg[2], m, sizeof(char) * len);

	msg[len + 2] = 0;

	int funcResult = send(socket, msg, 2 + len, 0);

	if (funcResult < 0)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		delete[] msg;
		return false;
	}
	else
	{
		printf("send %d byte to client\n", len);
		delete[] msg;
		return true;
	}
}

char* irecv(uint64 socket)
{
	char* result = NULL;
	int off = 0;

	char buffer[256];

	int recvResult = 0;
	uint16 msgLen = 0;

	while (1)
	{
		recvResult = recv(socket, buffer, 1024, 0);

		if (recvResult > 0)
		{
			//must be modify... it may occur many error in future
			if (!msgLen)
			{
				int len = recvResult - 2;

				for (int i = 1; i > -1; i--)
				{
					uint8 c = buffer[i];

					for (int j = 0; j < 8; j++)
					{
						msgLen |= (uint16)(c & (1 << j)) << (8 * (1 - i));
					}
				}

				result = new char[msgLen + 1];

				for (int i = 0; i < len; i++)
				{
					result[off] = buffer[i + 2];
					off++;
				}
			}
			else
			{
				for (int i = 0; i < recvResult; i++)
				{
					result[off] = buffer[i];
					off++;
				}
			}

			if (off == msgLen) break;
		}
		else if (recvResult == 0)
		{
			printf("Connection closing...\n");
			break;
		}
		else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			break;
		}
	}

	result[off] = 0;

	return result;
}



