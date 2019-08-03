/*
*   Inyect query in a schema of database
*   This example was based in mysql connector example:
*     -> https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-examples-complete-example-1.html
*   Author: Ivan Moreno
*     April 2019
*/
#include "plog/Log.h"
#include "sqlConnector.h"

sqlConnector::sqlConnector() {
  user = pass = host = schema = NULL;
  driver = NULL;
  con = NULL;
  stmt = NULL;
  res = NULL;
  pstmt = NULL;
}
sqlConnector::sqlConnector(const char* _u,const char* _p,const char* _h) {
  user = (char *)_u;
  pass = (char *)_p;
  host = (char *)_h;
}
sqlConnector::~sqlConnector() {
  if (con) {
    delete stmt;
    delete con;
  }
}
void sqlConnector::setUser(char* _user) {
  user = _user;
}
void sqlConnector::setPassword(char* _pass) {
  pass = _pass;
}
void sqlConnector::setHost(char* _host) {
  host = _host;
}
void sqlConnector::setSchema(char *_schema) {
  schema = _schema;
}
void sqlConnector::connect() {
  if ( user && pass && host && schema)
    try {
      /* Create a connection */
      driver = get_driver_instance();
      con = driver->connect(host,user,pass);
      
      /* Connect to the MySQL schema */
      con->setSchema(schema);
      stmt = con->createStatement();    
    } catch(sql::SQLException &e ) {
      /* manage errors*/
      PLOG_ERROR << "# ERR: SQLException in " << __FILE__;
      PLOG_ERROR << "( << __FUNCTION__ << ) on line >>" << __LINE__ << std::endl;
      PLOG_ERROR << "# ERR: " << e.what();
      PLOG_ERROR << " (MySQL error code: " << e.getErrorCode();
      PLOG_ERROR << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
}
void sqlConnector::excecuteQuery(char * query_str) {
  try {
    /* Execute query */
    stmt->execute(query_str);
  } catch(sql::SQLException &e) {
    /* manage errors*/
    PLOG_ERROR << "# ERR: SQLException in " << __FILE__;
    PLOG_ERROR << "( << __FUNCTION__ << ) on line >>" << __LINE__ << std::endl;
    PLOG_ERROR << "# ERR: " << e.what();
    PLOG_ERROR << " (MySQL error code: " << e.getErrorCode();
    PLOG_ERROR << ", SQLState: " << e.getSQLState() << " )" << std::endl;
  }
}
struct gas_values sqlConnector::fetch_gas_values() {
  static const char *_query  = "SELECT * FROM DSENSO_GyP ORDER BY ID DESC LIMIT 1;";

  gas_values _tmp_gv;

  memset(&_tmp_gv,0,sizeof(_tmp_gv));

  res = stmt->executeQuery(_query);

  while(res->next()) {
    _tmp_gv._id  = res->getInt64("ID");

    _tmp_gv.amb._tem = std::stof(res->getString("TEM"));
    _tmp_gv.amb._pre = std::stof(res->getString("PRE"));
    _tmp_gv.amb._hum = std::stof(res->getString("HUM"));

    _tmp_gv._co   = std::stof(res->getString("COPPM")); 
    _tmp_gv._o3   = std::stof(res->getString("O3PPM")); 
    _tmp_gv._so2  = std::stof(res->getString("SO2PPM")); 
    _tmp_gv._no2  = std::stof(res->getString("NO2PPM")); 
    _tmp_gv._pm1  = std::stof(res->getString("PM1")); 
    _tmp_gv._pm10 = std::stof(res->getString("PM10")); 
    _tmp_gv._pm25 = std::stof(res->getString("PM25"));
  }
  delete pstmt;
  delete res;
  return _tmp_gv;
}
uint64_t sqlConnector::getLatestId() {
  static const char *_query  = "SELECT ID FROM DSENSO_GyP ORDER BY ID DESC LIMIT 1;";
  uint64_t _id = 0;
  res = stmt->executeQuery(_query);
  if (res->next()) {
    _id = res->getInt("ID");
  } 
  return _id;
}