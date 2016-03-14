#include "Core_DataBase.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <vector>
#include <map>
#include <chrono>
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"
#include "Config.h"
#include "../config.h"

#define CMD_SIZE 2621440

Core_DataBase::Core_DataBase():
    len(CMD_SIZE)
{
    cmd = new char[len];
}

Core_DataBase::~Core_DataBase()
{
    delete []cmd;
}
//-------------------------------------------------------------------------------------------------------------------
bool Core_DataBase::getInformer(const std::string informer_id)
{
    #ifdef DEBUG
        auto start = std::chrono::high_resolution_clock::now();
        printf("%s\n","------------------------------------------------------------------");
    #endif // DEBUG
    bool ret = false;
    Kompex::SQLiteStatement *pStmt;

    informer = nullptr;

    bzero(cmd,sizeof(cmd));
    sqlite3_snprintf(CMD_SIZE, cmd, cfg->informerSqlStr.c_str(), informer_id.c_str());

    try
    {
        pStmt = new Kompex::SQLiteStatement(cfg->pDb->pDatabase);

        pStmt->Sql(cmd);

        while(pStmt->FetchRow())
        {
            informer =  new Informer(pStmt->GetColumnInt64(0),
                                     pStmt->GetColumnString(1),
                                     pStmt->GetColumnInt(2),
                                     pStmt->GetColumnString(3),
                                     pStmt->GetColumnString(4),
                                     pStmt->GetColumnString(5),
                                     pStmt->GetColumnString(6),
                                     pStmt->GetColumnDouble(7),
                                     pStmt->GetColumnDouble(8),
                                     pStmt->GetColumnDouble(9),
                                     pStmt->GetColumnDouble(10),
                                     cfg->range_category_,
                                     pStmt->GetColumnInt(11),
                                     pStmt->GetColumnBool(12),
                                     pStmt->GetColumnString(13),
                                     pStmt->GetColumnString(14),
                                     pStmt->GetColumnBool(15),
                                     pStmt->GetColumnBool(16),
                                     pStmt->GetColumnBool(17)
                                    );
            ret = true;
            break;
        }

        pStmt->FreeQuery();

        delete pStmt;
    }
    catch(Kompex::SQLiteException &ex)
    {
        std::clog<<"["<<pthread_self()<<"] error: "<<__func__
                 <<ex.GetString()
                 <<" \n"
                 <<cmd
                 <<std::endl;
    }
    #ifdef DEBUG
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        printf("Time %s taken: %lld \n", __func__,  microseconds);
        printf("%s\n","------------------------------------------------------------------");
    #endif // DEBUG

    return ret;
}
bool Core_DataBase::clear()
{
    if(informer)
        delete informer;
}
