#ifndef IDATABASEHANDLER_H
#define IDATABASEHANDLER_H

#include<string>
#include<vector>

class IDatabaseHandler
{
public:
		virtual bool CreateDatabase(std::string databaseFileName) = 0;
      virtual bool CreateTable(std::string query) = 0;
      virtual bool Insert(std::string email, std::string nickname, std::string password, bool isActive) = 0;
      virtual bool InsertByQuery(std::string query) = 0;
		virtual void SetInsertQuery(std::string query) = 0;
		virtual std::string GetErrorMessage() = 0;
		virtual std::vector<std::vector< std:: string>> Select(std::string selectQuery) = 0;
		virtual std::vector< std:: string> SelectSingleRecord(std::string selectQuery) = 0;

};
#endif // IDATABASEHANDLER_H
