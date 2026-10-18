/*
 * File   : LichPhatSong.cpp
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 */
#include "LichPhatSong.h"
#include <iomanip>
#include <iostream>
#include "InputHelper.h"
#include "Validator.h"

using namespace std;

void LichPhatSong::datMa(const string& id) {
    string s = Validator::toUpper(Validator::trim(id));
    if (!Validator::matchId(s, 'L', 3))
        throw ValidationException("Ma lich phai co dang Lxxx (x la chu so), vi du L001");
    ma_ = s;
}

void LichPhatSong::datMaKenh(const string& cid) {
    string s = Validator::toUpper(Validator::trim(cid));
    if (!Validator::matchId(s, 'K', 3))
        throw ValidationException("Ma kenh phai co dang Kxxx (x la chu so), vi du K001");
    maKenh_ = s;
}

void LichPhatSong::datTenChuongTrinh(const string& title) { tenChuongTrinh_ = Validator::checkText(title, "Ten chuong trinh", 60); }

void LichPhatSong::kiemTraKhoang(const TimeOfDay& start, const TimeOfDay& end) {
    if (end.minutes() <= start.minutes()) throw ValidationException("Gio ket thuc phai sau gio bat dau");
}

void LichPhatSong::datKhungGio(const TimeOfDay& start, const TimeOfDay& end) {
    kiemTraKhoang(start, end);
    gioBatDau_ = start;
    gioKetThuc_ = end;
}

bool LichPhatSong::trungKhungGio(const LichPhatSong& o) const {
    if (maKenh_ != o.maKenh_ || !(ngay_ == o.ngay_)) return false;
    return gioBatDau_.minutes() < o.gioKetThuc_.minutes() && o.gioBatDau_.minutes() < gioKetThuc_.minutes();
}

string LichPhatSong::loaiThanhChuoi(LoaiPhat t) {
    switch (t) {
        case LoaiPhat::TrucTiep: return "TRUC_TIEP";
        case LoaiPhat::PhatLai: return "PHAT_LAI";
        default: return "MOI";
    }
}

LichPhatSong::LoaiPhat LichPhatSong::loaiTuChuoi(const string& raw) {
    string s = Validator::toUpper(Validator::trim(raw));
    if (s == "TRUC_TIEP") return LoaiPhat::TrucTiep;
    if (s == "PHAT_LAI") return LoaiPhat::PhatLai;
    if (s == "MOI") return LoaiPhat::Moi;
    throw ValidationException("Loai phat khong hop le (TRUC_TIEP, PHAT_LAI, MOI): " + s);
}

string LichPhatSong::toFileString() const {
    return ma_ + "|" + maKenh_ + "|" + tenChuongTrinh_ + "|" + ngay_.toString() + "|" + gioBatDau_.toString() + "|" +
           gioKetThuc_.toString() + "|" + loaiThanhChuoi(loai_);
}

void LichPhatSong::fromFileString(const string& line) {
    auto f = Validator::split(line, '|');
    if (f.size() != 7) throw ValidationException("Ban ghi lich phai co 7 truong, gap " + to_string(f.size()));
    datMa(f[0]);
    datMaKenh(f[1]);
    datTenChuongTrinh(f[2]);
    datNgay(Date::parse(f[3]));
    datKhungGio(TimeOfDay::parse(f[4]), TimeOfDay::parse(f[5]));
    datLoai(loaiTuChuoi(f[6]));
}

void LichPhatSong::displayTableHeader() const {
    cout << left << setw(5) << "STT" << setw(7) << "Ma" << setw(7) << "Kenh" << setw(20) << "Ten kenh" << setw(30)
         << "Chuong trinh" << setw(12) << "Ngay" << setw(13) << "Khung gio" << "Loai" << "\n";
    cout << string(105, '-') << "\n";
}

void LichPhatSong::inDongCoTenKenh(int stt, const string& tenKenh) const {
    cout << left << setw(5) << stt << setw(7) << ma_ << setw(7) << maKenh_ << setw(20)
         << Validator::fit(tenKenh, 18) << setw(30) << Validator::fit(tenChuongTrinh_, 28) << setw(12)
         << ngay_.toString() << setw(13) << (gioBatDau_.toString() + "-" + gioKetThuc_.toString()) << loaiThanhChuoi(loai_)
         << "\n";
}

void LichPhatSong::displayTableRow(int stt) const { inDongCoTenKenh(stt, ""); }

void LichPhatSong::display() const {
    cout << "  Ma lich        : " << ma_ << "\n"
         << "  Ma kenh        : " << maKenh_ << "\n"
         << "  Chuong trinh   : " << tenChuongTrinh_ << "\n"
         << "  Ngay phat      : " << ngay_.toString() << "\n"
         << "  Gio bat dau    : " << gioBatDau_.toString() << "\n"
         << "  Gio ket thuc   : " << gioKetThuc_.toString() << "\n"
         << "  Loai phat song : " << loaiThanhChuoi(loai_) << "\n";
}

void LichPhatSong::nhapKhungGio(bool update) {
    while (true) {
        Date d = ngay_;
        TimeOfDay st = gioBatDau_, en = gioKetThuc_;
        if (update) {
            InputHelper::updateField("Ngay phat (dd/mm/yyyy)", d.toString(), [&](const string& x) { d = Date::parse(x); });
            InputHelper::updateField("Gio bat dau (HH:MM)", st.toString(), [&](const string& x) { st = TimeOfDay::parse(x); });
            InputHelper::updateField("Gio ket thuc (HH:MM)", en.toString(), [&](const string& x) { en = TimeOfDay::parse(x); });
        } else {
            InputHelper::promptField("Ngay phat (dd/mm/yyyy)", [&](const string& x) { d = Date::parse(x); });
            InputHelper::promptField("Gio bat dau (HH:MM)", [&](const string& x) { st = TimeOfDay::parse(x); });
            InputHelper::promptField("Gio ket thuc (HH:MM)", [&](const string& x) { en = TimeOfDay::parse(x); });
        }
        try {
            LichPhatSong probe = *this;
            probe.datNgay(d);
            probe.datKhungGio(st, en);
            if (kiemTraKhungGio_) kiemTraKhungGio_(probe);  // vd: khong trung khung gio voi lich khac
            ngay_ = d;
            gioBatDau_ = st;
            gioKetThuc_ = en;
            return;
        } catch (const ValidationException& e) {
            cout << "  Loi: " << e.what() << ". Vui long nhap lai ngay va khung gio.\n";
        }
    }
}

void LichPhatSong::nhapLoai(bool update) {
    cout << "  Loai phat song: 1 = Truc tiep, 2 = Phat lai, 3 = Moi\n";
    auto apply = [&](const string& s) {
        int v = Validator::toInt(s);
        if (v < 1 || v > 3) throw ValidationException("Chi nhap 1, 2 hoac 3");
        loai_ = static_cast<LoaiPhat>(v);
    };
    if (update)
        InputHelper::updateField("Loai phat song", to_string(static_cast<int>(loai_)), apply);
    else
        InputHelper::promptField("Loai phat song (1-3)", apply);
}

void LichPhatSong::inputData() {
    InputHelper::promptField("Ma lich (Lxxx)", [&](const string& s) {
        datMa(s);
        if (kiemTraMa_) kiemTraMa_(ma_);
    });
    InputHelper::promptField("Ma kenh (Kxxx)", [&](const string& s) {
        datMaKenh(s);
        if (kiemTraKenh_) kiemTraKenh_(maKenh_);
    });
    InputHelper::promptField("Ten chuong trinh", [&](const string& s) { datTenChuongTrinh(s); });
    nhapKhungGio(false);
    nhapLoai(false);
}

void LichPhatSong::updateData() {
    InputHelper::updateField("Ma kenh", maKenh_, [&](const string& s) {
        string old = maKenh_;  // khoi phuc neu ma moi khong hop le
        try {
            datMaKenh(s);
            if (kiemTraKenh_) kiemTraKenh_(maKenh_);
        } catch (...) {
            maKenh_ = old;
            throw;
        }
    });
    InputHelper::updateField("Ten chuong trinh", tenChuongTrinh_, [&](const string& s) { datTenChuongTrinh(s); });
    nhapKhungGio(true);
    nhapLoai(true);
}
