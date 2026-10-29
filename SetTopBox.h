//Tac gia : Nguyen Ba Thanh Van - B24DCVT389
//Quan ly Set-top-box 

#ifndef SETTOPBOX_H
#define SETTOPBOX_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include "Entity.h"
#include "Repository.h"
using namespace std;

class SetTopBox : public Entity {
private:
    string maSTB;      // Ma thiet bi dau thu (dang STBxxx, VD: STB001)
    string serial;     // So serial duy nhat cua thiet bi phan cung
    string model;      // Model dau thu (VD: STB-4K-GEN2, STB-HD-GEN1)
    string maKH;       // Ma khach hang dang giu/su dung thiet bi (Khoa ngoai -> KhachHang, de trong neu ton kho)
    int trangThai;     // 1: Ton kho, 2: Dang su dung, 3: Hong/Bao hanh

    // Ham kiem tra hop le: do dai toi da va khong chua ky tu phan tach '|'
    static void kt(const string& s, const string& nhan, size_t max, bool choRong = false) {
        if (s.empty() && !choRong) throw invalid_argument(nhan + " khong duoc de trong");
        if (s.size() > max) throw invalid_argument(nhan + " toi da " + to_string(max) + " ky tu");
        if (s.find('|') != string::npos) throw invalid_argument(nhan + " khong duoc chua '|'");
    }

    // Ham template ho tro nhap 1 truong tu ban phim an toan; neu sai se yeu cau nhap lai
    template <class F>
    static void nhap(const string& nhan, F set, bool giuNguyen = false) {
        while (true) {
            cout << "  " << nhan << ": ";
            string s;
            if (!getline(cin, s)) throw runtime_error("Het du lieu nhap");
            if (giuNguyen && s.empty()) return; // De trong = giu nguyen gia tri cu
            try { set(s); return; }
            catch (const invalid_argument& e) { cout << "  [LOI] " << e.what() << ". Nhap lai!\n"; }
        }
    }

public:
    // Constructor mac dinh: khoi tao trang thai la 1 (Ton kho)
    SetTopBox() : trangThai(1) {}

    // Cung cap Getter
    string getMaSTB() const { return maSTB; }
    string getMaKH() const { return maKH; }
    int getTrangThai() const { return trangThai; }

    // Cung cap Setter co kiem tra dinh dang
    void setMaSTB(const string& v) {
        if (v.size() < 4 || v.compare(0, 3, "STB") != 0 || v.find_first_not_of("0123456789", 3) != string::npos)
            throw invalid_argument("Ma STB phai co dang STBxxx (vi du STB001)");
        maSTB = v;
    }
    void setSerial(const string& v) { kt(v, "Serial", 20); serial = v; }
    void setModel(const string& v)  { kt(v, "Model", 30); model = v; }
    void setMaKH(const string& v)   { kt(v, "Ma khach hang", 10, true); maKH = v; }
    void setTrangThai(int v) {
        if (v < 1 || v > 3) throw invalid_argument("Trang thai phai la 1, 2 hoac 3");
        trangThai = v;
    }

    // Chuyen ma trang thai so sang chuoi de doc
    static string tenTrangThai(int t) { return t == 1 ? "Ton kho" : t == 2 ? "Dang su dung" : "Hong"; }

    // Cai dat 9 ham thuan ao tu lop Entity
    // 1. Khoa chinh dai dien cho thiet bi
    string getId() const override { return maSTB; }

    // 2. Ten dai dien hien thi dung trong tim kiem
    string getDisplayName() const override { return model + " " + serial; }

    // 3. Hien thi chi tiet thong tin STB theo chieu doc
    void display() const override {
        cout << "  Ma STB: " << maSTB << "\n  Serial: " << serial << "\n  Model: " << model
             << "\n  Ma KH: " << (maKH.empty() ? "-" : maKH)
             << "\n  Trang thai: " << tenTrangThai(trangThai) << endl;
    }

    // 4. In tieu de bang danh sach STB
    void displayTableHeader() const override {
        cout << "  " << string(76, '-') << "\n  " << left << setw(5) << "STT" << setw(9) << "Ma STB"
             << setw(15) << "Serial" << setw(20) << "Model" << setw(9) << "Ma KH" << setw(14) << "Trang thai"
             << "\n  " << string(76, '-') << endl;
    }

    // 5. In dong du lieu trong bang
    void displayTableRow(int stt) const override {
        cout << "  " << left << setw(5) << stt << setw(9) << maSTB << setw(15) << serial << setw(20) << model
             << setw(9) << (maKH.empty() ? "-" : maKH) << setw(14) << tenTrangThai(trangThai) << endl;
    }

    // 6. Tuan tu hoa du lieu thanh chuoi de ghi vao file
    string toFileString() const override {
        return maSTB + "|" + serial + "|" + model + "|" + maKH + "|" + to_string(trangThai);
    }

    // 7. Giai tuan tu hoa tu dong van ban doc trong file
    void fromFileString(const string& line) override {
        vector<string> f; string cur;
        for (char c : line) { if (c == '|') { f.push_back(cur); cur.clear(); } else if (c != '\r') cur += c; }
        f.push_back(cur);
        if (f.size() != 5) throw invalid_argument("Dong Set-top-box phai co 5 truong");
        setMaSTB(f[0]); setSerial(f[1]); setModel(f[2]); setMaKH(f[3]);
        if (f[4].size() != 1 || !isdigit((unsigned char)f[4][0])) throw invalid_argument("Trang thai sai");
        setTrangThai(f[4][0] - '0');
    }

    // 8. Nhap moi thong tin thiet bi tu ban phim
    void inputData() override {
        nhap("Ma STB (STBxxx)", [this](const string& s) { setMaSTB(s); });
        nhap("Serial", [this](const string& s) { setSerial(s); });
        nhap("Model", [this](const string& s) { setModel(s); });
        nhap("Ma khach hang (bo trong neu chua cap)", [this](const string& s) { setMaKH(s); });
        nhap("Trang thai (1.Ton kho 2.Dang su dung 3.Hong)", [this](const string& s) {
            if (s.size() != 1 || !isdigit((unsigned char)s[0])) throw invalid_argument("Phai nhap so 1-3");
            setTrangThai(s[0] - '0'); });
    }

    // 9. Cap nhat thong tin thiet bi (nhan Enter de giu nguyen gia tri cu)
    void updateData() override {
        nhap("Serial [" + serial + "]", [this](const string& s) { setSerial(s); }, true);
        nhap("Model [" + model + "]", [this](const string& s) { setModel(s); }, true);
        nhap("Ma KH [" + (maKH.empty() ? "-" : maKH) + "] ('-' de xoa)", [this](const string& s) {
            setMaKH(s == "-" ? "" : s); }, true);
        nhap("Trang thai [" + tenTrangThai(trangThai) + "] (1-3)", [this](const string& s) {
            if (s.size() != 1 || !isdigit((unsigned char)s[0])) throw invalid_argument("Phai nhap so 1-3");
            setTrangThai(s[0] - '0'); }, true);
    }
};

#endif