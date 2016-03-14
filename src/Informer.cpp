#include <sstream>
#include "Informer.h"
#include "json.h"

Informer::Informer(long id) :
    id(id)
{
}

Informer::Informer(long id, const std::string &title, unsigned int capacity, const std::string &bannersCss,
                   const std::string &teasersCss,
                   const std::string &headerHtml,
                   const std::string &footerHtml,
                   double range_short_term, double range_long_term,
                   double range_context, double range_search, double range_category,
                   int retargeting_capacity, bool blocked, const std::string &nonrelevant, const std::string &user_code, bool html_notification, bool plase_branch, bool retargeting_branch):
    id(id),
    title(title),
    capacity(capacity),
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
    nonrelevant(nonrelevant),
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
    std::stringstream str_json;

    str_json << "{" <<
        "\"informer_id_int\": " << id << "," <<
        "\"title\": \"" << Json::Utils::Escape(title) << "\"," <<
        "\"capacity\": " << capacity << "," <<
        "\"bannersCss\": \"" << Json::Utils::Escape(bannersCss) << "\"," <<
        "\"teasersCss\": \"" << Json::Utils::Escape(teasersCss) << "\"," <<
        "\"headerHtml\": \"" << Json::Utils::Escape(headerHtml) << "\"," <<
        "\"footerHtml\": \"" << Json::Utils::Escape(footerHtml) << "\"," <<
        "\"retargeting_capacity\": " << retargeting_capacity << "," <<
        "\"nonrelevant\": \"" << Json::Utils::Escape(nonrelevant) << "\"," <<
        "\"user_code\": \"" << Json::Utils::Escape(user_code) << "\"," <<
        "\"html_notification\": " << Json::Utils::Escape(html_notification) << "," <<
        "\"plase_branch\": " << Json::Utils::Escape(plase_branch) << "," <<
        "\"retargeting_branch\": " << Json::Utils::Escape(retargeting_branch) <<
         "}";

    return str_json.str();
}
