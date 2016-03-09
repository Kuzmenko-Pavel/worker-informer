#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <map>

#include <ctime>
#include <cstdlib>
#include <chrono>

#include "../config.h"

#include "Config.h"
#include "Core.h"
#include "DB.h"
#include "base64.h"

Core::Core()
{
    tid = pthread_self();
    std::clog<<"["<<tid<<"]core start"<<std::endl;
}
//-------------------------------------------------------------------------------------------------------------------
Core::~Core()
{
}
//-------------------------------------------------------------------------------------------------------------------
std::string Core::Process(Params *prms)
{
    #ifdef DEBUG
        auto start = std::chrono::high_resolution_clock::now();
        printf("%s\n","/////////////////////////////////////////////////////////////////////////");
    #endif // DEBUG
    startCoreTime = boost::posix_time::microsec_clock::local_time();

    params = prms;

    if(!getInformer(params->informer_id_))
    {
        std::clog<<"there is no informer id: "<<prms->getInformerId()<<std::endl;
        std::clog<<" ip:"<<params->getIP();
        std::clog<<" country:"<<params->getCountry();
        std::clog<<" region:"<<params->getRegion();
        std::clog<<" cookie:"<<params->getCookieId();
        std::clog<<" context:"<<params->getContext();
        std::clog<<" search:"<<params->getSearch();
        std::clog<<" informer id:"<<params->informer_id_;
        std::clog<<" location:"<<params->getLocation();
        return Config::Instance()->template_error_;
    }
    //get campaign list
    getCampaign(params);

    resultHtml();

    endCoreTime = boost::posix_time::microsec_clock::local_time();
    #ifdef DEBUG
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        printf("Time %s taken: %lld \n", __func__,  microseconds);
        printf("%s\n","/////////////////////////////////////////////////////////////////////////");
    #endif // DEBUG

    return retHtml;
}
//-------------------------------------------------------------------------------------------------------------------
void Core::log()
{
    if(cfg->toLog())
    {
        std::clog<<"["<<tid<<"]";
    }
    if(cfg->logCoreTime)
    {
        std::clog<<" core time:"<< boost::posix_time::to_simple_string(endCoreTime - startCoreTime);
    }

    if(cfg->logOutPutSize)
        std::clog<<" out:"<<vResult.size();

    if(cfg->logIP)
        std::clog<<" ip:"<<params->getIP();

    if(cfg->logCountry)
        std::clog<<" country:"<<params->getCountry();

    if(cfg->logRegion)
        std::clog<<" region:"<<params->getRegion();

    if(cfg->logCookie)
        std::clog<<" cookie:"<<params->getCookieId();

    if(cfg->logContext)
        std::clog<<" context:"<<params->getContext();

    if(cfg->logSearch)
        std::clog<<" search:"<<params->getSearch();

    if(cfg->logInformerId)
        std::clog<<" informer id:"<<informer->id;

    if(cfg->logLocation)
        std::clog<<" location:"<<params->getLocation();

    if(cfg->logOutPutOfferIds || cfg->logRetargetingOfferIds)
    {
        std::clog<<" key:"<<params->getUserKey();
        if(hm->place_clean)
        {
            std::clog<<"[clean],";
        }
        std::clog<<" output ids:[";
        for(auto it = vResult.begin(); it != vResult.end(); ++it)
        {
            std::clog<<" "<<(*it)->id<<" "<<(*it)->id_int
            <<" hits:"<<(*it)->uniqueHits
            <<" rate:"<<(*it)->rating
            <<" cam:"<<(*it)->campaign_id
            <<" branch:"<<(*it)->getBranch();
        }
        std::clog<<"]";
    }
}
//-------------------------------------------------------------------------------------------------------------------
std::string Core::OffersToHtml(Offer::Vector &items, const std::string &url)
{
    std::string informer_html;
    // Получаем HTML-код информера для отображение тизера
    informer_html =
        boost::str(boost::format(cfg->template_teaser_)
                   % informer->teasersCss
                   % OffersToJson(items)
                   % informer->capacity
                   % url
                   % params->location_
                   % informer->title
                   % informer->domain
                   % informer->account
                   % params->context_
                   % params->search_
                   % cfg->redirect_script_
                   % informer->headerHtml
                   % informer->footerHtml
                   % informer->html_notification
                );
    if (params->test_mode_)
    {
        informer_html = informer_html + "<!--test-->";
    }

    return informer_html;
}
//-------------------------------------------------------------------------------------------------------------------
std::string Core::OffersToJson(Offer::Vector &items)
{
    std::stringstream json;
    json << "{item:[";
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        if (it != items.begin())
            json << ",";

        (*it)->redirect_url = base64_encode(boost::str(
                        boost::format("id=%s\ninf=%s\ntoken=%s\nurl=%s\nserver=%s\nloc=%s")
                        % (*it)->id
                        % params->informer_id_
                        % (*it)->token
                        % (*it)->url
                        % cfg->server_ip_
                        % params->location_
                    ));

        json << (*it)->toJson();
    }

    json << "]";
    if(informer->nonrelevant == "social")
    {
        json << ", nonrelevant: 'social'";
    }
    else
    {
        json << ", nonrelevant: 'usercode'";
    }
    json << boost::str(boost::format(", inf: \"%s\" ") % params->informer_id_) ;
    if(hm->place_clean)
    {
        json << ", clean:true";
    }
    else
    {
        json << ", clean:false";
    }
#ifndef DUMMY
    if(hm->retargeting_clean)
    {
        json << ", clean_retargeting:true";
    }
    else
    {
        json << ", clean_retargeting:false";
    }
    if(hm->retargeting_account_clean)
    {
        json << ", clean_account_retargeting:true";
    }
    else
    {
        json << ", clean_account_retargeting:false";
    }
#else
    json << ", clean_retargeting:false";
    json << ", clean_account_retargeting:false";
#endif
    json << "}";
    return json.str();
}
//-------------------------------------------------------------------------------------------------------------------
void Core::resultHtml()
{
    if(!vResult.empty())
    {
        if (params->json_)
            retHtml = OffersToJson(vResult);
        else
            retHtml = OffersToHtml(vResult, params->getUrl());
    }
    else
    {
        if (params->async_)
        {
            retHtml = OffersToHtml(vResult, params->getUrl());
        }
        else
        {
            if (params->json_)
            {
                retHtml = OffersToJson(vResult);
            }
            else
            {
                //fast logical bag fix
                if (!params->storage_ && informer && !params->async_)
                {
                    hm->updateUserHistory(vResult);
                }
                retHtml.clear();
            }
        }
    }
}
