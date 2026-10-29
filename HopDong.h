// Tac gia: Do Gia Huy - B24DCVT172
// Use case Quan ly hop dong

#ifndef HOPDONG_H
#define HOPDONG_H

#include "Entity.h"
#include <iomanip>
#include <stdexcept>

class HopDong : public Entity {
private:
    string maHopDong;     // Ma hop dong duy nhat (Khoa chinh, VD: HD001, HD002...)
    string maKhachHang;   // Ma khach hang dung ten hop dong (Khoa ngoai -> KhachHang)
    string goiDichVu;     // Ma/ten goi dich vu dang ky (Khoa ngoai -> GoiKenh)
    string ngayKy;        // Ngay ky ket hop dong (DD/MM/YYYY)
    string ngayBatDau;    // Ngay hop dong bat dau co hieu luc (DD/MM/YYYY)
    string ngayKetThuc;   // Ngay het han hop dong (DD/MM/YYYY)
    double donGiaThang;   // Gia cuoc hang thang phai thanh toan (VND/thang)
    string trangThai;     // Trang thai: "Dang hieu luc", "Tam dung", "Da huy"

public:
    // Constructor mac dinh khoi tao gia tri an toan
    HopDong()
        : maHopDong(""), maKhachHang(""), goiDichVu(""), ngayKy(""),
          ngayBatDau(""), ngayKetThuc(""), donGiaThang(0), trangThai("Dang hieu luc") {}

    // Constructor day du tham so kem kiem tra gia khong am
    HopDong(const string& ma, const string& mkh, const string& goi,
            const string& nk, const string& nbd, const string& nkt,
            double gia, const string& tt)
        : maHopDong(ma), maKhachHang(mkh), goiDichVu(goi), ngayKy(nk),
          ngayBatDau(nbd), ngayKetThuc(nkt), donGiaThang(gia), trangThai(tt) {
        if (donGiaThang < 0) throw invalid_argument("Don gia thang khong duoc am!");
    }

    // Cung cap Getter
    string getMaHopDong() const { return maHopDong; }
    string getMaKhachHang() const { return maKhachHang; }
    string getGoiDichVu() const { return goiDichVu; }
    string getNgayKy() const { return ngayKy; }
    string getNgayBatDau() const { return ngayBatDau; }
    string getNgayKetThuc() const { return ngayKetThuc; }
    double getDonGiaThang() const { return donGiaThang; }
    string getTrangThai() const { return trangThai; }

    // Cung cap Setter co kiem tra hop le
    void setMaHopDong(const string& v) { maHopDong = v; }

    void setMaKhachHang(const string& v) {
        if (v.empty()) throw invalid_argument("Ma khach hang khong duoc de trong!");
        maKhachHang = v;
    }

    void setGoiDichVu(const string& v) {
        if (v.empty()) throw invalid_argument("Goi dich vu khong duoc de trong!");
        goiDichVu = v;
    }

    void setDonGiaThang(double v) {
        if (v < 0) throw invalid_argument("Don gia thang khong duoc am!");
        donGiaThang = v;
    }

    void setTrangThai(const string& v) { trangThai = v; }
    // Cai dat 9 ham thuan ao tu lop Entity
    // 1. Khoa chinh dai dien cho hop dong
    string getId() const override { return maHopDong; }
    // 2. Ten dai dien dung trong tim kiem
    string getDisplayName() const override { return maKhachHang + " - " + goiDichVu; }
    // 3. Hien thi chi tiet thong tin hop dong theo chieu doc
    void display() const override {
        cout << "  Ma hop dong   : " << maHopDong << '\n'
             << "  Ma khach hang : " << maKhachHang << '\n'
             << "  Goi dich vu   : " << goiDichVu << '\n'
             << "  Ngay ky       : " << ngayKy << '\n'
             << "  Ngay bat dau  : " << ngayBatDau << '\n'
             << "  Ngay ket thuc : " << ngayKetThuc << '\n'
             << "  Don gia/thang : " << fixed << setprecision(0) << donGiaThang << " VND\n"
             << "  Trang thai    : " << trangThai << endl;
    }

    // 4. In dong tieu de bang danh sach hop dong
    void displayTableHeader() const override {
        cout << "  " << left << setw(6) << "STT" << setw(14) << "Ma HD"
             << setw(16) << "Ma KH" << setw(22) << "Goi dich vu"
             << setw(13) << "Ngay ky" << setw(13) << "Ngay bat dau"
             << setw(13) << "Ngay ket thuc" << setw(16) << "Gia/thang"
             << "Trang thai\n";
        cout << "  " << string(125, '-') << '\n';
    }

    // 5. In tung dong du lieu hop dong trong bang
    void displayTableRow(int stt) const override {
        string goi = goiDichVu.size() > 20 ? goiDichVu.substr(0,17) + "..." : goiDichVu;
        cout << "  " << left << setw(6) << stt << setw(14) << maHopDong
             << setw(16) << maKhachHang << setw(22) << goi
             << setw(13) << ngayKy << setw(13) << ngayBatDau
             << setw(13) << ngayKetThuc << setw(16) << fixed << setprecision(0) << donGiaThang
             << trangThai << '\n';
    }

    // 6. Tuan tu hoa du lieu thanh chuoi de ghi vao file
    string toFileString() const override {
        ostringstream out;
        out << maHopDong << '|' << maKhachHang << '|' << goiDichVu << '|'
            << ngayKy << '|' << ngayBatDau << '|' << ngayKetThuc << '|'
            << setprecision(15) << donGiaThang << '|' << trangThai;
        return out.str();
    }

    // 7. Giai tuan tu hoa tu dong van ban doc tu file
    void fromFileString(const string& line) override {
        stringstream ss(line);
        string temp;
        getline(ss, maHopDong, '|');
        getline(ss, maKhachHang, '|');
        getline(ss, goiDichVu, '|');
        getline(ss, ngayKy, '|');
        getline(ss, ngayBatDau, '|');
        getline(ss, ngayKetThuc, '|');
        getline(ss, temp, '|');
        donGiaThang = stod(temp);
        getline(ss, trangThai);
        if (maHopDong.empty() || maKhachHang.empty() || goiDichVu.empty())
            throw invalid_argument("Du lieu hop dong khong hop le!");
    }

    // 8. Nhap moi hop dong tu ban phim kem kiem tra rang buoc
    void inputData() override {
        string temp;
        cout << "  Nhap ma hop dong (VD: HD001): ";
        getline(cin, maHopDong);
        if (maHopDong.empty()) throw invalid_argument("Ma hop dong khong duoc de trong!");

        cout << "  Nhap ma khach hang: ";
        getline(cin, maKhachHang);
        if (maKhachHang.empty()) throw invalid_argument("Ma khach hang khong duoc de trong!");

        cout << "  Nhap goi dich vu: ";
        getline(cin, goiDichVu);
        if (goiDichVu.empty()) throw invalid_argument("Goi dich vu khong duoc de trong!");

        cout << "  Nhap ngay ky (DD/MM/YYYY): "; getline(cin, ngayKy);
        cout << "  Nhap ngay bat dau (DD/MM/YYYY): "; getline(cin, ngayBatDau);
        cout << "  Nhap ngay ket thuc (DD/MM/YYYY): "; getline(cin, ngayKetThuc);

        cout << "  Nhap don gia/thang (VND): ";
        getline(cin, temp);
        try {
            size_t pos = 0;
            donGiaThang = stod(temp, &pos);
            if (pos != temp.size() || donGiaThang < 0) throw invalid_argument("");
        } catch (...) {
            throw invalid_argument("Don gia phai la so khong am!");
        }

        cout << "  Nhap trang thai (Dang hieu luc/Tam dung/Da huy): ";
        getline(cin, trangThai);
        if (trangThai.empty()) trangThai = "Dang hieu luc";
    }

    // 9. Cap nhat hop dong (nhan Enter de giu nguyen gia tri cu)
    void updateData() override {
        string temp;
        cout << "  (Nhan Enter de giu nguyen gia tri cu)\n\n";
        cout << "  Ma khach hang [" << maKhachHang << "]: ";
        getline(cin, temp); if (!temp.empty()) maKhachHang = temp;
        cout << "  Goi dich vu [" << goiDichVu << "]: ";
        getline(cin, temp); if (!temp.empty()) goiDichVu = temp;
        cout << "  Ngay ky [" << ngayKy << "]: ";
        getline(cin, temp); if (!temp.empty()) ngayKy = temp;
        cout << "  Ngay bat dau [" << ngayBatDau << "]: ";
        getline(cin, temp); if (!temp.empty()) ngayBatDau = temp;
        cout << "  Ngay ket thuc [" << ngayKetThuc << "]: ";
        getline(cin, temp); if (!temp.empty()) ngayKetThuc = temp;
        cout << "  Don gia/thang [" << fixed << setprecision(0) << donGiaThang << "]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            try {
                size_t pos = 0;
                double v = stod(temp, &pos);
                if (pos != temp.size() || v < 0) throw invalid_argument("");
                donGiaThang = v;
            } catch (...) {
                throw invalid_argument("Don gia phai la so khong am!");
            }
        }
        cout << "  Trang thai [" << trangThai << "]: ";
        getline(cin, temp); if (!temp.empty()) trangThai = temp;
        if (maKhachHang.empty() || goiDichVu.empty())
            throw invalid_argument("Ma khach hang va goi dich vu khong duoc de trong!");
    }
};

#endif