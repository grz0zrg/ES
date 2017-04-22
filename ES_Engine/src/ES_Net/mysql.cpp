#include "ES_Net/mysql.h"
#ifndef GP2X
namespace ES
{
	ES_Mysql::ES_Mysql()
	{
		if(mysql_init(&mysql) == NULL)
		{
			std::cout << "_Insufficient memory to allocate mysql object." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	ES_Mysql::~ES_Mysql()
	{
		mysql_close(&mysql);
	}

	int ES_Mysql::connect(const std::string &host, const std::string &user, const std::string &password, const std::string &db, unsigned int port, const char *unix_socket, unsigned long client_flag)
	{
		if(!mysql_real_connect(&mysql, host.c_str(), user.c_str(), password.c_str(), db.c_str(), port, unix_socket, client_flag))
			return -1;

		return 0;
	}

	int ES_Mysql::useResult()
	{
		result = mysql_use_result(&mysql);
		if(result == NULL)
			return -1;
		return 0;
	}

	void ES_Mysql::freeResult()
	{
		mysql_free_result(result);
	}

	int ES_Mysql::fetchRow()
	{
		row = mysql_fetch_row(result);
		if(row == NULL)
			return -1;
		return 0;
	}

	int ES_Mysql::query(const std::string &query)
	{
		if(mysql_query(&mysql, query.c_str()) != 0)
			return -1;
		return 0;
	}

	std::string ES_Mysql::getError()
	{
		return mysql_error(&mysql);
	}
}
#endif
