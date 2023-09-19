#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>
#include "date.hpp"
#include "Date_calc.hpp"


//日付 + 経過時間(ms) = 経過した後の日付
std::string add_ms(const std::string& time, int ms){
    Date date(time);
    //ms→seconds (500ms以上切り上げ)
    int addseconds = (ms + 500)/1000;
    
    std::tm tm;
    
    tm.tm_year = date.year - 1900;
    tm.tm_mon = date.mon - 1;
    tm.tm_mday = date.day;
    tm.tm_hour = date.hour;
    tm.tm_min = date.min;
    tm.tm_sec = date.sec + addseconds;

    time_t t = mktime(&tm);

    std::string ans = static_cast<std::ostringstream&&>(std::ostringstream() << std::put_time(std::localtime(&t), "%Y%m%d%H%M%S")).str();

    //std::cout<<ans<<std::endl;

    return ans;

}