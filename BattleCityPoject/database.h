#pragma once
#ifndef Database_info

//合适的connetcor版本 和 以下数据类型  必须正确才可正确连接远程数据库
const char db_user[] = "htc";         //username  
const char db_passwd[] = "123456";         //password  
const char db_host[] = "139.224.117.218";    //or"127.0.0.1"or自己的数据库ip  
const char db_name[] = "battleworld";        //database  
unsigned int db_port = 3306;           //server port 

/*
const char db_user[] = "root";         //username  
const char db_passwd[] = "root";         //password  
const char db_host[] = "localhost";    //or"127.0.0.1"or自己的数据库ip  
const char db_name[] = "battleworld";        //database  
unsigned int db_port = 3306;           //server port 
*/
#endif