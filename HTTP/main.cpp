/*************************************************************************
	> File Name: main.c
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Sun 02 Apr 2017 02:03:31 PM CST
 ************************************************************************/

#include"httpd.h"

static void Usage(const char *msg)
{
	assert(msg);
	cout << msg << " [local_ip] [local_port]" << endl;
}

static void *AcceptRequest(void *arg)
{
	int sock = (int)arg;
	pthread_detach(pthread_self());
	return (void *)Handle_Request(sock);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		exit(1);
	}
	int listen_sock = StartUp(argv[1], atoi(argv[2]));
	
	struct sockaddr_in peer;
	socklen_t len = sizeof(peer);

	while(1)
	{
		int conn_sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
		if(conn_sock < 0)
		{
			PrintLog("accept is failed!\n", WARNING);
			continue;
		}
		cout << "get a new connect, ip:" << inet_ntoa(peer.sin_addr) \
			<< " port:" << peer.sin_port << endl; 

		//创建连接成功
		//创建线程，利用线程来分别处理每个连接
		pthread_t tid; 
		if(pthread_create(&tid, NULL, AcceptRequest, (void*)conn_sock) != 0)
		{
			//创建线程失败
			PrintLog("pthread_create is failed", FATAL);
			close(conn_sock);
		}
	}

	close(listen_sock);
	return 0;
}
