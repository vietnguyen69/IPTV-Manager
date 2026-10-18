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
#include "Entity.h"
#include "Utils.h"

using namespace std;

/*
 * Lop GoiKenh - Quan ly goi kenh truyen hinh
 * Thuoc tinh:
 *   - maGoi      : Ma dinh danh goi kenh (dang GKxxx, VD: GK001)
 *   - tenGoi     : Ten goi kenh
 *   - moTa       : Mo ta goi kenh
 *   - giaThue    : Gia thue hang thang (VND)
 *   - soKenh     : So luong kenh trong goi
 *   - loaiGoi    : Loai goi (Co ban / Nang cao / Premium / VOD)
 *   - trangThai  : Trang thai goi (Dang cung cap / Ngung cung cap)
 */
class GoiKenh : public Entity {
private:
    string tenGoi;
    string moTa;
    double giaThue;     // Don vi: VND/thang
    int soKenh;
    string loaiGoi;     // "Co ban", "Nang cao", "Premium", "VOD"
    string trangThai;   // "Dang cung cap", "Ngung cung cap"

public:
    // Constructor mac dinh
    GoiKenh() : Entity(""), tenGoi(""), moTa(""), giaThue(0.0),
                soKenh(0), loaiGoi("Co ban"), trangThai("Dang cung cap") {}

    // Constructor day du
    GoiKenh(const string& ma, const string& ten, const string& moTa,
            double gia, int soK, const string& loai, const string& tthai)
        : Entity(ma), tenGoi(ten), moTa(moTa), giaThue(gia),
          soKenh(soK), loaiGoi(loai), trangThai(tthai) {}

    // ------- Getter / Setter -------
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
        for (const auto& l : hopLe)
            if (l == loai) { loaiGoi = loai; return; }
        throw InvalidInputException("Loai goi khong hop le! (Co ban/Nang cao/Premium/VOD)");
    }

    string getTrangThai() const { return trangThai; }
    void setTrangThai(const string& tt) {
        if (tt != "Dang cung cap" && tt != "Ngung cung cap")
            throw InvalidInputException("Trang thai khong hop le!");
        trangThai = tt;
    }

    bool isDangCungCap() const { return trangThai == "Dang cung cap"; }

    // ------- Cai dat cac ham thuan ao tu Entity -------

    // Nhap du lieu tu ban phim
    void nhapDuLieu() override {
        cout << "\n  --- NHAP THONG TIN GOI KENH ---\n";
        cout << "  Ma goi (dang GKxxx, VD: GK001): ";
        string ma;
        getline(cin, ma);
        if (ma.empty()) throw InvalidInputException("Ma goi khong duoc de trong!");
        // Kiem tra dinh dang GKxxx
        if (ma.size() < 3 || ma.substr(0, 2) != "GK")
            throw InvalidInputException("Ma goi phai co dang GKxxx (VD: GK001)!");
        id = ma;

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

    // Hien thi chi tiet mot ban ghi
    void hienThi() const override {
        cout << "\n";
        Display::inDuongKe(60);
        cout << "  CHI TIET GOI KENH\n";
        Display::inDuongKe(60);
        cout << "  Ma goi      : " << id << "\n";
        cout << "  Ten goi     : " << tenGoi << "\n";
        cout << "  Mo ta       : " << moTa << "\n";
        cout << "  Loai goi    : " << loaiGoi << "\n";
        cout << "  Gia thue    : " << fixed << setprecision(0) << giaThue << " VND/thang\n";
        cout << "  So kenh     : " << soKenh << " kenh\n";
        cout << "  Trang thai  : " << trangThai << "\n";
        Display::inDuongKe(60);
    }

    // Hien thi 1 dong trong bang danh sach
    void hienThiDanhSach() const override {
        cout << "  " << left
             << setw(8)  << id
             << setw(22) << (tenGoi.size() > 20 ? tenGoi.substr(0, 20) : tenGoi)
             << setw(12) << loaiGoi
             << setw(14) << fixed << setprecision(0) << giaThue
             << setw(8)  << soKenh
             << trangThai << "\n";
    }

    // Chuyen sang chuoi luu file (dung | lam phan cach)
    string toFileString() const override {
        ostringstream oss;
        oss << id << "|" << tenGoi << "|" << moTa << "|"
            << fixed << setprecision(2) << giaThue << "|"
            << soKenh << "|" << loaiGoi << "|" << trangThai;
        return oss.str();
    }

    // Doc tu chuoi trong file
    void fromFileString(const string& line) override {
        vector<string> parts = StrUtils::split(line, '|');
        if (parts.size() < 7)
            throw InvalidInputException("Dinh dang dong file GoiKenh khong hop le: " + line);
        id        = StrUtils::trim(parts[0]);
        tenGoi    = StrUtils::trim(parts[1]);
        moTa      = StrUtils::trim(parts[2]);
        giaThue   = stod(parts[3]);
        soKenh    = stoi(parts[4]);
        loaiGoi   = StrUtils::trim(parts[5]);
        trangThai = StrUtils::trim(parts[6]);
    }

    // In tieu de bang
    static void inTieuDeBang() {
        cout << "  " << left
             << setw(8)  << "Ma"
             << setw(22) << "Ten goi kenh"
             << setw(12) << "Loai goi"
             << setw(14) << "Gia thue(VND)"
             << setw(8)  << "So kenh"
             << "Trang thai\n";
        cout << "  " << string(80, '-') << "\n";
    }
};

#endif // GOI_KENH_H
