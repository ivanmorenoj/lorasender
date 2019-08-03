#ifndef SQLCONNECTOR_H_
#define SQLCONNECTOR_H_

#include <cstddef>
#include <iostream>
#include <vector>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <time.h>

#include "dbStruct.h"

class sqlConnector
{
private:
    char *user;
    char *pass;
    char *host;
    char *schema;
    char _buff[250];

    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement  *stmt;
    sql::ResultSet  *res;
    sql::PreparedStatement *pstmt;

public:
    sqlConnector();
    sqlConnector(const char* _u,const char* _p,const char* _h);
    ~sqlConnector();
    void setUser(char* _user);
    void setPassword(char* _pass);
    void setHost(char* _host);
    void setSchema(char *_schema);
    void connect();
    void excecuteQuery(char * query_str);

    uint64_t getLatestId();
    struct gas_values fetch_gas_values();
};

#endif //SQLCONNECTOR_H_