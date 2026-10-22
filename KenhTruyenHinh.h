/*
 * File   : KenhTruyenHinh.h
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 * Mo ta  : Lop KenhTruyenHinh - doi tuong "Kenh truyen hinh" (use case Quan ly kenh truyen hinh).
 *          Ke thua Entity. Thuoc tinh private, setter co kiem tra hop le (nem ValidationException).
 *
 * Dinh dang dong trong file: MaKenh|TenKenh|MaTheLoai|DoPhanGiai|SoKenh|TrangThai
 */
#ifndef KENHTRUYENHINH_H
#define KENHTRUYENHINH_H

#include "Entity.h"
#include <iomanip>
#include <stdexcept>

class KenhTruyenHinh : public Entity {
private:
    string maKenh;
    string tenKenh;
    string maTheLoai;
    string chatLuong;
    string ngonNgu;
    string moTa;

public:
    KenhTruyenHinh()
        : maKenh(""), tenKenh(""), maTheLoai(""),
          chatLuong(""), ngonNgu(""), moTa("") {}

    KenhTruyenHinh(const string& mk, const string& tk, const string& mtl,
                   const string& cl, const string& nn, const string& mt)
        : maKenh(mk), tenKenh(tk), maTheLoai(mtl),
          chatLuong(cl), ngonNgu(nn), moTa(mt) {}

    string getMaKenh() const { return maKenh; }
    string getTenKenh() const { return tenKenh; }
    string getMaTheLoai() const { return maTheLoai; }
    string getChatLuong() const { return chatLuong; }
    string getNgonNgu() const { return ngonNgu; }
    string getMoTa() const { return moTa; }

    void setMaKenh(const string& mk) { maKenh = mk; }

    void setTenKenh(const string& tk) {
        if (tk.empty()) throw invalid_argument("Ten kenh khong duoc de trong!");
        tenKenh = tk;
    }

    void setMaTheLoai(const string& mtl) { maTheLoai = mtl; }

    void setChatLuong(const string& cl) {
        if (cl != "SD" && cl != "HD" && cl != "Full HD" && cl != "4K") {
            throw invalid_argument("Chat luong phai la: SD, HD, Full HD hoac 4K!");
        }
        chatLuong = cl;
    }

    void setNgonNgu(const string& nn) { ngonNgu = nn; }
    void setMoTa(const string& mt) { moTa = mt; }

    string getId() const override { return maKenh; }
    string getDisplayName() const override { return tenKenh; }

    void display() const override {
        cout << "  Ma kenh       : " << maKenh << endl;
        cout << "  Ten kenh      : " << tenKenh << endl;
        cout << "  Ma the loai   : " << maTheLoai << endl;
        cout << "  Chat luong    : " << chatLuong << endl;
        cout << "  Ngon ngu      : " << ngonNgu << endl;
        cout << "  Mo ta         : " << moTa << endl;
    }

    void displayTableHeader() const override {
        cout << "  " << left
             << setw(6)  << "STT"
             << setw(12) << "Ma kenh"
             << setw(26) << "Ten kenh"
             << setw(12) << "The loai"
             << setw(12) << "Chat luong"
             << setw(12) << "Ngon ngu"
             << endl;
        cout << "  " << string(80, '-') << endl;
    }

    void displayTableRow(int stt) const override {
        cout << "  " << left
             << setw(6)  << stt
             << setw(12) << maKenh
             << setw(26) << tenKenh
             << setw(12) << maTheLoai
             << setw(12) << chatLuong
             << setw(12) << ngonNgu
             << endl;
    }

    string toFileString() const override {
        return maKenh + "|" + tenKenh + "|" + maTheLoai + "|"
             + chatLuong + "|" + ngonNgu + "|" + moTa;
    }

    void fromFileString(const string& line) override {
        stringstream ss(line);
        getline(ss, maKenh, '|');
        getline(ss, tenKenh, '|');
        getline(ss, maTheLoai, '|');
        getline(ss, chatLuong, '|');
        getline(ss, ngonNgu, '|');
        getline(ss, moTa, '|');
    }

    void inputData() override {
        cout << "  Nhap ma kenh (VD: K001): ";
        getline(cin, maKenh);
        if (maKenh.empty()) throw invalid_argument("Ma kenh khong duoc de trong!");

        cout << "  Nhap ten kenh: ";
        getline(cin, tenKenh);
        if (tenKenh.empty()) throw invalid_argument("Ten kenh khong duoc de trong!");

        cout << "  Nhap ma the loai (VD: TL001): ";
        getline(cin, maTheLoai);
        if (maTheLoai.empty()) throw invalid_argument("Ma the loai khong duoc de trong!");

        cout << "  Nhap chat luong (SD / HD / Full HD / 4K): ";
        getline(cin, chatLuong);
        if (chatLuong != "SD" && chatLuong != "HD" && chatLuong != "Full HD" && chatLuong != "4K") {
            throw invalid_argument("Chat luong phai la: SD, HD, Full HD hoac 4K!");
        }

        cout << "  Nhap ngon ngu: ";
        getline(cin, ngonNgu);
        cout << "  Nhap mo ta: ";
        getline(cin, moTa);
    }

    void updateData() override {
        string temp;
        cout << "  (Nhan Enter de giu nguyen gia tri cu)\n" << endl;

        cout << "  Ten kenh [" << tenKenh << "]: ";
        getline(cin, temp);
        if (!temp.empty()) tenKenh = temp;

        cout << "  Ma the loai [" << maTheLoai << "]: ";
        getline(cin, temp);
        if (!temp.empty()) maTheLoai = temp;

        cout << "  Chat luong [" << chatLuong << "]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            if (temp != "SD" && temp != "HD" && temp != "Full HD" && temp != "4K") {
                throw invalid_argument("Chat luong phai la: SD, HD, Full HD hoac 4K!");
            }
            chatLuong = temp;
        }

        cout << "  Ngon ngu [" << ngonNgu << "]: ";
        getline(cin, temp);
        if (!temp.empty()) ngonNgu = temp;

        cout << "  Mo ta [" << moTa << "]: ";
        getline(cin, temp);
        if (!temp.empty()) moTa = temp;
    }
};

#endif
