/*
 * File   : KenhTruyenHinh.cpp
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 */
#include "KenhTruyenHinh.h"
#include <iomanip>
#include <iostream>
#include "InputHelper.h"
#include "Validator.h"

using namespace std;

void KenhTruyenHinh::datMa(const string& id) {
    string s = Validator::toUpper(Validator::trim(id));
    if (!Validator::matchId(s, 'K', 3))
        throw ValidationException("Ma kenh phai co dang Kxxx (x la chu so), vi du K001");
    ma_ = s;
}

void KenhTruyenHinh::datTen(const string& name) { ten_ = Validator::checkText(name, "Ten kenh", 40); }

void KenhTruyenHinh::datMaTheLoai(const string& genreId) {
    string s = Validator::toUpper(Validator::trim(genreId));
    if (s.size() != 4 || s.substr(0, 2) != "TL" || !isdigit((unsigned char)s[2]) || !isdigit((unsigned char)s[3]))
        throw ValidationException("Ma the loai phai co dang TLxx, vi du TL01");
    maTheLoai_ = s;
}

void KenhTruyenHinh::datDoPhanGiai(const string& res) {
    string s = Validator::toUpper(Validator::trim(res));
    if (s != "SD" && s != "HD" && s != "FHD" && s != "4K")
        throw ValidationException("Do phan giai chi nhan SD, HD, FHD hoac 4K");
    doPhanGiai_ = s;
}

void KenhTruyenHinh::datSoKenh(int number) {
    if (number < 1 || number > 999) throw ValidationException("So kenh phai tu 1 den 999");
    soKenh_ = number;
}

string KenhTruyenHinh::toFileString() const {
    return ma_ + "|" + ten_ + "|" + maTheLoai_ + "|" + doPhanGiai_ + "|" + to_string(soKenh_) + "|" +
           (dangPhat_ ? "1" : "0");
}

void KenhTruyenHinh::fromFileString(const string& line) {
    auto f = Validator::split(line, '|');
    if (f.size() != 6) throw ValidationException("Ban ghi kenh phai co 6 truong, gap " + to_string(f.size()));
    datMa(f[0]);
    datTen(f[1]);
    datMaTheLoai(f[2]);
    datDoPhanGiai(f[3]);
    datSoKenh(Validator::toInt(f[4]));
    int st = Validator::toInt(f[5]);
    if (st != 0 && st != 1) throw ValidationException("Trang thai phai la 0 hoac 1");
    datTrangThai(st == 1);
}

void KenhTruyenHinh::displayTableHeader() const {
    cout << left << setw(5) << "STT" << setw(7) << "Ma" << setw(26) << "Ten kenh" << setw(10) << "The loai"
         << setw(10) << "Phan giai" << right << setw(9) << "So kenh" << "  " << left << "Trang thai" << "\n";
    cout << string(77, '-') << "\n";
}

void KenhTruyenHinh::displayTableRow(int stt) const {
    cout << left << setw(5) << stt << setw(7) << ma_ << setw(26) << Validator::fit(ten_, 24) << setw(10) << maTheLoai_
         << setw(10) << doPhanGiai_ << right << setw(9) << soKenh_ << "  " << left
         << (dangPhat_ ? "Dang phat" : "Tam ngung") << "\n";
}

void KenhTruyenHinh::display() const {
    cout << "  Ma kenh      : " << ma_ << "\n"
         << "  Ten kenh     : " << ten_ << "\n"
         << "  Ma the loai  : " << maTheLoai_ << "\n"
         << "  Do phan giai : " << doPhanGiai_ << "\n"
         << "  So kenh      : " << soKenh_ << "\n"
         << "  Trang thai   : " << (dangPhat_ ? "Dang phat" : "Tam ngung") << "\n";
}

void KenhTruyenHinh::inputData() {
    InputHelper::promptField("Ma kenh (Kxxx)", [&](const string& s) {
        datMa(s);
        if (kiemTraMa_) kiemTraMa_(ma_);
    });
    InputHelper::promptField("Ten kenh", [&](const string& s) { datTen(s); });
    InputHelper::promptField("Ma the loai (TLxx)", [&](const string& s) { datMaTheLoai(s); });
    InputHelper::promptField("Do phan giai (SD/HD/FHD/4K)", [&](const string& s) { datDoPhanGiai(s); });
    InputHelper::promptField("So kenh (1-999)", [&](const string& s) { datSoKenh(Validator::toInt(s)); });
    InputHelper::promptField("Trang thai (1 = dang phat, 0 = tam ngung)", [&](const string& s) {
        int v = Validator::toInt(s);
        if (v != 0 && v != 1) throw ValidationException("Chi nhap 0 hoac 1");
        datTrangThai(v == 1);
    });
}

void KenhTruyenHinh::updateData() {
    InputHelper::updateField("Ten kenh", ten_, [&](const string& s) { datTen(s); });
    InputHelper::updateField("Ma the loai", maTheLoai_, [&](const string& s) { datMaTheLoai(s); });
    InputHelper::updateField("Do phan giai", doPhanGiai_, [&](const string& s) { datDoPhanGiai(s); });
    InputHelper::updateField("So kenh", to_string(soKenh_), [&](const string& s) { datSoKenh(Validator::toInt(s)); });
    InputHelper::updateField("Trang thai (1/0)", dangPhat_ ? "1" : "0", [&](const string& s) {
        int v = Validator::toInt(s);
        if (v != 0 && v != 1) throw ValidationException("Chi nhap 0 hoac 1");
        datTrangThai(v == 1);
    });
}
