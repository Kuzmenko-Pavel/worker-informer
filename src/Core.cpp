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
        std::clog<<" informer id:"<<params->informer_id_;
        return Config::Instance()->template_error_;
    }
    prms->informer_id_int(informer->id);
    prms->capacity(informer->capacity);
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
void Core::resultHtml()
{
    retHtml =
        boost::str(boost::format(cfg->template_)
                   % informer->toJson()
                   % params->toJson()
                );
    if (params->test_mode_)
    {
        retHtml = retHtml + "<!--test-->";
    }
}
//-------------------------------------------------------------------------------------------------------------------
std::string Core::UserCode(Params *prms)
{
    startCoreTime = boost::posix_time::microsec_clock::local_time();

    params = prms;

    if(!getInformer(params->informer_id_))
    {
        std::clog<<"there is no informer id: "<<prms->getInformerId()<<std::endl;
        std::clog<<" ip:"<<params->getIP();
        std::clog<<" country:"<<params->getCountry();
        std::clog<<" region:"<<params->getRegion();
        std::clog<<" cookie:"<<params->getCookieId();
        std::clog<<" informer id:"<<params->informer_id_;
        return Config::Instance()->template_error_;
    }
    else
    {
        std::string html;
        html +="<html><head><META http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><style type=\"text/css\">html, body {padding: 0; margin: 0; border: 0;}</style></head><body>";
        html += informer->user_code;
        html +="</body></html>";
        clear();
        return html;
    }

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

    if(cfg->logIP)
        std::clog<<" ip:"<<params->getIP();

    if(cfg->logCountry)
        std::clog<<" country:"<<params->getCountry();

    if(cfg->logRegion)
        std::clog<<" region:"<<params->getRegion();

    if(cfg->logCookie)
        std::clog<<" cookie:"<<params->getCookieId();

    if(cfg->logInformerId)
        std::clog<<" informer id:"<<informer->id;
}
//-------------------------------------------------------------------------------------------------------------------
void Core::ProcessClean()
{
    request_processed_++;
    log();
    clear();
}
