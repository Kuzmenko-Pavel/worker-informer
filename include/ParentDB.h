#ifndef PARENTDB_H
#define PARENTDB_H

#include <bsoncxx/builder/basic/document.hpp>
#include <KompexSQLiteDatabase.h>
#include <KompexSQLiteException.h>

using bsoncxx::builder::basic::document;

class ParentDB
{
public:
    ParentDB();
    virtual ~ParentDB();

    bool InformerUpdate(document &query);
    void InformerRemove(const std::string &id);

private:
    Kompex::SQLiteDatabase *pdb;
    char buf[262144];
    void logDb(const Kompex::SQLiteException &ex) const;
};

#endif // PARENTDB_H
