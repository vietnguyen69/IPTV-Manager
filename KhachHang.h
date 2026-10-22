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
#include <stdexcept>
#include "Entity.h"
#include "Utils.h"

using namespace std;

class KhachHang : public Entity {
private:
    string maKH;
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

    // Kiem tra so dien thoai (9-11 chu so)
    static bool isSDTHopLe(const string& sdt) {
        if (sdt.size() < 9 || sdt.size() > 11) return false;
        for (char c : sdt) if (!isdigit(c)) return false;
        return true;
    }

public:
    // Constructor mac dinh
    KhachHang() : maKH(""), hoTen(""), soCCCD(""), soDienThoai(""),
                  email(""), diaChi(""), ngayDangKy(""), maGoiKenh(""),
                  trangThai("Hoat dong") {}

    // Constructor day du
    KhachHang(const string& ma, const string& ht, const string& cccd,
              const string& sdt, const string& mail, const string& dc,
              const string& ngay, const string& maGoi, const string& tt)
        : maKH(ma), hoTen(ht), soCCCD(cccd), soDienThoai(sdt),
          email(mail), diaChi(dc), ngayDangKy(ngay), maGoiKenh(maGoi),
          trangThai(tt) {}

    // ------- Getter / Setter -------
    string getMaKH() const { return maKH; }
    void setMaKH(const string& ma) { maKH = ma; }

    string getHoTen() const { return hoTen; }
    void setHoTen(const string& ht) {
        if (ht.empty()) throw InvalidInputException("Ho ten khong duoc de trong!");
        if (ht.size() > 100) throw InvalidInputException("Ho ten toi da 100 ky tu!");
        hoTen = ht;
    }

    string getSoCCCD() const { return soCCCD; }
    void setSoCCCD(const string& cccd) {
        if (cccd.empty()) throw InvalidInputException("So CCCD khong duoc de trong!");
        if (cccd.size() != 9 && cccd.size() != 12)
            throw InvalidInputException("So CCCD phai co 9 hoac 12 chu so!");
        for (char c : cccd) {
            if (!isdigit(c)) throw InvalidInputException("So CCCD chi duoc chua chu so!");
        }
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
            throw InvalidInputException("Dinh dang email khong hop le!");
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
            throw InvalidInputException("Ngay khong hop le (dinh dang dd/mm/yyyy, nam 1900-2100)!");
        ngayDangKy = ngay;
    }

    string getMaGoiKenh() const { return maGoiKenh; }
    void setMaGoiKenh(const string& maGoi) { maGoiKenh = maGoi; }

    string getTrangThai() const { return trangThai; }
    void setTrangThai(const string& tt) {
        if (tt != "Hoat dong" && tt != "Tam ngung" && tt != "Da huy")
            throw InvalidInputException("Trang thai khong hop le! (Hoat dong/Tam ngung/Da huy)");
        trangThai = tt;
    }

    // ------- Cai dat 9 ham thuan ao tu Entity -------
    string getId() const override { return maKH; }
    string getDisplayName() const override { return hoTen; }

    // Hien thi chi tiet mot ban ghi
    void display() const override {
        cout << "\n";
        Display::inDuongKe(65);
        cout << "  CHI TIET KHACH HANG\n";
        Display::inDuongKe(65);
        cout << "  Ma khach hang   : " << maKH << "\n";
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

    // In tieu de bang
    void displayTableHeader() const override {
        cout << "  " << left
             << setw(6)  << "STT"
             << setw(10) << "Ma KH"
             << setw(24) << "Ho va ten"
             << setw(14) << "So dien thoai"
             << setw(14) << "Ngay dang ky"
             << setw(12) << "Ma goi"
             << "Trang thai\n";
        cout << "  " << string(92, '-') << "\n";
    }

    // Hien thi 1 dong trong bang danh sach
    void displayTableRow(int stt) const override {
        string tenShort = (hoTen.size() > 22 ? hoTen.substr(0, 19) + "..." : hoTen);
        cout << "  " << left
             << setw(6)  << stt
             << setw(10) << maKH
             << setw(24) << tenShort
             << setw(14) << soDienThoai
             << setw(14) << ngayDangKy
             << setw(12) << (maGoiKenh.empty() ? "---" : maGoiKenh)
             << trangThai << "\n";
    }

    // Chuyen sang chuoi luu file
    string toFileString() const override {
        ostringstream oss;
        oss << maKH << "|" << hoTen << "|" << soCCCD << "|"
            << soDienThoai << "|" << email << "|" << diaChi << "|"
            << ngayDangKy << "|" << maGoiKenh << "|" << trangThai;
        return oss.str();
    }

    // Doc tu chuoi trong file
    void fromFileString(const string& line) override {
        vector<string> parts = StrUtils::split(line, '|');
        if (parts.size() < 9)
            throw InvalidInputException("Dinh dang dong file KhachHang khong hop le: " + line);
        maKH         = StrUtils::trim(parts[0]);
        hoTen        = StrUtils::trim(parts[1]);
        soCCCD       = StrUtils::trim(parts[2]);
        soDienThoai  = StrUtils::trim(parts[3]);
        email        = StrUtils::trim(parts[4]);
        diaChi       = StrUtils::trim(parts[5]);
        ngayDangKy   = StrUtils::trim(parts[6]);
        maGoiKenh    = StrUtils::trim(parts[7]);
        trangThai    = StrUtils::trim(parts[8]);
    }

    // Nhap du lieu moi tu ban phim
    void inputData() override {
        cout << "\n  --- NHAP THONG TIN KHACH HANG ---\n";
        cout << "  Ma khach hang (dang KHxxx, VD: KH001): ";
        getline(cin, maKH);
        maKH = StrUtils::trim(maKH);
        if (maKH.empty()) throw InvalidInputException("Ma khach hang khong duoc de trong!");
        if (maKH.size() < 3 || maKH.substr(0, 2) != "KH")
            throw InvalidInputException("Ma khach hang phai co dang KHxxx (VD: KH001)!");

        setHoTen(InputHelper::nhapChuoiBatBuoc("  Ho va ten: "));

        // Nhap CCCD
        while (true) {
            try {
                string cccd = InputHelper::nhapChuoiBatBuoc("  So CCCD (9 hoac 12 so): ");
                setSoCCCD(cccd);
                break;
            } catch (const InvalidInputException& e) {
                Display::inThongBaoLoi(e.what());
            }
        }

        // Nhap SĐT
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

        // Nhap ngay dang ky
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

    // Cap nhat du lieu (Enter de giu nguyen)
    void updateData() override {
        string temp;
        cout << "  (Nhan Enter de giu nguyen gia tri cu)\n\n";

        setHoTen(InputHelper::nhapChuoiTuyCon("  Ho va ten", hoTen));

        cout << "  So CCCD [" << soCCCD << "]: ";
        getline(cin, temp);
        temp = StrUtils::trim(temp);
        if (!temp.empty()) setSoCCCD(temp);

        cout << "  So dien thoai [" << soDienThoai << "]: ";
        getline(cin, temp);
        temp = StrUtils::trim(temp);
        if (!temp.empty()) setSoDienThoai(temp);

        cout << "  Email [" << email << "]: ";
        getline(cin, temp);
        temp = StrUtils::trim(temp);
        if (!temp.empty()) setEmail(temp);

        setDiaChi(InputHelper::nhapChuoiTuyCon("  Dia chi", diaChi));

        cout << "  Ngay dang ky [" << ngayDangKy << "]: ";
        getline(cin, temp);
        temp = StrUtils::trim(temp);
        if (!temp.empty()) setNgayDangKy(temp);

        cout << "  Ma goi kenh [" << maGoiKenh << "]: ";
        getline(cin, temp);
        temp = StrUtils::trim(temp);
        if (!temp.empty()) maGoiKenh = temp;

        cout << "  Trang thai hien tai: " << trangThai << "\n";
        cout << "  Trang thai moi:\n"
             << "    1. Hoat dong\n"
             << "    2. Tam ngung\n"
             << "    3. Da huy\n"
             << "    0. Giu nguyen\n";
        int tt = InputHelper::nhapInt("  Chon (0-3): ", 0, 3);
        vector<string> dsTT = {"", "Hoat dong", "Tam ngung", "Da huy"};
        if (tt > 0) trangThai = dsTT[tt];
    }
};

#endif // KHACH_HANG_H
