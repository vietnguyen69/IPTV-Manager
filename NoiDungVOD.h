//Tac gia: Nguyen Quoc Viet - B24DCVT395
//Quan ly noi dung VOD

#ifndef NOIDUNGVOD_H
#define NOIDUNGVOD_H

#include "Entity.h"
#include <iomanip>
#include <stdexcept>

class NoiDungVOD : public Entity {
private:
    string maVOD;       // Ma dinh danh noi dung VOD (Khoa chinh, VD: VOD001, VOD002...)
    string tieuDe;      // Tieu de / Ten phim / Chuong trinh VOD
    string maTheLoai;   // Ma the loai kenh (Khoa ngoai -> TheLoaiKenh)
    int    thoiLuong;   // Thoi luong tinh theo phut (phai > 0)
    int    namSanXuat;  // Nam san xuat (hop le tu 1900 den 2030)
    string daoDien;     // Ten dao dien / nguoi sang tao
    string moTa;        // Mo ta tom tat noi dung

public:
    //Constructor mac dinh khoi tao cac gia tri an toan
    NoiDungVOD()
        : maVOD(""), tieuDe(""), maTheLoai(""),
          thoiLuong(0), namSanXuat(0), daoDien(""), moTa("") {}

    //Constructor day du tham so
    NoiDungVOD(const string& ma, const string& td, const string& mtl,
               int tl, int nsx, const string& dd, const string& mt)
        : maVOD(ma), tieuDe(td), maTheLoai(mtl),
          thoiLuong(tl), namSanXuat(nsx), daoDien(dd), moTa(mt) {}

    //Cung cap Getter
    string getMaVOD() const { return maVOD; }
    string getTieuDe() const { return tieuDe; }
    string getMaTheLoai() const { return maTheLoai; }
    int    getThoiLuong() const { return thoiLuong; }
    int    getNamSanXuat() const { return namSanXuat; }
    string getDaoDien() const { return daoDien; }
    string getMoTa() const { return moTa; }

    //Cung cap Setter co kem logic kiem tra hop le
    void setMaVOD(const string& ma) { maVOD = ma; }

    void setTieuDe(const string& td) {
        if (td.empty()) throw invalid_argument("Tieu de khong duoc de trong!");
        tieuDe = td;
    }

    void setMaTheLoai(const string& mtl) { maTheLoai = mtl; }

    void setThoiLuong(int tl) {
        if (tl <= 0) throw invalid_argument("Thoi luong phai lon hon 0 phut!");
        thoiLuong = tl;
    }

    void setNamSanXuat(int nsx) {
        if (nsx < 1900 || nsx > 2030) {
            throw invalid_argument("Nam san xuat khong hop le (tu 1900 den 2030)!");
        }
        namSanXuat = nsx;
    }

    void setDaoDien(const string& dd) { daoDien = dd; }
    void setMoTa(const string& mt) { moTa = mt; }
	// Cai dat 9 ham thuan ao tu lop Entity
    //Tra ve khoa chinh dinh danh duy nhat de quan ly trong Repository
    string getId() const override { return maVOD; }

    //Tra ve ten dai dien dung de tim kiem gan dung (search theo tieu de)
    string getDisplayName() const override { return tieuDe; }

    //Hien thi chi tiet day du thong tin theo chieu doc
    void display() const override {
        cout << "  Ma VOD       : " << maVOD << endl;
        cout << "  Tieu de      : " << tieuDe << endl;
        cout << "  Ma the loai  : " << maTheLoai << endl;
        cout << "  Thoi luong   : " << thoiLuong << " phut" << endl;
        cout << "  Nam san xuat : " << namSanXuat << endl;
        cout << "  Dao dien     : " << daoDien << endl;
        cout << "  Mo ta        : " << moTa << endl;
    }

    //In tieu de cua bang danh sach noi dung VOD
    void displayTableHeader() const override {
        cout << "  " << left
             << setw(6)  << "STT"
             << setw(12) << "Ma VOD"
             << setw(26) << "Tieu de"
             << setw(12) << "The loai"
             << setw(12) << "Thoi luong"
             << setw(8)  << "Nam SX"
             << setw(20) << "Dao dien"
             << endl;
        cout << "  " << string(96, '-') << endl;
    }

    //In 1 dong du lieu tuong ung trong bang danh sach
    void displayTableRow(int stt) const override {
        string tdShort = tieuDe.length() > 24 ? tieuDe.substr(0, 21) + "..." : tieuDe;
        cout << "  " << left
             << setw(6)  << stt
             << setw(12) << maVOD
             << setw(26) << tdShort
             << setw(12) << maTheLoai
             << setw(12) << (to_string(thoiLuong) + " phut")
             << setw(8)  << namSanXuat
             << setw(20) << daoDien
             << endl;
    }

    //Tuan tu hoa du lieu (Serialize) thanh chuoi noi boi '|' de luu file
    string toFileString() const override {
        return maVOD + "|" + tieuDe + "|" + maTheLoai + "|"
             + to_string(thoiLuong) + "|" + to_string(namSanXuat) + "|"
             + daoDien + "|" + moTa;
    }

    //Giai tuan tu hoa (Deserialize) tu dong van ban doc tu file de nap vao doi tuong
    void fromFileString(const string& line) override {
        stringstream ss(line);
        string temp;
        getline(ss, maVOD, '|');
        getline(ss, tieuDe, '|');
        getline(ss, maTheLoai, '|');
        getline(ss, temp, '|');
        thoiLuong = stoi(temp);
        getline(ss, temp, '|');
        namSanXuat = stoi(temp);
        getline(ss, daoDien, '|');
        getline(ss, moTa, '|');
    }

    //Nhap moi du lieu VOD tu ban phim kem kiem tra hop le
    void inputData() override {
        string temp;

        cout << "  Nhap ma VOD (VD: VOD001): ";
        getline(cin, maVOD);
        if (maVOD.empty()) throw invalid_argument("Ma VOD khong duoc de trong!");

        cout << "  Nhap tieu de phim/chuong trinh: ";
        getline(cin, tieuDe);
        if (tieuDe.empty()) throw invalid_argument("Tieu de khong duoc de trong!");

        cout << "  Nhap ma the loai (VD: TL01): ";
        getline(cin, maTheLoai);
        if (maTheLoai.empty()) throw invalid_argument("Ma the loai khong duoc de trong!");

        cout << "  Nhap thoi luong (phut): ";
        getline(cin, temp);
        try {
            thoiLuong = stoi(temp);
            if (thoiLuong <= 0) throw invalid_argument("");
        } catch (...) {
            throw invalid_argument("Thoi luong phai la so nguyen duong!");
        }

        cout << "  Nhap nam san xuat (1900-2030): ";
        getline(cin, temp);
        try {
            namSanXuat = stoi(temp);
            if (namSanXuat < 1900 || namSanXuat > 2030) throw invalid_argument("");
        } catch (...) {
            throw invalid_argument("Nam san xuat khong hop le (1900-2030)!");
        }

        cout << "  Nhap dao dien: ";
        getline(cin, daoDien);

        cout << "  Nhap mo ta noi dung: ";
        getline(cin, moTa);
    }

    //Cap nhat thong tin VOD (nhan Enter de giu nguyen gia tri cu)
    void updateData() override {
        string temp;
        cout << "  (Nhan Enter de giu nguyen gia tri cu)\n" << endl;

        cout << "  Tieu de [" << tieuDe << "]: ";
        getline(cin, temp);
        if (!temp.empty()) tieuDe = temp;

        cout << "  Ma the loai [" << maTheLoai << "]: ";
        getline(cin, temp);
        if (!temp.empty()) maTheLoai = temp;

        cout << "  Thoi luong [" << thoiLuong << " phut]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            try {
                int val = stoi(temp);
                if (val <= 0) throw invalid_argument("");
                thoiLuong = val;
            } catch (...) {
                throw invalid_argument("Thoi luong phai la so nguyen duong!");
            }
        }

        cout << "  Nam san xuat [" << namSanXuat << "]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            try {
                int val = stoi(temp);
                if (val < 1900 || val > 2030) throw invalid_argument("");
                namSanXuat = val;
            } catch (...) {
                throw invalid_argument("Nam san xuat khong hop le (1900-2030)!");
            }
        }

        cout << "  Dao dien [" << daoDien << "]: ";
        getline(cin, temp);
        if (!temp.empty()) daoDien = temp;

        cout << "  Mo ta [" << moTa << "]: ";
        getline(cin, temp);
        if (!temp.empty()) moTa = temp;
    }
};

#endif