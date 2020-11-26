#include "DatabaseHandler.h"
#include <iostream>
SqlLiteDBHandler::SqlLiteDBHandler()
{


}

SqlLiteDBHandler::~SqlLiteDBHandler()
{
    sqlite3_close(m_Database);
}

bool SqlLiteDBHandler::CreateDatabase(std::string databaseFileName)
{
	if(sqlite3_open(databaseFileName.c_str(), (&m_Database)))
   {
		auto errorMsg = sqlite3_errmsg(m_Database);
      m_RecentError = errorMsg;
      std::cerr << "Can't open database: " << m_RecentError;
      return false;
   }

   std::cout <<"Opened database successfully\n";
   return true;
}

bool SqlLiteDBHandler::CreateTable(std::string query)
{
	sqlite3_stmt *createStmt;
   sqlite3_prepare(m_Database, query.c_str(), query.size(), &createStmt, NULL);

   if (sqlite3_step(createStmt) != SQLITE_DONE)
   {
	 	auto errorMsg = sqlite3_errmsg(m_Database);
      m_RecentError = errorMsg;
      std::cerr << "Can't create table: \n" << m_RecentError << "\n";
      return false;
   }

   std::cout <<"Table created\n";
   return true;
}


bool SqlLiteDBHandler::Insert(std::string email, std::string nickname, std::string password, bool isActive)
{
   std::string insertQuery = m_InsertQuery + " VALUES('";
   insertQuery += nickname;
   insertQuery += "', '" ;
	insertQuery += email;
   insertQuery += "', '" ;
   insertQuery += password;
   insertQuery += "', '" ;
   insertQuery += isActive ? "1" : "0";
   insertQuery += "');" ;
   sqlite3_stmt *insertStmt;
   sqlite3_prepare(m_Database, insertQuery.c_str(), insertQuery.size(), &insertStmt, NULL);

   if (sqlite3_step(insertStmt) != SQLITE_DONE)
   {
      auto errorMsg = sqlite3_errmsg(m_Database);
      std::cout << errorMsg <<"\n";
      m_RecentError = errorMsg;
      std::cout << "Didn't Insert Item!" << std::endl;
      return false;
   }
   return true;
}

bool SqlLiteDBHandler::InsertByQuery(std::string query)
{
	sqlite3_stmt *insertStmt;
   sqlite3_prepare(m_Database, query.c_str(), query.size(), &insertStmt, NULL);

   if (sqlite3_step(insertStmt) != SQLITE_DONE)
   {
   	auto errorMsg = sqlite3_errmsg(m_Database);
   	std::cout << errorMsg <<"\n";
      m_RecentError = errorMsg;
      std::cout << "Didn't Insert Item!" << std::endl;
      return false;
   }
   return true;
}

void SqlLiteDBHandler::SetInsertQuery(std::string query)
{
    m_InsertQuery = query;
}

std::string SqlLiteDBHandler::GetErrorMessage()
{
	return m_RecentError;
}

std::vector<std::vector< std:: string>> SqlLiteDBHandler::Select(std::string selectQuery)
{
	sqlite3_stmt *stmt;
	std::vector<std::vector<std::string>> result;

	sqlite3_prepare( m_Database, selectQuery.c_str(), -1, &stmt, NULL );//preparing the statement
   sqlite3_step( stmt );//executing the statement

   while( sqlite3_column_text( stmt, 0 ) )
	{	
		std::vector < std:: string > r;
   	for( int i = 0; i < 4; i++ )
   	{
   		auto res = std::string( (char *)sqlite3_column_text( stmt, i ) );
      	r.push_back(res);
      	
      }
      result.push_back(r  );
   	sqlite3_step( stmt );
	}
	
	return result;
}

std::vector< std:: string> SqlLiteDBHandler::SelectSingleRecord(std::string selectQuery) 
{
	sqlite3_stmt *stmt;
	std::vector<std::string> result;

	sqlite3_prepare( m_Database, selectQuery.c_str(), -1, &stmt, NULL );//preparing the statement
   sqlite3_step( stmt );//executing the statement

   while( sqlite3_column_text( stmt, 0 ) )
	{	
   	for( int i = 0; i < 4; i++ )
   	{
   		auto res = std::string( (char *)sqlite3_column_text( stmt, i ) );
      	result.push_back(res);
      	
      }
   	sqlite3_step( stmt );
	}
	
	return result;
}
