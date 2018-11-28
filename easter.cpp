#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>


#define EASTER_OK                0
#define EASTER_INVALID_FILENAME  1
#define EASTER_INVALID_YEARS     2
#define EASTER_IO_ERROR          3


#endif /* __PROGTEST__ */

#include <functional>
#include <vector>

enum class Easter {
    Ok = EASTER_OK,
    InvalidFilename = EASTER_INVALID_FILENAME,
    InvalidInput = EASTER_INVALID_YEARS,
    IOError = EASTER_IO_ERROR
};

bool charIsValid(const char c) {

    if (isalnum(c)) {
        return true;
    }
    return (c == '\\' or c == '/' or c == '.');

}

bool filenameIsValid(const std::string & name) {

    if (name.size() < 6) {
        return false;
    }

    if (name.rfind(".html") != name.size() -5) {
        return false;
    }

    for (const char c : name) {
        if (not (charIsValid(c))) {
            return false;
        }
    }

    return true;

}

void yearInRange(const uint64_t year) {

    if (not (1582 < year and year < 2200)) {
        throw std::runtime_error("Invalid year");
    }

} 

std::vector<uint64_t> convertInterval(const std::string & interval) {

    size_t dashPosition = interval.find("-");

    uint64_t begin = std::stoll(interval.substr(0, dashPosition));
    uint64_t end = std::stoll(interval.substr(dashPosition + 1));

    yearInRange(begin);
    yearInRange(end);

    if (begin > end) {
        throw std::runtime_error("Invalid interval");
    }

    std::vector<uint64_t> years;

    for (; begin <= end; ++begin) {
        years.emplace_back(begin);
    }

    return years;

}

std::vector<uint64_t> convertYears(const std::string & years) {

    std::stringstream ss(years);
    std::vector<uint64_t> yearsVector;

    while (ss) {

        std::string year;
        std::getline(ss, year, ',');
        if (not isdigit(ss.peek())) {
            ss.ignore(1);
        }

        if (year.find("-") != std::string::npos) {

            const std::vector<uint64_t> tmp = convertInterval(year);
            yearsVector.insert(yearsVector.end(), tmp.begin(), tmp.end());

        } else {

            uint64_t y = std::stoll(year);
            yearInRange(y);
            yearsVector.emplace_back(y);

        }

    }

    return yearsVector;

}

struct EasterDate {

    const uint16_t year;
    const uint8_t month;
    const uint8_t day;

    EasterDate(const uint16_t year, const uint8_t month, const uint8_t day)
        : year(year), month(month), day(day) { }
    
};

EasterDate getEasterDate(const uint64_t y) {

    const uint64_t a = y % 19;
    const uint64_t b = y / 100;
    const uint64_t c = y % 100;
    const uint64_t d = b / 4;
    const uint64_t e = b % 4;
    const uint64_t f = (b + 8) / 25;
    const uint64_t g = (b - f + 1) / 3;
    const uint64_t h = (9 * a + b – d – g + 15) % 30;
    const uint64_t i = c / 4;
    const uint64_t k = c % 4;
    const uint64_t l = (32 + 2 * e + 2 * i - h - k) % 7;
    const uint64_t m = (a + 11 * h + 22 * l) / 451;
    const uint64_t n = (h + l - 7 * m + 114) / 31;
    const uint64_t p = (h + l - 7 * m + 114) % 31;

    const uint8_t day = ((uint8_t)p) + 1;
    const uint8_t month = (uint8_t)n;
    const uint16_t year = (uint16_t)y;

    return EasterDate(year, month, day);

}

std::vector<EasterDate> getEasterDates(const std::vector<uint64_t> & years) {

    std::vector<EasterDate> dates;

    for (uint64_t year : years) {
        dates.emplace_back(getEasterDate(year));
    }

    return dates;

}

Easter printEasterDates(const std::string & years, const std::string & filename) {

    if (not filenameIsValid(filename)) {
        return Easter::InvalidFilename;
    }

    std::vector<uint64_t> yearsVector;

    try {
        yearsVector = convertYears(years);
    } catch (const std::exception & e) {
        return Easter::InvalidInput;
    }

    const std::vector<EasterDate> dates = getEasterDates(yearsVector);

} 


int easterReport(const char * years, const char * outFileName) {
    return (int)printEasterDates(years, outFileName);
}

#ifndef __PROGTEST__

int main(int argc, const char * argv[]) {
    printEasterDates("2012,2013,2015-2020", "nic.html");
}

#endif /* __PROGTEST__ */
