#include <sstream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/regex/icu.hpp>
#include <boost/date_time.hpp>
#include <iostream>
#include <string>

#include "Params.h"
#include "GeoIPTools.h"
#include "Log.h"
#include "json.h"
#include <map>


Params::Params() :
    test_mode_(false)
{
    time_ = boost::posix_time::second_clock::local_time();
}

Params &Params::ip(const std::string &ip, const std::string &qip)
{
    ip_ = ip;
    if(!qip.empty())
    {
        ip_ = qip;
    }

    country_ = geoip->country_code_by_addr(ip_);
    if (country_ == "NOT FOUND")
        region_ = "*";
    region_ = geoip->region_code_by_addr(ip_);
    return *this;
}

std::string time_t_to_string(time_t t)
{
    std::stringstream sstr;
    sstr << t;
    return sstr.str();
}

Params &Params::cookie_id(const std::string &cookie_id)
{
    if(cookie_id.empty())
    {
        cookie_id_ = time_t_to_string(time(NULL));
    }
    else
    {
        cookie_id_ = cookie_id;
        boost::u32regex replaceSymbol = boost::make_u32regex("[^0-9]");
        cookie_id_ = boost::u32regex_replace(cookie_id_ ,replaceSymbol,"");
    }
    boost::trim(cookie_id_);
    key_long = atol(cookie_id_.c_str());

    return *this;
}
Params &Params::informer_id(const std::string &informer_id)
{
    informer_id_ = informer_id;
    boost::to_lower(informer_id_);
    return *this;
}
Params &Params::informer_id_int(long long &informer_id_int)
{
    informer_id_int_ = informer_id_int;
    return *this;
}
Params &Params::capacity(unsigned int &capacity)
{
    capacity_ = capacity;
    return *this;
}
Params &Params::country(const std::string &country)
{
    if(!country.empty())
    {
        country_ = country;
    }
    return *this;
}
Params &Params::region(const std::string &region)
{
    if(!region.empty())
    {
        region_ = region;
    }
    return *this;
}
Params &Params::device(const std::string &device)
{
    if(!device.empty())
    {
        device_ = device;
    }
    else
    {
        device_ = "pc";
    }
    return *this;
}
Params &Params::test_mode(bool test_mode)
{
    test_mode_ = test_mode;
    return *this;
}

Params &Params::w(const std::string &w)
{
    w_ = w;
    return *this;
}

Params &Params::h(const std::string &h)
{
    h_ = h;
    return *this;
}

Params &Params::D(const std::string &D)
{
    D_ = D;
    return *this;
}
Params &Params::M(const std::string &M)
{
    M_ = M;
    return *this;
}
Params &Params::H(const std::string &H)
{
    H_ = H;
    return *this;
}
Params &Params::get(const std::string &get)
{
    get_ = get;
    return *this;
}
Params &Params::post(const std::string &post)
{
    post_ = post;
    return *this;
}
std::string Params::getIP() const
{
    return ip_;
}
std::string Params::getCookieId() const
{
    return cookie_id_;
}

std::string Params::getUserKey() const
{
    return cookie_id_;
}

unsigned long long Params::getUserKeyLong() const
{
    return key_long;
}

std::string Params::getCountry() const
{
    return country_;
}
std::string Params::getRegion() const
{
    return region_;
}

std::string Params::getInformerId() const
{
    return informer_id_;
}

boost::posix_time::ptime Params::getTime() const
{
    return time_;
}
bool Params::isTestMode() const
{
    return test_mode_;
}

std::string Params::getW() const
{
    return w_;
}

std::string Params::getH() const
{
    return h_;
}


std::string Params::getDevice() const
{
    return device_;
}
std::string encryptDecrypt(std::string toEncrypt, std::string ip)
{
    unsigned int content_length = 0;
    content_length = ip.length();
    char * key = new char[content_length];
    memset(key, '0', content_length);
    strcpy(key, ip.c_str());
    
    std::string output = toEncrypt;
    for (int i = 0; i < toEncrypt.size(); i++)
    {
        output[i] = toEncrypt[i] ^ key[i % (sizeof(key) / sizeof(char))];
    }
    delete [] key;
    return output;
}
std::string Params::toJson() const
{
    std::string token = encryptDecrypt("valid", ip_);
    nlohmann::json j;
    j["ip"] = ip_;
    j["informer_id"] = informer_id_;
    j["informer_id_int"] = informer_id_int_;
    j["cookie"] = cookie_id_;
    j["test"] = test_mode_;
    j["capacity"] = capacity_;
    j["country"] = country_;
    j["region"] = region_;
    j["w"] = w_;
    j["h"] = h_;
    j["M"] = M_;
    j["D"] = D_;
    j["H"] = H_;
    j["device"] = device_;
    j["request"] = "initial";
    j["token"] = token;

    return j.dump();
}
