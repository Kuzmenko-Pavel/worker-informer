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
    std::string informer_id_;
    long long informer_id_int_;
    std::string get_;
    std::string post_;
    unsigned int capacity_;
    int rating_division_;
    Params();
    Params &informer_id(const std::string &informer_id);
    Params &informer_id_int(long long &informer_id_int);
    Params &capacity(unsigned int &capacity);
    Params &rating_division(int &rating_division);
    Params &country(const std::string &country);
    Params &region(const std::string &region);
    Params &test_mode(bool test_mode);
    Params &get(const std::string &get);
    Params &post(const std::string &post);
    std::string getCountry() const;
    std::string getRegion() const;
    std::string getInformerId() const;
    bool isTestMode() const;
    std::string toJson() const;

private:
    std::string country_;
    std::string region_;
};

#endif // PARAMS_H
