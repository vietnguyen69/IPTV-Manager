/*
 * File   : DateTime.h
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 * Mo ta  : Lop Date (dd/mm/yyyy) va TimeOfDay (HH:MM) kem ham kiem tra hop le.
 */
#pragma once
#include <cctype>
#include <cstdio>
#include <string>
#include "Exceptions.h"
#include "Validator.h"

/** Ngay thang nam. */
class Date {
    int day_ = 1, month_ = 1, year_ = 2000;

public:
    Date() = default;
    Date(int d, int m, int y) : day_(d), month_(m), year_(y) {}

    static bool isLeap(int y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }

    static bool isValid(int d, int m, int y) {
        if (y < 2000 || y > 2100 || m < 1 || m > 12 || d < 1) return false;
        static const int dm[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int maxD = dm[m - 1] + ((m == 2 && isLeap(y)) ? 1 : 0);
        return d <= maxD;
    }

    /** Chuyen "dd/mm/yyyy" thanh Date; nem ValidationException neu sai. */
    static Date parse(const std::string& raw) {
        std::string s = Validator::trim(raw);
        bool ok = s.size() == 10 && s[2] == '/' && s[5] == '/';
        if (ok)
            for (size_t i = 0; i < s.size(); ++i)
                if (i != 2 && i != 5 && !std::isdigit(static_cast<unsigned char>(s[i]))) ok = false;
        if (!ok) throw ValidationException("Ngay phai co dang dd/mm/yyyy");
        int d = std::stoi(s.substr(0, 2)), m = std::stoi(s.substr(3, 2)), y = std::stoi(s.substr(6, 4));
        if (!isValid(d, m, y)) throw ValidationException("Ngay khong hop le (nam 2000-2100): " + s);
        return Date(d, m, y);
    }

    std::string toString() const {
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%02d/%02d/%04d", day_, month_, year_);
        return buf;
    }

    /** Khoa so sanh yyyymmdd. */
    int key() const { return year_ * 10000 + month_ * 100 + day_; }

    bool operator==(const Date& o) const { return key() == o.key(); }
    bool operator<(const Date& o) const { return key() < o.key(); }
};

/** Gio trong ngay (HH:MM). */
class TimeOfDay {
    int hour_ = 0, minute_ = 0;

public:
    TimeOfDay() = default;
    TimeOfDay(int h, int m) : hour_(h), minute_(m) {}

    static TimeOfDay parse(const std::string& raw) {
        std::string s = Validator::trim(raw);
        bool ok = s.size() == 5 && s[2] == ':';
        if (ok)
            for (size_t i = 0; i < s.size(); ++i)
                if (i != 2 && !std::isdigit(static_cast<unsigned char>(s[i]))) ok = false;
        if (!ok) throw ValidationException("Gio phai co dang HH:MM");
        int h = std::stoi(s.substr(0, 2)), m = std::stoi(s.substr(3, 2));
        if (h > 23 || m > 59) throw ValidationException("Gio khong hop le: " + s);
        return TimeOfDay(h, m);
    }

    std::string toString() const {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%02d:%02d", hour_, minute_);
        return buf;
    }

    /** So phut tinh tu 00:00. */
    int minutes() const { return hour_ * 60 + minute_; }

    bool operator<(const TimeOfDay& o) const { return minutes() < o.minutes(); }
};
