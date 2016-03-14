#ifndef PARENTDB_H
#define PARENTDB_H

#include <mongo/client/dbclient_rs.h>

#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteException.h"

class ParentDB
{
public:
    ParentDB();
    virtual ~ParentDB();

    bool InformerLoadAll();
    bool InformerUpdate(mongo::Query);
    void InformerRemove(const std::string &id);

private:
    bool fConnectedToMainDatabase;
    Kompex::SQLiteDatabase *pdb;
    char buf[262144];
    mongo::DBClientReplicaSet *monga_main;

    void logDb(const Kompex::SQLiteException &ex) const;


    bool ConnectMainDatabase();
};

#endif // PARENTDB_H
