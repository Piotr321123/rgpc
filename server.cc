#include <bits/stdc++.h>
#include <grpcpp/grpcpp.h>
#include <string>
#include "users.grpc.pb.h"
#include <iostream>
#include <memory>
#include "DatabaseHandler.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using users::RegisterUserReply;
using users::RegisterUserRequest;


// Server Implementation
class ReverseServiceImplementation final : public users::Users::Service
{
	public:	
		ReverseServiceImplementation(std::unique_ptr<IDatabaseHandler> databaseHandler) : m_DatabaseHandler(std::move(databaseHandler))
		{
			std::string createTableQuery = "CREATE TABLE IF NOT EXISTS user (email TEXT, nickName TEXT, password TEXT,isActive BIT, CONSTRAINT PK_NickName PRIMARY KEY (nickName))";
			std::string insertQuery = "INSERT INTO user (email, nickName, password, isActive) ";
			m_DatabaseHandler->CreateDatabase("../../database.db");
			m_DatabaseHandler->CreateTable(createTableQuery);
			m_DatabaseHandler->SetInsertQuery(insertQuery);
		}


	private:
		Status registerUser(ServerContext* context, const RegisterUserRequest* request, RegisterUserReply* reply) override 
		{			 
			if(!m_DatabaseHandler->Insert(request->email(), request->nickname(), request->password(), request->isactive()))
			{
				reply->set_errormsg(m_DatabaseHandler->GetErrorMessage());
				return Status::OK;
			}

			reply->set_errormsg("OK");
			return Status::OK;
		}
		
		Status readUsers(ServerContext* context, const users::ReadUsersRequest* request, grpc::ServerWriter<users::ReadUsersReply>* writer) override 
		{			 
			std::string query = "SELECT * FROM user LIMIT " + 
			std::to_string(request->pagesize() * request->pagenumber()) + " , " + 
			std::to_string(request->pagesize()) + ";";
			
			std::cout << " Query : " << query << "\n";
			auto users = m_DatabaseHandler->Select(query);
			for (const auto& user : users) 
			{
			   users::ReadUsersReply reply;
			   reply.set_email(user[0]);
 				reply.set_nickname(user[1]);

				reply.set_password(user[2]);
			   reply.set_isactive(user[3] == "1" ? true : false);

				writer->Write(reply);
    			}
			
			return Status::OK;
		}
		
		Status readUser(ServerContext* context, const users::ReadUserRequest* request, users::ReadUserReply* reply) override 
		{			 
			std::string query = "SELECT * FROM user WHERE nickname='" + request->nickname() + "'";
						
			std::cout << " Query : " << query << "\n";
			auto user = m_DatabaseHandler->SelectSingleRecord(query);
			
			reply->set_email(user[0]);
			reply->set_nickname(user[1]);
			reply->set_password(user[2]);
		   reply->set_isactive(user[3] == "1" ? true : false);

			//reply->set_errormsg("OK");
			return Status::OK;
		}
		
		std::unique_ptr<IDatabaseHandler> m_DatabaseHandler;
};

void RunServer() 
{
	std::string server_address("0.0.0.0:50051");
	std::unique_ptr<IDatabaseHandler> databaseHandler = std::make_unique<SqlLiteDBHandler>();
		  
	ReverseServiceImplementation service(std::move(databaseHandler));

	ServerBuilder builder;
	// Listen on the given address without any authentication mechanism
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// Register "service" as the instance through which
	// communication with client takes place
	builder.RegisterService(&service);

	// Assembling the server
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on port: " << server_address << std::endl;

	server->Wait();
}

int main(int argc, char** argv) {
	RunServer();
	return 0;
}
