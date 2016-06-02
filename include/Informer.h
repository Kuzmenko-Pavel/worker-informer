#ifndef INFORMER_H
#define INFORMER_H

#include <string>

/**
    \brief Класс, описывающий рекламную выгрузку
*/
class Informer
{
public:
       long long id;                            //Индентификатор РБ
        std::string guid;
        std::string title;
        std::string account;
        std::string domain;
        unsigned int capacity;                      //Количество мест под тизер
        unsigned int auto_reload;               
        unsigned int blinking;               
        unsigned int shake ;               
        bool blinking_reload;                 
        bool shake_reload;                 
        bool shake_mouse;                 
        std::string teasersCss;                 //Стиль CSS РБ для отображения тизеров
        std::string headerHtml;
        std::string footerHtml;
        unsigned retargeting_capacity;
        double range_short_term, range_long_term, range_context, range_search, range_category;

        bool blocked;                           //Статус активности РБ
        std::string nonrelevant;            //Что отображать при отсутствии платных РП
        std::string user_code;                  //Строка пользовательского кода
        bool valid;                             //Валидность блока
        bool html_notification;                 
        bool plase_branch;                      
        bool social_branch;                      
        bool retargeting_branch;
        int rating_division;
        int height;                             //Высота блока
        int width;                              //Ширина блока
        int height_banner;                      //Высота отображаемых банеров
        int width_banner;                       //Ширина отображаемых банеров

    Informer(long id);
    Informer(long id,
            const std::string &guid,
            const std::string &title,
            const std::string &account,
            const std::string &domain,
            unsigned int capacity,
            unsigned int auto_reload,
            unsigned int blinking,          
            unsigned int shake,               
            bool blinking_reload,                 
            bool shake_reload,                
            bool shake_mouse,                 
            const std::string &teasersCss,
            const std::string &headerHtml,
            const std::string &footerHtml,
            double retargeting_capacity,
            double range_short_term,
            double range_long_term,
            double range_context,
            double range_search,
            int range_category,
            bool valid,
            bool social_branch,
            const std::string &user_code,
            bool html_notification,
            bool plase_branch,
            bool retargeting_branch,
            int rating_division
                );
    virtual ~Informer();

    bool is_null() const
    {
        return id==0;
    }

    bool operator==(const Informer &other) const;
    bool operator<(const Informer &other) const;
    std::string toJson() const;
private:

};

#endif // INFORMER_H
