#include <grpcpp/grpcpp.h>
#include <string>
#include "users.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using users::RegisterUserReply;
using users::RegisterUserRequest;

class RegisterUserClient {
 public:
  RegisterUserClient(std::shared_ptr<Channel> channel)
      : stub_(users::Users::NewStub(channel)) {}

  // Assembles client payload, sends it to the server, and returns its response
  std::string CreateUser(std::string email, std::string nickname, std::string password, bool isActive) 
  {
    RegisterUserRequest request;

    request.set_email(email);
    request.set_nickname(nickname);
    request.set_password(password);
    request.set_isactive(isActive);
  
    // Container for server response
    RegisterUserReply reply;

    // Context can be used to send meta data to server or modify RPC behaviour
    ClientContext context;

    // Actual Remote Procedure Call
    Status status = stub_->registerUser(&context, request, &reply);

    return reply.errormsg();
  }
  
  std::string GetUsers(int pageSize, int pageNumber) 
  {
    users::ReadUsersRequest request;

    request.set_pagesize(pageSize);
    request.set_pagenumber(pageNumber);

    users::ReadUsersReply reply;
    // Context can be used to send meta data to server or modify RPC behaviour
    ClientContext context;

    // Actual Remote Procedure Call
    auto replyStream = stub_->readUsers(&context, request);
    std::cout << "Get users from server:" << std::endl;
	 while ( replyStream->Read(&reply)) 
	 {
	 	std::cout << "nickname: " << reply.nickname() << " email: " << reply.email() << " password: "<< reply.password() << " isActive: " << reply.isactive() << std::endl; 	
	 }

    return ""
  }
  
  std::string GetUser(std::string nickname) 
  {
    users::ReadUserRequest request;

    request.set_nickname(nickname);

    // Data to be sent to server

    // Container for server response

	 users::ReadUserReply reply;
    // Context can be used to send meta data to server or modify RPC behaviour
    ClientContext context;
    std::cout << "Get user from server with nickname: " << nickname << std::endl;
    // Actual Remote Procedure Call
    stub_->readUser(&context, request, &reply);
	 std::cout << "nickname: " << reply.nickname() << " email: " << reply.email() << " password: "<< reply.password() << " isActive: " << reply.isactive() << std::endl; 	
    
    return "";
  }
  
 private:
  std::unique_ptr<users::Users::Stub> stub_;
};

void RunClient() {
  std::string target_address("0.0.0.0:50051");
  // Instantiates the client
  RegisterUserClient client(
      // Channel from which RPCs are made - endpoint is the target_address
      grpc::CreateChannel(target_address,
                          // Indicate when channel is not authenticated
                          grpc::InsecureChannelCredentials()));

  std::string response;

  // RPC is created and response is stored
  response = client.CreateUser("user", "email@email.com" , "password", true);
  response = client.CreateUser("user123", "email123@email.com" , "password123", true);

	client.GetUsers(100, 0);
	
	client.GetUser("user");
  // Prints results
  std::cout << "Reply from server: " << response << std::endl;
}

int main(int argc, char* argv[]) {
  RunClient();

  return 0;
}
