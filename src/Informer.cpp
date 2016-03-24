#include <sstream>
#include "Informer.h"
#include "json.h"

Informer::Informer(long id) :
    id(id)
{
}

Informer::Informer(long id, const std::string &guid, const std::string &title, const std::string &account, const std::string &domain, unsigned int capacity, unsigned int auto_reload, const std::string &bannersCss,
                   const std::string &teasersCss,
                   const std::string &headerHtml,
                   const std::string &footerHtml,
                   double range_short_term, double range_long_term,
                   double range_context, double range_search, double range_category,
                   int retargeting_capacity, bool blocked, bool social_branch, const std::string &user_code, bool html_notification, bool plase_branch, bool retargeting_branch):
    id(id),
    guid(guid),
    title(title),
    account(account),
    domain(domain),
    capacity(capacity),
    auto_reload(auto_reload),
    bannersCss(bannersCss),
    teasersCss(teasersCss),
    headerHtml(headerHtml),
    footerHtml(footerHtml),
    retargeting_capacity(retargeting_capacity),
    range_short_term(range_short_term),
    range_long_term(range_long_term),
    range_context(range_context),
    range_search(range_search),
    range_category(range_category),
    blocked(blocked),
    social_branch(social_branch),
    user_code(user_code),
    html_notification(html_notification),
    plase_branch(plase_branch),
    retargeting_branch(retargeting_branch)

{
}

Informer::~Informer()
{
}

bool Informer::operator==(const Informer &other) const
{
    return this->id == other.id;
}

bool Informer::operator<(const Informer &other) const
{
    return this->id < other.id;
}
std::string Informer::toJson() const
{
    nlohmann::json j;
    j["informer_id_int"] = id;
    j["informer_id"] = guid;
    j["title"] = title;
    j["account"] = account;
    j["domain"] = domain;
    j["capacity"] = capacity;
    j["auto_reload"] = auto_reload;
    j["teasersCss"] = teasersCss;
    j["bannersCss"] = bannersCss;
    j["headerHtml"] = headerHtml;
    j["footerHtml"] = footerHtml;
    j["retargeting_capacity"] = retargeting_capacity;
    j["social_branch"] = social_branch;
    j["html_notification"] = html_notification;
    j["place_branch"] = plase_branch;
    j["retargeting_branch"] = retargeting_branch;

    return j.dump();
}
