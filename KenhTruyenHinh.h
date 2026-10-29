// Tac gia: Duong Duc Manh - B24DCVT240
//Use case Quan ly kenh truyen hinh

#ifndef KENHTRUYENHINH_H
#define KENHTRUYENHINH_H

#include "Entity.h"
#include <iomanip>
#include <stdexcept>

class KenhTruyenHinh : public Entity {
private:
    string maKenh;       // Ma dinh danh kenh (Khoa chinh, VD: K001, K002...)
    string tenKenh;      // Ten kenh truyen hinh (VD: VTV1 HD, K+ SPORT 1...)
    string maTheLoai;    // Ma the loai kenh (Khoa ngoai -> TheLoaiKenh, VD: TL01)
    string chatLuong;    // Chat luong phat song: "SD", "HD", "Full HD", "4K"
    string ngonNgu;      // Ngon ngu phat song (VD: Tieng Viet, Tieng Anh...)
    string moTa;         // Mo ta noi dung phat song cua kenh

public:
    //Constructor mac dinh khoi tao chuoi rong
    KenhTruyenHinh()
        : maKenh(""), tenKenh(""), maTheLoai(""),
          chatLuong(""), ngonNgu(""), moTa("") {}

    //Constructor day du tham so
    KenhTruyenHinh(const string& mk, const string& tk, const string& mtl,
                   const string& cl, const string& nn, const string& mt)
        : maKenh(mk), tenKenh(tk), maTheLoai(mtl),
          chatLuong(cl), ngonNgu(nn), moTa(mt) {}

    // Cung cap Getter
    string getMaKenh() const { return maKenh; }
    string getTenKenh() const { return tenKenh; }
    string getMaTheLoai() const { return maTheLoai; }
    string getChatLuong() const { return chatLuong; }
    string getNgonNgu() const { return ngonNgu; }
    string getMoTa() const { return moTa; }

    // Cung cap Setter co kem kiem tra hop le
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

	// Cai dat 9 ham thuan ao tu lop Entity
    // Khoa chinh dai dien cho kenh
    string getId() const override { return maKenh; }

    // Ten dai dien dung trong tim kiem
    string getDisplayName() const override { return tenKenh; }

    //Hien thi chi tiet thong tin kenh theo chieu doc
    void display() const override {
        cout << "  Ma kenh       : " << maKenh << endl;
        cout << "  Ten kenh      : " << tenKenh << endl;
        cout << "  Ma the loai   : " << maTheLoai << endl;
        cout << "  Chat luong    : " << chatLuong << endl;
        cout << "  Ngon ngu      : " << ngonNgu << endl;
        cout << "  Mo ta         : " << moTa << endl;
    }

    //In tieu de bang danh sach kenh truyen hinh
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

    //In tung dong thong tin trong bang danh sach
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

    //Tuan tu hoa du lieu thanh chuoi de ghi vao file
    string toFileString() const override {
        return maKenh + "|" + tenKenh + "|" + maTheLoai + "|"
             + chatLuong + "|" + ngonNgu + "|" + moTa;
    }

    //Giai tuan tu hoa tu dong van ban doc trong file
    void fromFileString(const string& line) override {
        stringstream ss(line);
        getline(ss, maKenh, '|');
        getline(ss, tenKenh, '|');
        getline(ss, maTheLoai, '|');
        getline(ss, chatLuong, '|');
        getline(ss, ngonNgu, '|');
        getline(ss, moTa, '|');
    }

    //Nhap moi thong tin kenh tu ban phim kem kiem tra rang buoc
    void inputData() override {
        cout << "  Nhap ma kenh (VD: K001): ";
        getline(cin, maKenh);
        if (maKenh.empty()) throw invalid_argument("Ma kenh khong duoc de trong!");

        cout << "  Nhap ten kenh: ";
        getline(cin, tenKenh);
        if (tenKenh.empty()) throw invalid_argument("Ten kenh khong duoc de trong!");

        cout << "  Nhap ma the loai (VD: TL01): ";
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

    //Cap nhat thong tin kenh (nhan Enter de giu nguyen gia tri cu)
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