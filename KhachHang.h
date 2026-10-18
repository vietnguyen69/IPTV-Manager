/*
 * KhachHang.h
 * Lop quan ly Khach Hang dich vu truyen hinh so/IPTV
 * Ke thua tu Entity
 * De tai 7: Quan ly dich vu truyen hinh so/IPTV
 */
#ifndef KHACH_HANG_H
#define KHACH_HANG_H

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include "Entity.h"
#include "Utils.h"

using namespace std;

/*
 * Lop KhachHang - Quan ly thong tin khach hang IPTV
 * Thuoc tinh:
 *   - maKH        : Ma dinh danh khach hang (dang KHxxx, VD: KH001)
 *   - hoTen       : Ho va ten khach hang
 *   - soCCCD      : So can cuoc cong dan
 *   - soDienThoai : So dien thoai lien lac
 *   - email       : Dia chi email
 *   - diaChi      : Dia chi lap dat
 *   - ngayDangKy  : Ngay dang ky dich vu (dd/mm/yyyy)
 *   - maGoiKenh   : Ma goi kenh dang su dung (khoa ngoai tham chieu GoiKenh)
 *   - trangThai   : Trang thai tai khoan (Hoat dong / Tam ngung / Da huy)
 */
class KhachHang : public Entity {
private:
    string hoTen;
    string soCCCD;
    string soDienThoai;
    string email;
    string diaChi;
    string ngayDangKy;  // Dinh dang dd/mm/yyyy
    string maGoiKenh;   // Khoa ngoai -> GoiKenh
    string trangThai;   // "Hoat dong", "Tam ngung", "Da huy"

    // Kiem tra dinh dang ngay dd/mm/yyyy
    static bool isNgayHopLe(const string& ngay) {
        if (ngay.size() != 10) return false;
        if (ngay[2] != '/' || ngay[5] != '/') return false;
        try {
            int d = stoi(ngay.substr(0, 2));
            int m = stoi(ngay.substr(3, 2));
            int y = stoi(ngay.substr(6, 4));
            if (m < 1 || m > 12) return false;
            if (d < 1 || d > 31) return false;
            if (y < 1900 || y > 2100) return false;
            // Kiem tra so ngay theo thang
            int maxDay[] = {31,29,31,30,31,30,31,31,30,31,30,31};
            if (d > maxDay[m-1]) return false;
            return true;
        } catch (...) { return false; }
    }

    // Kiem tra dinh dang email co ban
    static bool isEmailHopLe(const string& mail) {
        if (mail.empty()) return true; // Email co the de trong
        size_t at = mail.find('@');
        if (at == string::npos || at == 0) return false;
        size_t dot = mail.find('.', at);
        if (dot == string::npos || dot == mail.size() - 1) return false;
        return true;
    }

    // Kiem tra so dien thoai (10 chu so, bat dau bang 0)
    static bool isSDTHopLe(const string& sdt) {
        if (sdt.size() < 9 || sdt.size() > 11) return false;
        for (char c : sdt) if (!isdigit(c)) return false;
        return true;
    }

public:
    // Constructor mac dinh
    KhachHang() : Entity(""), hoTen(""), soCCCD(""), soDienThoai(""),
                  email(""), diaChi(""), ngayDangKy(""), maGoiKenh(""),
                  trangThai("Hoat dong") {}

    // Constructor day du
    KhachHang(const string& ma, const string& ht, const string& cccd,
              const string& sdt, const string& mail, const string& dc,
              const string& ngay, const string& maGoi, const string& tt)
        : Entity(ma), hoTen(ht), soCCCD(cccd), soDienThoai(sdt),
          email(mail), diaChi(dc), ngayDangKy(ngay), maGoiKenh(maGoi),
          trangThai(tt) {}

    // ------- Getter / Setter -------
    string getHoTen() const { return hoTen; }
    void setHoTen(const string& ht) {
        if (ht.empty()) throw InvalidInputException("Ho ten khong duoc de trong!");
        if (ht.size() > 100) throw InvalidInputException("Ho ten toi da 100 ky tu!");
        hoTen = ht;
    }

    string getSoCCCD() const { return soCCCD; }
    void setSoCCCD(const string& cccd) {
        if (cccd.size() != 12 && cccd.size() != 9)
            throw InvalidInputException("So CCCD phai co 9 hoac 12 chu so!");
        for (char c : cccd)
            if (!isdigit(c)) throw InvalidInputException("So CCCD chi chua chu so!");
        soCCCD = cccd;
    }

    string getSoDienThoai() const { return soDienThoai; }
    void setSoDienThoai(const string& sdt) {
        if (!isSDTHopLe(sdt))
            throw InvalidInputException("So dien thoai khong hop le (9-11 chu so)!");
        soDienThoai = sdt;
    }

    string getEmail() const { return email; }
    void setEmail(const string& mail) {
        if (!isEmailHopLe(mail))
            throw InvalidInputException("Dia chi email khong hop le!");
        email = mail;
    }

    string getDiaChi() const { return diaChi; }
    void setDiaChi(const string& dc) {
        if (dc.empty()) throw InvalidInputException("Dia chi khong duoc de trong!");
        diaChi = dc;
    }

    string getNgayDangKy() const { return ngayDangKy; }
    void setNgayDangKy(const string& ngay) {
        if (!isNgayHopLe(ngay))
            throw InvalidInputException("Ngay khong hop le! Dinh dang dd/mm/yyyy");
        ngayDangKy = ngay;
    }

    string getMaGoiKenh() const { return maGoiKenh; }
    void setMaGoiKenh(const string& ma) { maGoiKenh = ma; }

    string getTrangThai() const { return trangThai; }
    void setTrangThai(const string& tt) {
        if (tt != "Hoat dong" && tt != "Tam ngung" && tt != "Da huy")
            throw InvalidInputException("Trang thai khong hop le! (Hoat dong/Tam ngung/Da huy)");
        trangThai = tt;
    }

    bool isHoatDong() const { return trangThai == "Hoat dong"; }

    // ------- Cai dat cac ham thuan ao tu Entity -------

    // Nhap du lieu tu ban phim
    void nhapDuLieu() override {
        cout << "\n  --- NHAP THONG TIN KHACH HANG ---\n";
        cout << "  Ma khach hang (dang KHxxx, VD: KH001): ";
        string ma;
        getline(cin, ma);
        if (ma.empty()) throw InvalidInputException("Ma khach hang khong duoc de trong!");
        if (ma.size() < 3 || ma.substr(0, 2) != "KH")
            throw InvalidInputException("Ma khach hang phai co dang KHxxx (VD: KH001)!");
        id = ma;

        setHoTen(InputHelper::nhapChuoiBatBuoc("  Ho va ten: "));

        // Nhap CCCD voi kiem tra
        while (true) {
            try {
                string cccd = InputHelper::nhapChuoiBatBuoc("  So CCCD (9 hoac 12 so): ");
                setSoCCCD(cccd);
                break;
            } catch (const InvalidInputException& e) {
                Display::inThongBaoLoi(e.what());
            }
        }

        // Nhap SĐT voi kiem tra
        while (true) {
            try {
                string sdt = InputHelper::nhapChuoiBatBuoc("  So dien thoai (9-11 so): ");
                setSoDienThoai(sdt);
                break;
            } catch (const InvalidInputException& e) {
                Display::inThongBaoLoi(e.what());
            }
        }

        // Nhap email (tuy chon)
        while (true) {
            try {
                string mail = InputHelper::nhapChuoi("  Email (co the de trong): ");
                setEmail(mail);
                break;
            } catch (const InvalidInputException& e) {
                Display::inThongBaoLoi(e.what());
            }
        }

        setDiaChi(InputHelper::nhapChuoiBatBuoc("  Dia chi lap dat: "));

        // Nhap ngay dang ky voi kiem tra
        while (true) {
            try {
                string ngay = InputHelper::nhapChuoiBatBuoc("  Ngay dang ky (dd/mm/yyyy): ");
                setNgayDangKy(ngay);
                break;
            } catch (const InvalidInputException& e) {
                Display::inThongBaoLoi(e.what());
            }
        }

        maGoiKenh = InputHelper::nhapChuoi("  Ma goi kenh dang su dung (de trong neu chua co): ");

        cout << "  Trang thai tai khoan:\n"
             << "    1. Hoat dong\n"
             << "    2. Tam ngung\n"
             << "    3. Da huy\n";
        int tt = InputHelper::nhapInt("  Chon trang thai (1-3): ", 1, 3);
        vector<string> dsTT = {"Hoat dong", "Tam ngung", "Da huy"};
        trangThai = dsTT[tt - 1];
    }

    // Hien thi chi tiet
    void hienThi() const override {
        cout << "\n";
        Display::inDuongKe(65);
        cout << "  CHI TIET KHACH HANG\n";
        Display::inDuongKe(65);
        cout << "  Ma khach hang   : " << id << "\n";
        cout << "  Ho va ten       : " << hoTen << "\n";
        cout << "  So CCCD         : " << soCCCD << "\n";
        cout << "  So dien thoai   : " << soDienThoai << "\n";
        cout << "  Email           : " << (email.empty() ? "(Chua cap nhat)" : email) << "\n";
        cout << "  Dia chi         : " << diaChi << "\n";
        cout << "  Ngay dang ky    : " << ngayDangKy << "\n";
        cout << "  Ma goi kenh     : " << (maGoiKenh.empty() ? "(Chua dang ky goi)" : maGoiKenh) << "\n";
        cout << "  Trang thai      : " << trangThai << "\n";
        Display::inDuongKe(65);
    }

    // Hien thi 1 dong trong bang danh sach
    void hienThiDanhSach() const override {
        string ten = hoTen.size() > 20 ? hoTen.substr(0, 20) : hoTen;
        cout << "  " << left
             << setw(8)  << id
             << setw(22) << ten
             << setw(13) << soDienThoai
             << setw(12) << ngayDangKy
             << setw(10) << (maGoiKenh.empty() ? "---" : maGoiKenh)
             << trangThai << "\n";
    }

    // Chuyen sang chuoi luu file
    string toFileString() const override {
        ostringstream oss;
        oss << id << "|" << hoTen << "|" << soCCCD << "|"
            << soDienThoai << "|" << email << "|" << diaChi << "|"
            << ngayDangKy << "|" << maGoiKenh << "|" << trangThai;
        return oss.str();
    }

    // Doc tu chuoi trong file
    void fromFileString(const string& line) override {
        vector<string> parts = StrUtils::split(line, '|');
        if (parts.size() < 9)
            throw InvalidInputException("Dinh dang dong file KhachHang khong hop le: " + line);
        id           = StrUtils::trim(parts[0]);
        hoTen        = StrUtils::trim(parts[1]);
        soCCCD       = StrUtils::trim(parts[2]);
        soDienThoai  = StrUtils::trim(parts[3]);
        email        = StrUtils::trim(parts[4]);
        diaChi       = StrUtils::trim(parts[5]);
        ngayDangKy   = StrUtils::trim(parts[6]);
        maGoiKenh    = StrUtils::trim(parts[7]);
        trangThai    = StrUtils::trim(parts[8]);
    }

    // In tieu de bang
    static void inTieuDeBang() {
        cout << "  " << left
             << setw(8)  << "Ma KH"
             << setw(22) << "Ho va ten"
             << setw(13) << "Dien thoai"
             << setw(12) << "Ngay DK"
             << setw(10) << "Ma goi"
             << "Trang thai\n";
        cout << "  " << string(80, '-') << "\n";
    }
};

#endif // KHACH_HANG_H
