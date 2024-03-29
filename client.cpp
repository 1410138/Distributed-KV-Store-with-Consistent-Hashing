#include<bits/stdc++.h>
#include "COMMON_FUNCN.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "jsonstring.h"

using namespace rapidjson;
using namespace std;

#define FILENAME "cs_config.txt"

void request_for_get_delete(string type,string key,int sock_fd)
{
    Document document1;
    send_message(sock_fd,get_delete_CS(type,key));
    string value_json=receive_message(sock_fd);
    if(document1.Parse(value_json.c_str()).HasParseError())
    {
        cout<<"error in request for client parsing string"<<endl;
    }
    else if(strcmp(document1["req_type"].GetString(),"data")==0)
    { 
        assert(document1.IsObject());
        assert(document1.HasMember("req_type"));
        assert(document1.HasMember("message"));
        string value=document1["message"].GetString();
        cout<<"value: "<<value<<endl;
    }
    else if(strcmp(document1["req_type"].GetString(),"ack")==0)
    { 
        assert(document1.IsObject());
        assert(document1.HasMember("req_type"));
        assert(document1.HasMember("message"));
        string value=document1["message"].GetString();
        cout<<value<<endl;
    }
}

//*************************************************do put and update together**************************************//

void request_for_update_put(string type,string key,string value, int sock_fd)
{
    Document document1;
    send_message(sock_fd,put_update_CS(type,key,value));
    string value_json=receive_message(sock_fd); // 1) ack+no_slave_server_active
                                                // 2) ack+put_success  
                                                // 3) ack+commit_failed
    if(document1.Parse(value_json.c_str()).HasParseError())
    {
        cout<<"error in request for client parsing string"<<endl;
    }
    else if(strcmp(document1["req_type"].GetString(),"data")==0)
    { 
        assert(document1.IsObject());
        assert(document1.HasMember("req_type"));
        assert(document1.HasMember("message"));
        string value1=document1["message"].GetString();
        cout<<value1<<endl;
    }
    else if(strcmp(document1["req_type"].GetString(),"ack")==0)
    { 
        assert(document1.IsObject());
        assert(document1.HasMember("req_type"));
        assert(document1.HasMember("message"));
        string value1=document1["message"].GetString();
        cout<<value1<<endl;
    }
}

//*********************************************************************************************//

int main(int argc,char **argv)
{
	if(argc<=2)
	{
		cout<<"enter ip port "<<endl;
		return 0;
	}
	string ip_address=argv[1];
	string port_number=argv[2];
	
	int sock_fd=initialize_socket(ip_address,port_number);
     
    ifstream file(FILENAME);
    string cs_ip,cs_port;

    if (file.is_open()) 
    {	    
	    getline(file, cs_ip);
	    getline(file, cs_port); 
	    file.close();
	}
    cout<<" cs ip "<<cs_ip<<" port "<<cs_port<<endl;
    connect_f(sock_fd,cs_ip,cs_port);
	receive_message(sock_fd);// ack + connected
    send_message(sock_fd,identity_string("client"));
    receive_message(sock_fd);// ack + ready_to_serve

    cout<<"command format : command_type:key:value"<<endl;
    while(true)
    {
        string command;
        cout<<"command > ";
        cin>>command;

        if(command=="exit")
        {
     	    cout<<"Bye~"<<endl;
     	    return 0;
        }
        char *command_char=(char*)command.c_str();
        string type=strtok(command_char,":");

        if(type=="get"||type=="delete")
        {
            char *get_char=strtok(NULL,":");
     	    string key=get_char;
     	    request_for_get_delete(type,key,sock_fd);
        } 

        if(type=="put"||type=="update")
        {
            char *get_char=strtok(NULL,":");
     	    string key=get_char;
        	char *get_val=strtok(NULL,":");
     	    string value=get_val;
            request_for_update_put(type,key,value,sock_fd);
        }
        cout<<endl;
    }
}