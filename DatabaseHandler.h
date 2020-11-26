#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H
#include "IDatabaseHandler.h"
#include <sqlite3.h>

class SqlLiteDBHandler : public IDatabaseHandler
{
public:
	SqlLiteDBHandler();


	virtual bool CreateDatabase(std::string databaseFileName) override;
	virtual bool CreateTable(std::string query) override;
	virtual bool Insert(std::string email, std::string nickname, std::string password, bool isActive) override;
	virtual bool InsertByQuery(std::string query) override;
	virtual void SetInsertQuery(std::string query)override;
	virtual std::string GetErrorMessage() override;
	virtual std::vector<std::vector< std:: string>> Select(std::string selectQuery) override;
	virtual std::vector< std:: string> SelectSingleRecord(std::string selectQuery) override;
   ~SqlLiteDBHandler();

private:
	sqlite3* m_Database;
	std::string m_InsertQuery;
	std::string m_RecentError;

};

#endif // DATABASEHANDLER_H
