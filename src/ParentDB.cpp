#include <vector>
#include <boost/algorithm/string.hpp>
#include "../config.h"

#include "ParentDB.h"
#include "Log.h"
#include "KompexSQLiteStatement.h"
#include "json.h"
#include "Config.h"

ParentDB::ParentDB()
{
    pdb = Config::Instance()->pDb->pDatabase;
    fConnectedToMainDatabase = false;
    ConnectMainDatabase();
}

ParentDB::~ParentDB()
{
    //dtor
}


bool ParentDB::ConnectMainDatabase()
{
    if(fConnectedToMainDatabase)
        return true;

    std::vector<mongo::HostAndPort> hvec;
    for(auto h = cfg->mongo_main_host_.begin(); h != cfg->mongo_main_host_.end(); ++h)
    {
        hvec.push_back(mongo::HostAndPort(*h));
        std::clog<<"Connecting to: "<<(*h)<<std::endl;
    }

    try
    {
        if(!cfg->mongo_main_set_.empty())
        {
            monga_main = new mongo::DBClientReplicaSet(cfg->mongo_main_set_, hvec);
            monga_main->connect();
        }


        if(!cfg->mongo_main_login_.empty())
        {
            std::string err;
            if(!monga_main->auth(cfg->mongo_main_db_,cfg->mongo_main_login_,cfg->mongo_main_passwd_, err))
            {
                std::clog<<"auth db: "<<cfg->mongo_main_db_<<" login: "<<cfg->mongo_main_login_<<" error: "<<err<<std::endl;
            }
            else
            {
                fConnectedToMainDatabase = true;
            }
        }
        else
        {
            fConnectedToMainDatabase = true;
        }
    }
    catch (mongo::UserException &ex)
    {
        std::clog<<"ParentDB::"<<__func__<<" mongo error: "<<ex.what()<<std::endl;
        return false;
    }

    return true;
}
//-------------------------------------------------------------------------------------------------------
/** Загружает данные обо всех информерах */
bool ParentDB::InformerLoadAll()
{
    if(!fConnectedToMainDatabase)
        return false;
    InformerUpdate(mongo::Query());
    return true;
}

bool ParentDB::InformerUpdate(mongo::Query query)
{
    if(!fConnectedToMainDatabase)
        return false;

    std::unique_ptr<mongo::DBClientCursor> cursor = monga_main->query(cfg->mongo_main_db_ + ".informer", query);
    Kompex::SQLiteStatement *pStmt;
    long long long_id = 0;
    pStmt = new Kompex::SQLiteStatement(pdb);
    try{
    while (cursor->more())
    {
        mongo::BSONObj x = cursor->next();
        std::string id = x.getStringField("guid");
        boost::to_lower(id);
        if (id.empty())
        {
            continue;
        }

        long_id = x.getField("guid_int").numberLong();
        bzero(buf,sizeof(buf));
        sqlite3_snprintf(sizeof(buf),buf,"SELECT id FROM Informer WHERE id=%lld;", long_id);
        bool find = false; 
        try
        {
            pStmt->Sql(buf);
            while(pStmt->FetchRow())
            {
                find = true;
                break;
            }
            pStmt->FreeQuery();
        }
        catch(Kompex::SQLiteException &ex)
        {
            logDb(ex);
        }


        int capacity = 0;
        std::string headerHtml;
        std::string footerHtml;
        int social_branch = 1;
        std::string user_code;
        mongo::BSONElement capacity_element =
            x.getFieldDotted("admaker.Main.itemsNumber");
        mongo::BSONElement header_html =
            x.getFieldDotted("admaker.MainHeader.html");
        mongo::BSONElement footer_html =
            x.getFieldDotted("admaker.MainFooter.html");
        mongo::BSONElement nonrelevant_element =
            x.getFieldDotted("nonRelevant.action");
        mongo::BSONElement user_code_element =
            x.getFieldDotted("nonRelevant.userCode");
        switch (capacity_element.type())
        {
        case mongo::NumberInt:
            capacity = capacity_element.numberInt();
            break;
        case mongo::String:
            capacity =
                boost::lexical_cast<int>(capacity_element.str());
            break;
        default:
            capacity = 0;
        }
        if (nonrelevant_element.str() == "usercode")
        {
            social_branch = 0;
        }
        headerHtml = header_html.str();
        footerHtml = footer_html.str();
        user_code = user_code_element.str();

        std::string css;
        css = x.getStringField("css");
        cfg->minifyhtml(css);
       
        if (find)
        {
            bzero(buf,sizeof(buf));
            sqlite3_snprintf(sizeof(buf),buf,
                             "UPDATE Informer SET title='%q',account='%q',domain='%q',bannersCss='%q',teasersCss='%q',headerHtml='%q',footerHtml='%q',\
                              social_branch=%d, valid=1,height=%d,width=%d,height_banner=%d,width_banner=%d,capacity=%d, auto_reload=%d,\
                              range_short_term=%f, range_long_term=%f, range_context=%f, range_search=%f, retargeting_capacity=%u, user_code='%q', html_notification=%d, place_branch=%d, retargeting_branch=%d\
                              WHERE id=%lld;",
                             x.getStringField("title"),
                             x.getStringField("user"),
                             x.getStringField("domain"),
                             x.getStringField("css_banner"),
                             css.c_str(),
                             headerHtml.c_str(),
                             footerHtml.c_str(),
                             social_branch,
                             x.getIntField("height"),
                             x.getIntField("width"),
                             x.getIntField("height_banner"),
                             x.getIntField("width_banner"),
                             capacity,
                             x.hasField("auto_reload") ? x.getIntField("auto_reload") : 0,
                             x.hasField("range_short_term") ? x.getField("range_short_term").numberDouble() : cfg->range_short_term_,
                             x.hasField("range_long_term") ? x.getField("range_long_term").numberDouble() : cfg->range_long_term_,
                             x.hasField("range_context") ? x.getField("range_context").numberDouble() : cfg->range_context_,
                             x.hasField("range_search") ? x.getField("range_search").numberDouble() : cfg->range_search_,
                             x.hasField("retargeting_capacity") ?
                                (unsigned)(capacity * x.getField("retargeting_capacity").numberDouble()) :
                                (unsigned)(cfg->retargeting_percentage_ * capacity / 100),
                             user_code.c_str(),
                             x.getBoolField("html_notification") ? 1 : 0,
                             x.getBoolField("plase_branch") ? 1 : 0,
                             x.getBoolField("retargeting_branch") ? 1 : 0,
                             long_id
                            );
        }
        else
        {
            bzero(buf,sizeof(buf));
            sqlite3_snprintf(sizeof(buf),buf,
                             "INSERT OR IGNORE INTO Informer(id,guid,title, account, domain, bannersCss,teasersCss,headerHtml,footerHtml,\
                              social_branch,valid,height,width,height_banner,width_banner,capacity, auto_reload,\
                              range_short_term, range_long_term, range_context, range_search, retargeting_capacity, user_code, html_notification, place_branch, retargeting_branch) VALUES(\
                              %lld,'%q','%q','%q','%q','%q','%q','%q','%q',\
                              %d,1,%d,%d,%d,%d,%d, %d,\
                              %f,%f,%f,%f,%u,'%q',%d,%d,%d);",
                             long_id,
                             id.c_str(),
                             x.getStringField("title"),
                             x.getStringField("user"),
                             x.getStringField("domain"),
                             x.getStringField("css_banner"),
                             css.c_str(),
                             headerHtml.c_str(),
                             footerHtml.c_str(),
                             social_branch,
                             x.getIntField("height"),
                             x.getIntField("width"),
                             x.getIntField("height_banner"),
                             x.getIntField("width_banner"),
                             capacity,
                             x.hasField("auto_reload") ? x.getIntField("auto_reload") : 0,
                             x.hasField("range_short_term") ? x.getField("range_short_term").numberDouble() : cfg->range_short_term_,
                             x.hasField("range_long_term") ? x.getField("range_long_term").numberDouble() : cfg->range_long_term_,
                             x.hasField("range_context") ? x.getField("range_context").numberDouble() : cfg->range_context_,
                             x.hasField("range_search") ? x.getField("range_search").numberDouble() : cfg->range_search_,
                             x.hasField("retargeting_capacity") ?
                                (unsigned)(capacity * x.getField("retargeting_capacity").numberDouble()) :
                                (unsigned)(cfg->retargeting_percentage_ * capacity / 100),
                             user_code.c_str(),
                             x.getBoolField("html_notification") ? 1 : 0,
                             x.getBoolField("plase_branch") ? 1 : 0,
                             x.getBoolField("retargeting_branch") ? 1 : 0
                            );

        }
        try
        {
            pStmt->SqlStatement(buf);
        }
        catch(Kompex::SQLiteException &ex)
        {
            logDb(ex);
        }
        bzero(buf,sizeof(buf));
        Log::info("updated informer id %lld", long_id);
    }
    }
    catch(std::exception const &ex)
    {
        std::clog<<"["<<pthread_self()<<"]"<<__func__<<" error: "
                 <<ex.what()
                 <<" \n"
                 <<std::endl;
    }


    
    bzero(buf,sizeof(buf));

    pStmt->FreeQuery();
    delete pStmt;

    return true;
}


void ParentDB::InformerRemove(const std::string &id)
{
    Kompex::SQLiteStatement *pStmt;

    if(id.empty())
    {
        return;
    }

    pStmt = new Kompex::SQLiteStatement(pdb);
    sqlite3_snprintf(sizeof(buf),buf,"DELETE FROM Informer WHERE guid='%s';",id.c_str());
    try
    {
        pStmt->SqlStatement(buf);
    }
    catch(Kompex::SQLiteException &ex)
    {
        logDb(ex);
    }

    pStmt->FreeQuery();

    delete pStmt;

    Log::info("informer %s removed",id.c_str());
}
//-------------------------------------------------------------------------------------------------------


void ParentDB::logDb(const Kompex::SQLiteException &ex) const
{
    std::clog<<"ParentDB::logDb error: "<<ex.GetString()<<std::endl;
    std::clog<<"ParentDB::logDb request: "<<buf<<std::endl;
    #ifdef DEBUG
    printf("%s\n",ex.GetString().c_str());
    printf("%s\n",buf);
    #endif // DEBUG
}
