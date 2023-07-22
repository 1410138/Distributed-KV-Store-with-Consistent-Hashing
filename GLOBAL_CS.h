#ifndef ABC
#define ABC

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <bits/stdc++.h>
using namespace std;

map<string,int>heartbeat_count;
pthread_rwlock_t rwlock;	// global rwlock to check if data_migration is in progress and to pause other requests 

struct thread_struct
{
	int sockfd; 
	string ip_plus_port;
	string ip_port_CS;
};

struct heartbeat_struct
{
	string ip_cs;
};

#endif