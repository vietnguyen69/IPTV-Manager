/*
 * GoiKenh.h
 * Lop quan ly Goi Kenh truyen hinh so/IPTV
 * Ke thua tu Entity
 * De tai 7: Quan ly dich vu truyen hinh so/IPTV
 */
#ifndef GOI_KENH_H
#define GOI_KENH_H

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "Entity.h"
#include "Utils.h"

using namespace std;

class GoiKenh : public Entity {
private:
    string maGoi;
    string tenGoi;
    string moTa;
    double giaThue;     // Don vi: VND/thang
    int soKenh;
    string loaiGoi;     // "Co ban", "Nang cao", "Premium", "VOD"
    string trangThai;   // "Dang cung cap", "Ngung cung cap"

public:
    GoiKenh() : maGoi(""), tenGoi(""), moTa(""), giaThue(0.0),
                soKenh(0), loaiGoi("Co ban"), trangThai("Dang cung cap") {}

    GoiKenh(const string& ma, const string& ten, const string& mt,
            double gia, int soK, const string& loai, const string& tthai)
        : maGoi(ma), tenGoi(ten), moTa(mt), giaThue(gia),
          soKenh(soK), loaiGoi(loai), trangThai(tthai) {}

    // ------- Getter / Setter -------
    string getMaGoi() const { return maGoi; }
    void setMaGoi(const string& ma) { maGoi = ma; }

    string getTenGoi() const { return tenGoi; }
    void setTenGoi(const string& ten) {
        if (ten.empty()) throw InvalidInputException("Ten goi khong duoc de trong!");
        if (ten.size() > 100) throw InvalidInputException("Ten goi toi da 100 ky tu!");
        tenGoi = ten;
    }

    string getMoTa() const { return moTa; }
    void setMoTa(const string& mt) { moTa = mt; }

    double getGiaThue() const { return giaThue; }
    void setGiaThue(double gia) {
        if (gia <= 0) throw InvalidInputException("Gia thue phai lon hon 0!");
        giaThue = gia;
    }

    int getSoKenh() const { return soKenh; }
    void setSoKenh(int sk) {
        if (sk < 0) throw InvalidInputException("So kenh phai >= 0!");
        soKenh = sk;
    }

    string getLoaiGoi() const { return loaiGoi; }
    void setLoaiGoi(const string& loai) {
        vector<string> hopLe = {"Co ban", "Nang cao", "Premium", "VOD"};
        for (const auto& l : hopLe) {
            if (l == loai) { loaiGoi = loai; return; }
        }
        throw InvalidInputException("Loai goi khong hop le! (Co ban/Nang cao/Premium/VOD)");
    }

    string getTrangThai() const { return trangThai; }
    void setTrangThai(const string& tt) {
        if (tt != "Dang cung cap" && tt != "Ngung cung cap")
            throw InvalidInputException("Trang thai khong hop le!");
        trangThai = tt;
    }

    bool isDangCungCap() const { return trangThai == "Dang cung cap"; }

    // ------- Cai dat 9 ham thuan ao tu Entity -------
    string getId() const override { return maGoi; }
    string getDisplayName() const override { return tenGoi; }

    // Hien thi chi tiet mot ban ghi
    void display() const override {
        cout << "\n";
        Display::inDuongKe(60);
        cout << "  CHI TIET GOI KENH\n";
        Display::inDuongKe(60);
        cout << "  Ma goi      : " << maGoi << "\n";
        cout << "  Ten goi     : " << tenGoi << "\n";
        cout << "  Mo ta       : " << moTa << "\n";
        cout << "  Loai goi    : " << loaiGoi << "\n";
        cout << "  Gia thue    : " << fixed << setprecision(0) << giaThue << " VND/thang\n";
        cout << "  So kenh     : " << soKenh << " kenh\n";
        cout << "  Trang thai  : " << trangThai << "\n";
        Display::inDuongKe(60);
    }

    // In dong tieu de bang
    void displayTableHeader() const override {
        cout << "  " << left
             << setw(6)  << "STT"
             << setw(10) << "Ma goi"
             << setw(25) << "Ten goi kenh"
             << setw(14) << "Loai goi"
             << setw(16) << "Gia thue(VND)"
             << setw(10) << "So kenh"
             << "Trang thai\n";
        cout << "  " << string(95, '-') << "\n";
    }

    // Hien thi 1 dong trong bang danh sach
    void displayTableRow(int stt) const override {
        string tenShort = (tenGoi.size() > 22 ? tenGoi.substr(0, 19) + "..." : tenGoi);
        cout << "  " << left
             << setw(6)  << stt
             << setw(10) << maGoi
             << setw(25) << tenShort
             << setw(14) << loaiGoi
             << setw(16) << fixed << setprecision(0) << giaThue
             << setw(10) << soKenh
             << trangThai << "\n";
    }

    // Chuyen sang chuoi luu file (dung | lam phan cach)
    string toFileString() const override {
        ostringstream oss;
        oss << maGoi << "|" << tenGoi << "|" << moTa << "|"
            << fixed << setprecision(2) << giaThue << "|"
            << soKenh << "|" << loaiGoi << "|" << trangThai;
        return oss.str();
    }

    // Doc tu chuoi trong file
    void fromFileString(const string& line) override {
        vector<string> parts = StrUtils::split(line, '|');
        if (parts.size() < 7)
            throw InvalidInputException("Dinh dang dong file GoiKenh khong hop le: " + line);
        maGoi     = StrUtils::trim(parts[0]);
        tenGoi    = StrUtils::trim(parts[1]);
        moTa      = StrUtils::trim(parts[2]);
        giaThue   = stod(parts[3]);
        soKenh    = stoi(parts[4]);
        loaiGoi   = StrUtils::trim(parts[5]);
        trangThai = StrUtils::trim(parts[6]);
    }

    // Nhap du lieu moi tu ban phim
    void inputData() override {
        cout << "\n  --- NHAP THONG TIN GOI KENH ---\n";
        cout << "  Nhap ma goi (dang GKxxx, VD: GK001): ";
        getline(cin, maGoi);
        maGoi = StrUtils::trim(maGoi);
        if (maGoi.empty()) throw InvalidInputException("Ma goi khong duoc de trong!");
        if (maGoi.size() < 3 || maGoi.substr(0, 2) != "GK")
            throw InvalidInputException("Ma goi phai co dang GKxxx (VD: GK001)!");

        setTenGoi(InputHelper::nhapChuoiBatBuoc("  Ten goi kenh: "));
        moTa = InputHelper::nhapChuoi("  Mo ta goi kenh: ");

        cout << "  Loai goi:\n"
             << "    1. Co ban\n"
             << "    2. Nang cao\n"
             << "    3. Premium\n"
             << "    4. VOD\n";
        int chon = InputHelper::nhapInt("  Chon loai (1-4): ", 1, 4);
        vector<string> dsLoai = {"Co ban", "Nang cao", "Premium", "VOD"};
        loaiGoi = dsLoai[chon - 1];

        giaThue = InputHelper::nhapDouble("  Gia thue hang thang (VND): ", 0.01);
        soKenh  = InputHelper::nhapInt("  So luong kenh trong goi: ", 0, 9999);

        cout << "  Trang thai:\n"
             << "    1. Dang cung cap\n"
             << "    2. Ngung cung cap\n";
        int tt = InputHelper::nhapInt("  Chon trang thai (1-2): ", 1, 2);
        trangThai = (tt == 1) ? "Dang cung cap" : "Ngung cung cap";
    }

    // Cap nhat du lieu (Enter de giu nguyen)
    void updateData() override {
        string temp;
        cout << "  (Nhan Enter de giu nguyen gia tri cu)\n\n";

        setTenGoi(InputHelper::nhapChuoiTuyCon("  Ten goi kenh", tenGoi));
        moTa = InputHelper::nhapChuoiTuyCon("  Mo ta", moTa);

        cout << "  Loai goi hien tai: " << loaiGoi << "\n";
        cout << "  Loai moi (Co ban/Nang cao/Premium/VOD, Enter de giu): ";
        getline(cin, temp);
        temp = StrUtils::trim(temp);
        if (!temp.empty()) setLoaiGoi(temp);

        giaThue = InputHelper::nhapDoubleTuyCon("  Gia thue (VND/thang)", giaThue, 0.01);
        soKenh  = InputHelper::nhapIntTuyCon("  So luong kenh", soKenh, 0, 9999);

        cout << "  Trang thai hien tai: " << trangThai << "\n";
        cout << "  Trang thai moi:\n"
             << "    1. Dang cung cap\n"
             << "    2. Ngung cung cap\n"
             << "    0. Giu nguyen\n";
        int tt = InputHelper::nhapInt("  Chon (0-2): ", 0, 2);
        if (tt == 1) trangThai = "Dang cung cap";
        else if (tt == 2) trangThai = "Ngung cung cap";
    }
};

#endif // GOI_KENH_H
