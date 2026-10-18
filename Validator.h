/*
 * File   : Validator.h
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 * Mo ta  : Cac ham tien ich kiem tra va xu ly chuoi dung chung.
 */
#pragma once
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include "Exceptions.h"

namespace Validator {

/** Cat khoang trang hai dau chuoi. */
inline std::string trim(const std::string& s) {
    size_t b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
    return s.substr(b, e - b);
}

inline std::string toUpper(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

inline std::string toLower(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

/** Tim chuoi con khong phan biet hoa thuong. */
inline bool containsIgnoreCase(const std::string& hay, const std::string& needle) {
    return toLower(hay).find(toLower(needle)) != std::string::npos;
}

/** Kiem tra ma co dang <prefix><digits chu so>, vi du K001. */
inline bool matchId(const std::string& s, char prefix, size_t digits) {
    if (s.size() != digits + 1 || s[0] != prefix) return false;
    for (size_t i = 1; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

/** Kiem tra chuoi van ban chung: khong rong, khong qua maxLen, khong chua '|'. */
inline std::string checkText(const std::string& raw, const std::string& label, size_t maxLen) {
    std::string s = trim(raw);
    if (s.empty()) throw ValidationException(label + " khong duoc de trong");
    if (s.size() > maxLen)
        throw ValidationException(label + " toi da " + std::to_string(maxLen) + " ky tu");
    if (s.find('|') != std::string::npos)
        throw ValidationException(label + " khong duoc chua ky tu '|'");
    return s;
}

/** Chuyen chuoi sang so nguyen; nem ValidationException neu sai dinh dang. */
inline int toInt(const std::string& raw) {
    std::string s = trim(raw);
    if (s.empty()) throw ValidationException("Gia tri khong duoc de trong");
    size_t i = (s[0] == '-') ? 1 : 0;
    if (i == s.size() || s.size() - i > 9) throw ValidationException("Phai nhap so nguyen hop le");
    for (size_t k = i; k < s.size(); ++k)
        if (!std::isdigit(static_cast<unsigned char>(s[k])))
            throw ValidationException("Phai nhap so nguyen hop le");
    return std::stoi(s);
}

/** Tach chuoi theo ky tu phan cach. */
inline std::vector<std::string> split(const std::string& s, char sep) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == sep) { out.push_back(cur); cur.clear(); }
        else cur += c;
    }
    out.push_back(cur);
    return out;
}

/** Cat chuoi cho vua cot khi in bang. */
inline std::string fit(const std::string& s, size_t width) {
    if (s.size() <= width) return s;
    if (width <= 2) return s.substr(0, width);
    return s.substr(0, width - 2) + "..";
}

}  // namespace Validator
