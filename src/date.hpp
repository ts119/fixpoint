#ifndef __DATE_HPP__
#define __DATE_HPP__

#include <string>
#include <iostream>

class Date{
    private:
    typedef std::string string;
    public:
    //YYYYMMDDHHMMSS
    string date;

    int year, mon, day, hour, min, sec;

    Date(const string& date_): date(date_) {
        year = std::stoi(date.substr(0,4));
        mon = std::stoi(date.substr(4,2));
        day = std::stoi(date.substr(6,2));
        hour = std::stoi(date.substr(8,2));
        min = std::stoi(date.substr(10,2));
        sec = std::stoi(date.substr(12,2));
    }

};

#endif