#ifndef POS_SERVER_HEADER
#define POS_SERVER_HEADER

//Basic Includes
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<postgresql/libpq-fe.h>

/* Networking */
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/fcntl.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>
#include<netdb.h>
#include<time.h>

#include "../defs.h"
#include "../result.h"
//#include "../connect.h"
#include "../generics.h"

#include <sstream>

#define DEFAULT_HOST 'localhost'
#define DEFAULT_PORT 3577
#define SQLEXEC(server, cmd)    mResult = PQexec(mPostgres, (cmd));
#define SERVER_ROOT_DIR "/home/art/pos/server"

/*
ALTER SEQUENCE <tablename>_<id>_seq RESTART WITH 0
*/

typedef std::unique_ptr<Result> RES_UPTR;

class DataServer {
    public:
        //ctor
        DataServer();
        //dtor
        ~DataServer();

        //Execute a command
        template <typename T>
        std::unique_ptr<Result> command(T);

    private:
        bool                    connected   = false;
        PGconn*                 pg_conn     = nullptr;
        PGresult*               pg_result   = nullptr;
        pid_t                   pid;
        Result                  cmd_result;
        std::ostringstream      sql;

        //DB command help functions
        inline int tuple_count(void) { return PQntuples(pg_result); }

        inline int read_int(int pIndex, const char* pValueName)
        {
            if(!pValueName)
                return -1;

            const std::string value = PQgetvalue(pg_result, pIndex, PQfnumber(pg_result, pValueName));
            return std::stoi(value);
        }

        inline const char* read_cstring(int pIndex, const char* pValueName)
        {
            if(!pValueName)
                return "Undefined";

            return PQgetvalue(pg_result, pIndex, PQfnumber(pg_result, pValueName));
        }

        inline double read_double(int pIndex, const char* pValueName)
        {
            if(!pValueName)
                return -1;
            return std::stof(PQgetvalue(pg_result, pIndex, PQfnumber(pg_result, pValueName)));
        }


        void run_sql(){
            pg_result = PQexec(pg_conn, sql.str().c_str());
        }
        void clr_sql(){ sql.str(""); PQclear(pg_result); }
};

typedef GenericSingleton<DataServer>    Server;

#endif //POS_SERVER_HEADER

