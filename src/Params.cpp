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
#include "Log.h"
#include "json.h"
#include <map>


Params::Params() :
    test_mode_(false)
{
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
Params &Params::rating_division(int &rating_division)
{
    rating_division_ = rating_division;
    return *this;
}
Params &Params::country(const std::string &country)
{
    country_ = country;
    return *this;
}
Params &Params::region(const std::string &region)
{
    region_ = region;
    return *this;
}
Params &Params::test_mode(bool test_mode)
{
    test_mode_ = test_mode;
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
bool Params::isTestMode() const
{
    return test_mode_;
}
std::string Params::toJson() const
{
    nlohmann::json j;
    j["informer_id"] = informer_id_;
    j["informer_id_int"] = informer_id_int_;
    j["test"] = test_mode_;
    j["capacity"] = capacity_;
    j["rating_division"] = rating_division_;
    j["country"] = country_;
    j["region"] = region_;
    j["request"] = "initial";

    return j.dump();
}
