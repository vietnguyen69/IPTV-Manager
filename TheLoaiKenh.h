#ifndef THELOAIKENH_H
#define THELOAIKENH_H

#include "Entity.h"
#include <iomanip>
#include <stdexcept>

class TheLoaiKenh : public Entity {
private:
    string maTheLoai;
    string tenTheLoai;
    string moTa;

public:
    TheLoaiKenh() : maTheLoai(""), tenTheLoai(""), moTa("") {}

    TheLoaiKenh(const string& ma, const string& ten, const string& mt)
        : maTheLoai(ma), tenTheLoai(ten), moTa(mt) {}

    string getMaTheLoai() const { return maTheLoai; }
    string getTenTheLoai() const { return tenTheLoai; }
    string getMoTa() const { return moTa; }

    void setMaTheLoai(const string& ma) { maTheLoai = ma; }

    void setTenTheLoai(const string& ten) {
        if (ten.empty()) {
            throw invalid_argument("Ten the loai khong duoc de trong!");
        }
        tenTheLoai = ten;
    }

    void setMoTa(const string& mt) { moTa = mt; }

    string getId() const override { return maTheLoai; }
    string getDisplayName() const override { return tenTheLoai; }

    void display() const override {
        cout << "  Ma the loai  : " << maTheLoai << endl;
        cout << "  Ten the loai : " << tenTheLoai << endl;
        cout << "  Mo ta        : " << moTa << endl;
    }

    void displayTableHeader() const override {
        cout << "  " << left
             << setw(6)  << "STT"
             << setw(15) << "Ma the loai"
             << setw(25) << "Ten the loai"
             << setw(35) << "Mo ta"
             << endl;
        cout << "  " << string(81, '-') << endl;
    }

    void displayTableRow(int stt) const override {
        cout << "  " << left
             << setw(6)  << stt
             << setw(15) << maTheLoai
             << setw(25) << tenTheLoai
             << setw(35) << (moTa.length() > 33 ? moTa.substr(0, 30) + "..." : moTa)
             << endl;
    }

    string toFileString() const override {
        return maTheLoai + "|" + tenTheLoai + "|" + moTa;
    }

    void fromFileString(const string& line) override {
        stringstream ss(line);
        getline(ss, maTheLoai, '|');
        getline(ss, tenTheLoai, '|');
        getline(ss, moTa, '|');
    }

    void inputData() override {
        cout << "  Nhap ma the loai (VD: TL001): ";
        getline(cin, maTheLoai);
        if (maTheLoai.empty()) {
            throw invalid_argument("Ma the loai khong duoc de trong!");
        }

        cout << "  Nhap ten the loai: ";
        getline(cin, tenTheLoai);
        if (tenTheLoai.empty()) {
            throw invalid_argument("Ten the loai khong duoc de trong!");
        }

        cout << "  Nhap mo ta: ";
        getline(cin, moTa);
    }

    void updateData() override {
        string temp;
        cout << "  (Nhan Enter de giu nguyen gia tri cu)\n" << endl;

        cout << "  Ten the loai [" << tenTheLoai << "]: ";
        getline(cin, temp);
        if (!temp.empty()) tenTheLoai = temp;

        cout << "  Mo ta [" << moTa << "]: ";
        getline(cin, temp);
        if (!temp.empty()) moTa = temp;
    }
};

#endif
