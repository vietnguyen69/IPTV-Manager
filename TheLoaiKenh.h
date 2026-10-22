//Tac gia: Nguyen Quoc Viet - B24DCVT395
//Use case: Quan ly the loai kenh
#ifndef THELOAIKENH_H
#define THELOAIKENH_H

#include "Entity.h"
#include <iomanip>
#include <stdexcept>

class TheLoaiKenh : public Entity {
private:
    string maTheLoai;   // Ma the loai kenh (Khoa chinh, VD: TL01, TL02...)
    string tenTheLoai;  // Ten the loai (VD: The thao, Thoi su, Phim truyen...)
    string moTa;        // Mo ta chi tiet ve the loai

public:
    // Constructor mac dinh khoi tao chuoi rong
    TheLoaiKenh() : maTheLoai(""), tenTheLoai(""), moTa("") {}

    // Constructor day du tham so
    TheLoaiKenh(const string& ma, const string& ten, const string& mt)
        : maTheLoai(ma), tenTheLoai(ten), moTa(mt) {}

    // ----- Cung cap Getter -----
    string getMaTheLoai() const { return maTheLoai; }
    string getTenTheLoai() const { return tenTheLoai; }
    string getMoTa() const { return moTa; }

    // ----- Cung cap Setter co kem logic kiem tra hop le -----
    void setMaTheLoai(const string& ma) { maTheLoai = ma; }

    void setTenTheLoai(const string& ten) {
        if (ten.empty()) {
            throw invalid_argument("Ten the loai khong duoc de trong!");
        }
        tenTheLoai = ten;
    }

    void setMoTa(const string& mt) { moTa = mt; }

    // =================================================================================
    // CAI DAT 9 PHUONG THUC THUAN AO TU LOP CO SO ENTITY
    // =================================================================================

    // 1. Tra ve khoa chinh dinh danh duy nhat cho Repository quan ly
    string getId() const override { return maTheLoai; }

    // 2. Tra ve ten dai dien dung de tim kiem gan dung (search)
    string getDisplayName() const override { return tenTheLoai; }

    // 3. Hien thi chi tiet thong tin ban ghi theo chieu doc (khi xem chi tiet / truoc khi xoa)
    void display() const override {
        cout << "  Ma the loai  : " << maTheLoai << endl;
        cout << "  Ten the loai : " << tenTheLoai << endl;
        cout << "  Mo ta        : " << moTa << endl;
    }

    // 4. In dong tieu de cua bang danh sach
    void displayTableHeader() const override {
        cout << "  " << left
             << setw(6)  << "STT"
             << setw(15) << "Ma the loai"
             << setw(25) << "Ten the loai"
             << setw(35) << "Mo ta"
             << endl;
        cout << "  " << string(81, '-') << endl;
    }

    // 5. In 1 dong du lieu tuong ung trong bang danh sach
    void displayTableRow(int stt) const override {
        cout << "  " << left
             << setw(6)  << stt
             << setw(15) << maTheLoai
             << setw(25) << tenTheLoai
             << setw(35) << (moTa.length() > 33 ? moTa.substr(0, 30) + "..." : moTa)
             << endl;
    }

    // 6. Tuan tu hoa du lieu (Serialize) thanh dong van ban cach nhau boi dau '|' de ghi vao file
    string toFileString() const override {
        return maTheLoai + "|" + tenTheLoai + "|" + moTa;
    }

    // 7. Giai tuan tu hoa (Deserialize) tu dong van ban doc tu file de nap lai vao thuoc tinh
    void fromFileString(const string& line) override {
        stringstream ss(line);
        getline(ss, maTheLoai, '|');
        getline(ss, tenTheLoai, '|');
        getline(ss, moTa, '|');
    }

    // 8. Nhap moi du lieu tu ban phim (co kiem tra rang buoc bat buoc khong de trong)
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

    // 9. Cap nhat thong tin ban ghi (neu de trong va nhan Enter thi giu nguyen gia tri cu)
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
