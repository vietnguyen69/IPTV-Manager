//Tac gia : Nguyen Ba Thanh Van - B24DCVT389
//Quan ly phieu ho tro ky thuat

#ifndef PHIEUHOTRO_H
#define PHIEUHOTRO_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <ctime>
#include "Entity.h"
#include "Repository.h"
using namespace std;

class PhieuHoTro : public Entity {
private:
    string maPhieu;    // Ma phieu ho tro (dang HTxxx, VD: HT001)
    string maSTB;      // Ma Set-top-box gap su co (Khoa ngoai -> SetTopBox, dang STBxxx)
    string moTa;       // Mo ta chi tiet ve su co ky thuat
    string ngayTao;    // Ngay tao phieu (dd/mm/yyyy, khong duoc sau ngay hien tai)
    int trangThai;     // 1: Moi, 2: Dang xu ly, 3: Hoan thanh

    // Ham template nhap 1 truong co bat loi va cho phep giu nguyen gia tri cu
    template <class F>
    static void nhap(const string& nhan, F set, bool giuNguyen = false) {
        while (true) {
            cout << "  " << nhan << ": ";
            string s;
            if (!getline(cin, s)) throw runtime_error("Het du lieu nhap");
            if (giuNguyen && s.empty()) return;
            try { set(s); return; }
            catch (const invalid_argument& e) { cout << "  [LOI] " << e.what() << ". Nhap lai!\n"; }
        }
    }

    // Chuyen ky tu chuoi sang so nguyen 1-3
    static int doiTrangThai(const string& s) {
        if (s.size() != 1 || !isdigit((unsigned char)s[0])) throw invalid_argument("Phai nhap so 1-3");
        return s[0] - '0';
    }

public:
    // Constructor mac dinh: trang thai khoi tao la 1 (Moi)
    PhieuHoTro() : trangThai(1) {}

    // Cung cap Getter
    string getMaPhieu() const { return maPhieu; }
    string getMaSTB() const { return maSTB; }
    int getTrangThai() const { return trangThai; }

    // Cung cap Setter co kem kiem tra hop le
    void setMaPhieu(const string& v) {
        if (v.size() < 3 || v.compare(0, 2, "HT") != 0 || v.find_first_not_of("0123456789", 2) != string::npos)
            throw invalid_argument("Ma phieu phai co dang HTxxx (vi du HT001)");
        maPhieu = v;
    }

    void setMaSTB(const string& v) {
        if (v.size() < 4 || v.compare(0, 3, "STB") != 0 || v.find_first_not_of("0123456789", 3) != string::npos)
            throw invalid_argument("Ma STB phai co dang STBxxx");
        maSTB = v;
    }

    void setMoTa(const string& v) {
        if (v.empty()) throw invalid_argument("Mo ta khong duoc de trong");
        if (v.size() > 100) throw invalid_argument("Mo ta toi da 100 ky tu");
        if (v.find('|') != string::npos) throw invalid_argument("Mo ta khong duoc chua '|'");
        moTa = v;
    }

    void setTrangThai(int v) {
        if (v < 1 || v > 3) throw invalid_argument("Trang thai phai la 1, 2 hoac 3");
        trangThai = v;
    }

    // Kiem tra tinh hop le cua ngay tao (dd/mm/yyyy, dung so ngay trong thang va khong vuot qua ngay hien tai)
    void setNgayTao(const string& v) {
        int d, m, y;
        if (v.size() != 10 || v[2] != '/' || v[5] != '/' ||
            sscanf(v.c_str(), "%2d/%2d/%4d", &d, &m, &y) != 3)
            throw invalid_argument("Ngay phai co dang dd/mm/yyyy");
        static const int ngay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        bool nhuan = (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
        if (y < 1900 || m < 1 || m > 12 || d < 1 || d > ngay[m - 1] + (m == 2 && nhuan))
            throw invalid_argument("Ngay khong hop le");
        time_t t = time(nullptr); tm* now = localtime(&t);
        if (y * 10000 + m * 100 + d > (now->tm_year + 1900) * 10000 + (now->tm_mon + 1) * 100 + now->tm_mday)
            throw invalid_argument("Ngay tao khong duoc sau ngay hien tai");
        ngayTao = v;
    }

    // Chuyen ma trang thai sang ten hien thi
    static string tenTrangThai(int t) { return t == 1 ? "Moi" : t == 2 ? "Dang xu ly" : "Hoan thanh"; }

    // Cai dat 9 ham thuan ao tu lop Entity
    // 1. Khoa chinh dai dien cho phieu ho tro
    string getId() const override { return maPhieu; }
    // 2. Ten dai dien dung trong tim kiem
    string getDisplayName() const override { return maSTB + " " + moTa; }
    // 3. Hien thi chi tiet phieu ho tro theo chieu doc
    void display() const override {
        cout << "  Ma phieu: " << maPhieu << "\n  Ma STB: " << maSTB << "\n  Mo ta: " << moTa
             << "\n  Trang thai: " << tenTrangThai(trangThai) << "\n  Ngay tao: " << ngayTao << endl;
    }

    // 4. In tieu de bang danh sach phieu ho tro
    void displayTableHeader() const override {
        cout << "  " << string(88, '-') << "\n  " << left << setw(5) << "STT" << setw(9) << "Ma HT" << setw(9) << "Ma STB"
             << setw(42) << "Mo ta" << setw(14) << "Trang thai" << setw(12) << "Ngay tao"
             << "\n  " << string(88, '-') << endl;
    }

    // 5. In dong du lieu phieu ho tro trong bang
    void displayTableRow(int stt) const override {
        cout << "  " << left << setw(5) << stt << setw(9) << maPhieu << setw(9) << maSTB
             << setw(42) << (moTa.size() > 40 ? moTa.substr(0, 38) + ".." : moTa)
             << setw(14) << tenTrangThai(trangThai) << setw(12) << ngayTao << endl;
    }

    // 6. Tuan tu hoa du lieu thanh chuoi de ghi file
    string toFileString() const override {
        return maPhieu + "|" + maSTB + "|" + moTa + "|" + to_string(trangThai) + "|" + ngayTao;
    }

    // 7. Giai tuan tu hoa tu dong van ban doc tu file
    void fromFileString(const string& line) override {
        vector<string> f; string cur;
        for (char c : line) { if (c == '|') { f.push_back(cur); cur.clear(); } else if (c != '\r') cur += c; }
        f.push_back(cur);
        if (f.size() != 5) throw invalid_argument("Dong Phieu ho tro phai co 5 truong");
        setMaPhieu(f[0]); setMaSTB(f[1]); setMoTa(f[2]); setTrangThai(doiTrangThai(f[3])); setNgayTao(f[4]);
    }

    // 8. Nhap moi phieu ho tro tu ban phim
    void inputData() override {
        nhap("Ma phieu (HTxxx)", [this](const string& s) { setMaPhieu(s); });
        nhap("Ma Set-top-box (STBxxx)", [this](const string& s) { setMaSTB(s); });
        nhap("Mo ta su co", [this](const string& s) { setMoTa(s); });
        nhap("Trang thai (1.Moi 2.Dang xu ly 3.Hoan thanh)", [this](const string& s) { setTrangThai(doiTrangThai(s)); });
        nhap("Ngay tao (dd/mm/yyyy)", [this](const string& s) { setNgayTao(s); });
    }

    // 9. Cap nhat phieu ho tro (nhan Enter de giu nguyen gia tri cu)
    void updateData() override {
        nhap("Mo ta [" + moTa + "]", [this](const string& s) { setMoTa(s); }, true);
        nhap("Trang thai [" + tenTrangThai(trangThai) + "] (1-3)", [this](const string& s) { setTrangThai(doiTrangThai(s)); }, true);
        nhap("Ngay tao [" + ngayTao + "]", [this](const string& s) { setNgayTao(s); }, true);
    }
};

#endif