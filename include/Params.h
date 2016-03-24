#ifndef PARAMS_H
#define PARAMS_H

#include <sstream>
#include <string>
#include <boost/date_time.hpp>
#include <boost/algorithm/string/join.hpp>
#include <vector>
#include <map>

/** \brief Параметры, которые определяют показ рекламы */
class Params
{
public:
    bool test_mode_;
    std::string ip_;
    std::string cookie_id_;
    std::string location_;
    unsigned long long key_long;
    boost::posix_time::ptime time_;
    std::string informer_id_;
    long long informer_id_int_;
    std::string w_;
    std::string h_;
    std::string D_;
    std::string M_;
    std::string H_;
    std::string device_;
    std::string get_;
    std::string post_;
    unsigned int capacity_;
    Params();
    Params &ip(const std::string &ip, const std::string &qip);
    Params &cookie_id(const std::string &cookie_id);
    Params &informer_id(const std::string &informer_id);
    Params &informer_id_int(long long &informer_id_int);
    Params &capacity(unsigned int &capacity);
    Params &country(const std::string &country);
    Params &region(const std::string &region);
    Params &test_mode(bool test_mode);
    Params &w(const std::string &w);
    Params &h(const std::string &h);
    Params &D(const std::string &D);
    Params &M(const std::string &M);
    Params &H(const std::string &H);
    Params &device(const std::string &device);
    Params &get(const std::string &get);
    Params &post(const std::string &post);
    std::string getIP() const;
    std::string getCookieId() const;
    std::string getUserKey() const;
    unsigned long long getUserKeyLong() const;
    std::string getCountry() const;
    std::string getRegion() const;
    std::string getInformerId() const;
    boost::posix_time::ptime getTime() const;
    bool isTestMode() const;
    std::string getW() const;
    std::string getH() const;
    std::string getDevice() const;
    std::string toJson() const;

private:
    std::string country_;
    std::string countryByIp_;
    std::string region_;
};

#endif // PARAMS_H
