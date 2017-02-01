#ifndef PARENTDB_H
#define PARENTDB_H

#include <mongocxx/pool.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteException.h"

class ParentDB
{
public:
    ParentDB();
    virtual ~ParentDB();

    bool InformerLoadAll();
    bool InformerUpdate(bsoncxx::builder::stream::document);
    void InformerRemove(const std::string &id);

private:
    bool fConnectedToMainDatabase;
    Kompex::SQLiteDatabase *pdb;
    char buf[262144];
    void logDb(const Kompex::SQLiteException &ex) const;
    mongocxx::pool *monga_main;
    bool ConnectMainDatabase();
};

#endif // PARENTDB_H
