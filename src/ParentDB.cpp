#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "../config.h"
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/read_preference.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <mongocxx/options/find.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/types/value.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <KompexSQLiteStatement.h>

#include "ParentDB.h"
#include "Log.h"
#include "json.h"
#include "Config.h"

using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;
using mongocxx::options::find;
using mongocxx::read_preference;

mongocxx::instance instance{};

ParentDB::ParentDB()
{
    pdb = Config::Instance()->pDb->pDatabase;
}

ParentDB::~ParentDB()
{
}


bool ParentDB::InformerUpdate(document &query)
{
    mongocxx::client conn{mongocxx::uri{cfg->mongo_main_url_}};
    conn.read_preference(read_preference(read_preference::read_mode::k_secondary_preferred));
    auto coll = conn[cfg->mongo_main_db_]["informer"];
    //find opts{};
    //auto filter = document{};
    //filter.append(kvp("_id", 1));
    //opts.projection(filter.extract());
    //auto cursor = coll.find(query.view(), opts);
    auto cursor = coll.find(query.view());
    Kompex::SQLiteStatement *pStmt;
    long long long_id = 0;
    pStmt = new Kompex::SQLiteStatement(pdb);
    std::vector<std::string> items;
    try{
        for (auto &&doc : cursor)
        {
               items.push_back(bsoncxx::to_json(doc));
        }
        for(auto i : items) {
           nlohmann::json x = nlohmann::json::parse(i);
           std::string id = x["guid"].get<std::string>();
           boost::to_lower(id);
            if (id.empty())
           {

                continue;
            }

        long_id = x["guid_int"].get<long long>();
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
        nlohmann::json capacity_element = x["admaker"]["Main"]["itemsNumber"];
        nlohmann::json header_html = x["admaker"]["MainHeader"]["html"];
        nlohmann::json footer_html = x["admaker"]["MainFooter"]["html"];
        nlohmann::json nonrelevant_element = x["nonRelevant"]["action"];
        nlohmann::json user_code_element = x["nonRelevant"]["userCode"];
        if (capacity_element.is_number())
        {
            capacity = capacity_element.get<int>();
        }
        else if (capacity_element.is_string())
        {
            capacity = boost::lexical_cast<int>(capacity_element.get<std::string>());
        }
        if (nonrelevant_element.get<std::string>() == "usercode")
        {
            social_branch = 0;
        }
        headerHtml = header_html.get<std::string>();
        footerHtml = footer_html.get<std::string>();
        user_code = user_code_element.get<std::string>();

        std::string css;
        css = x["css"].get<std::string>();
        cfg->minifyhtml(css);

        if (find)
        {
            bzero(buf,sizeof(buf));
            sqlite3_snprintf(sizeof(buf),buf,
                             "UPDATE Informer SET title='%q',account='%q',domain='%q',teasersCss='%q',headerHtml='%q',footerHtml='%q',\
                              social_branch=%d, valid=1,height=%d,width=%d,height_banner=%d,width_banner=%d,capacity=%d, auto_reload=%d,\
                              blinking=%d, shake=%d, blinking_reload=%d, shake_reload=%d, shake_mouse=%d,\
                              range_short_term=%f, range_long_term=%f, range_context=%f, range_search=%f, retargeting_capacity=%u, user_code='%q', html_notification=%d, place_branch=%d, retargeting_branch=%d,\
                              rating_division=%d\
                              WHERE id=%lld;",
                             x["title"].get<std::string>().c_str(),
                             x["user"].get<std::string>().c_str(),
                             x["domain"].get<std::string>().c_str(),
                             css.c_str(),
                             headerHtml.c_str(),
                             footerHtml.c_str(),
                             social_branch,
                             x["height"].get<int>(),
                             x["width"].get<int>(),
                             x["height_banner"].get<int>(),
                             x["width_banner"].get<int>(),
                             capacity,
                             x["auto_reload"].is_number() ? x["auto_reload"].get<int>() : 0,
                             x["blinking"].is_number() ? x["blinking"].get<int>() : 0,
                             x["shake"].is_number() ? x["shake"].get<int>() : 0,
                             x["blinking_reload"].get<bool>() ? 1 : 0,
                             x["shake_reload"].get<bool>() ? 1 : 0,
                             x["shake_mouse"].get<bool>() ? 1 : 0,
                             x["range_short_term"].is_number() ? x["range_short_term"].get<float>() : cfg->range_short_term_,
                             x["range_long_term"].is_number() ? x["range_long_term"].get<float>() : cfg->range_long_term_,
                             x["range_context"].is_number() ? x["range_context"].get<float>() : cfg->range_context_,
                             x["range_search"].is_number() ? x["range_search"].get<float>() : cfg->range_search_,
                             x["retargeting_capacity"].is_number() ?
                                (unsigned)(capacity * x["retargeting_capacity"].get<float>()) :
                                (unsigned)(cfg->retargeting_percentage_ * capacity / 100),
                             user_code.c_str(),
                             x["html_notification"].get<bool>() ? 1 : 0,
                             x["plase_branch"].get<bool>() ? 1 : 0,
                             x["retargeting_branch"].get<bool>() ? 1 : 0,
                             x["rating_division"].is_number() ? x["rating_division"].get<float>() : 1000,
                             long_id
                            );
        }
        else
        {
            bzero(buf,sizeof(buf));
            sqlite3_snprintf(sizeof(buf),buf,
                             "INSERT OR IGNORE INTO Informer(id,guid,title, account, domain, teasersCss,headerHtml,footerHtml,\
                              social_branch,valid,height,width,height_banner,width_banner,capacity, auto_reload,\
                              blinking, shake, blinking_reload, shake_reload, shake_mouse,\
                              range_short_term, range_long_term, range_context, range_search, retargeting_capacity, user_code, html_notification, place_branch, retargeting_branch,\
                              rating_division\
                              ) VALUES(\
                              %lld,'%q','%q','%q','%q','%q','%q','%q',\
                              %d,1,%d,%d,%d,%d,%d, %d,\
                              %d,%d,%d,%d, %d,\
                              %f,%f,%f,%f,%u,'%q',%d,%d,%d,%d);",
                             long_id,
                             id.c_str(),
                             x["title"].get<std::string>().c_str(),
                             x["user"].get<std::string>().c_str(),
                             x["domain"].get<std::string>().c_str(),
                             css.c_str(),
                             headerHtml.c_str(),
                             footerHtml.c_str(),
                             social_branch,
                             x["height"].get<int>(),
                             x["width"].get<int>(),
                             x["height_banner"].get<int>(),
                             x["width_banner"].get<int>(),
                             capacity,
                             x["auto_reload"].is_number() ? x["auto_reload"].get<int>() : 0,
                             x["blinking"].is_number() ? x["blinking"].get<int>() : 0,
                             x["shake"].is_number() ? x["shake"].get<int>() : 0,
                             x["blinking_reload"].get<bool>() ? 1 : 0,
                             x["shake_reload"].get<bool>() ? 1 : 0,
                             x["shake_mouse"].get<bool>() ? 1 : 0,
                             x["range_short_term"].is_number() ? x["range_short_term"].get<float>() : cfg->range_short_term_,
                             x["range_long_term"].is_number() ? x["range_long_term"].get<float>() : cfg->range_long_term_,
                             x["range_context"].is_number() ? x["range_context"].get<float>() : cfg->range_context_,
                             x["range_search"].is_number() ? x["range_search"].get<float>() : cfg->range_search_,
                             x["retargeting_capacity"].is_number() ?
                                (unsigned)(capacity * x["retargeting_capacity"].get<float>()) :
                                (unsigned)(cfg->retargeting_percentage_ * capacity / 100),
                             user_code.c_str(),
                             x["html_notification"].get<bool>() ? 1 : 0,
                             x["plase_branch"].get<bool>() ? 1 : 0,
                             x["retargeting_branch"].get<bool>() ? 1 : 0,
                             x["rating_division"].is_number() ? x["rating_division"].get<float>() : 1000
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

    //Log::info("informer %s removed",id.c_str());
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
