// Tac gia: Do Gia Huy - B24DCVT172
//Use case: Quan ly hoa don

#ifndef HOADON_H
#define HOADON_H

#include "Entity.h"
#include <iomanip>
#include <stdexcept>

class HoaDon : public Entity {
private:
    string maHoaDon;            // Ma hoa don duy nhat (Khoa chinh, VD: HDON001, HDON002...)
    string maHopDong;           // Ma hop dong phat sinh hoa don (Khoa ngoai -> HopDong)
    string kyCuoc;              // Ky cuoc tinh phi (dinh dang MM/YYYY, VD: 10/2026)
    string ngayLap;             // Ngay xuat hoa don (DD/MM/YYYY)
    string hanThanhToan;        // Han chot thanh toan (DD/MM/YYYY)
    double soTien;              // So tien can thanh toan (VND, khong duoc am)
    string trangThaiThanhToan;  // "Chua thanh toan", "Da thanh toan", "Qua han"

public:
    // Constructor mac dinh khoi tao cac gia tri an toan
    HoaDon()
        : maHoaDon(""), maHopDong(""), kyCuoc(""), ngayLap(""),
          hanThanhToan(""), soTien(0), trangThaiThanhToan("Chua thanh toan") {}

    // Constructor day du tham so kem kiem tra so tien khong am
    HoaDon(const string& ma, const string& mhd, const string& ky,
           const string& nl, const string& han, double tien, const string& tt)
        : maHoaDon(ma), maHopDong(mhd), kyCuoc(ky), ngayLap(nl),
          hanThanhToan(han), soTien(tien), trangThaiThanhToan(tt) {
        if (soTien < 0) throw invalid_argument("So tien khong duoc am!");
    }

    // Cung cap Getter
    string getMaHoaDon() const { return maHoaDon; }
    string getMaHopDong() const { return maHopDong; }
    string getKyCuoc() const { return kyCuoc; }
    string getNgayLap() const { return ngayLap; }
    string getHanThanhToan() const { return hanThanhToan; }
    double getSoTien() const { return soTien; }
    string getTrangThaiThanhToan() const { return trangThaiThanhToan; }

    // Cung cap Setter co kem logic kiem tra hop le
    void setMaHoaDon(const string& v) { maHoaDon = v; }

    void setMaHopDong(const string& v) {
        if (v.empty()) throw invalid_argument("Ma hop dong khong duoc de trong!");
        maHopDong = v;
    }

    void setKyCuoc(const string& v) {
        if (v.empty()) throw invalid_argument("Ky cuoc khong duoc de trong!");
        kyCuoc = v;
    }

    void setNgayLap(const string& v) { ngayLap = v; }
    void setHanThanhToan(const string& v) { hanThanhToan = v; }

    void setSoTien(double v) {
        if (v < 0) throw invalid_argument("So tien khong duoc am!");
        soTien = v;
    }

    void setTrangThaiThanhToan(const string& v) { trangThaiThanhToan = v; }

    // Cai dat 9 ham thuan ao tu Entity
    // 1. Khoa chinh dai dien cho hoa don
    string getId() const override { return maHoaDon; }
    // 2. Ten dai dien dung trong tim kiem
    string getDisplayName() const override { return maHopDong + " - " + kyCuoc; }

    // 3. Hien thi chi tiet thong tin hoa don theo chieu doc
    void display() const override {
        cout << "  Ma hoa don       : " << maHoaDon << '\n'
             << "  Ma hop dong      : " << maHopDong << '\n'
             << "  Ky cuoc          : " << kyCuoc << '\n'
             << "  Ngay lap         : " << ngayLap << '\n'
             << "  Han thanh toan   : " << hanThanhToan << '\n'
             << "  So tien          : " << fixed << setprecision(0) << soTien << " VND\n"
             << "  Trang thai       : " << trangThaiThanhToan << endl;
    }

    // 4. In dong tieu de bang danh sach hoa don
    void displayTableHeader() const override {
        cout << "  " << left << setw(6) << "STT" << setw(14) << "Ma HD"
             << setw(14) << "Ma hop dong" << setw(18) << "Ky cuoc"
             << setw(14) << "Ngay lap" << setw(18) << "Han thanh toan"
             << setw(18) << "So tien (VND)" << "Trang thai\n";
        cout << "  " << string(112, '-') << '\n';
    }

    // 5. In tung dong du lieu hoa don trong bang
    void displayTableRow(int stt) const override {
        cout << "  " << left << setw(6) << stt << setw(14) << maHoaDon
             << setw(14) << maHopDong << setw(18) << kyCuoc
             << setw(14) << ngayLap << setw(18) << hanThanhToan
             << setw(18) << fixed << setprecision(0) << soTien
             << trangThaiThanhToan << '\n';
    }

    // 6. Tuan tu hoa du lieu hoa don thanh chuoi de ghi vao file
    string toFileString() const override {
        ostringstream out;
        out << maHoaDon << '|' << maHopDong << '|' << kyCuoc << '|'
            << ngayLap << '|' << hanThanhToan << '|'
            << setprecision(15) << soTien << '|' << trangThaiThanhToan;
        return out.str();
    }

    // 7. Giai tuan tu hoa tu dong van ban doc tu file
    void fromFileString(const string& line) override {
        stringstream ss(line);
        string temp;
        getline(ss, maHoaDon, '|');
        getline(ss, maHopDong, '|');
        getline(ss, kyCuoc, '|');
        getline(ss, ngayLap, '|');
        getline(ss, hanThanhToan, '|');
        getline(ss, temp, '|');
        soTien = stod(temp);
        getline(ss, trangThaiThanhToan);
        if (maHoaDon.empty() || maHopDong.empty() || kyCuoc.empty())
            throw invalid_argument("Du lieu hoa don khong hop le!");
    }

    // 8. Nhap moi thong tin hoa don tu ban phim kem kiem tra rang buoc
    void inputData() override {
        string temp;
        cout << "  Nhap ma hoa don (VD: HDON001): ";
        getline(cin, maHoaDon);
        if (maHoaDon.empty()) throw invalid_argument("Ma hoa don khong duoc de trong!");

        cout << "  Nhap ma hop dong (VD: HD001): ";
        getline(cin, maHopDong);
        if (maHopDong.empty()) throw invalid_argument("Ma hop dong khong duoc de trong!");

        cout << "  Nhap ky cuoc (VD: 10/2026): ";
        getline(cin, kyCuoc);
        if (kyCuoc.empty()) throw invalid_argument("Ky cuoc khong duoc de trong!");

        cout << "  Nhap ngay lap (DD/MM/YYYY): "; getline(cin, ngayLap);
        cout << "  Nhap han thanh toan (DD/MM/YYYY): "; getline(cin, hanThanhToan);

        cout << "  Nhap so tien (VND): ";
        getline(cin, temp);
        try {
            size_t pos = 0;
            soTien = stod(temp, &pos);
            if (pos != temp.size() || soTien < 0) throw invalid_argument("");
        } catch (...) {
            throw invalid_argument("So tien phai la so khong am!");
        }

        cout << "  Nhap trang thai (Chua thanh toan/Da thanh toan/Qua han): ";
        getline(cin, trangThaiThanhToan);
        if (trangThaiThanhToan.empty()) trangThaiThanhToan = "Chua thanh toan";
    }

    // 9. Cap nhat thong tin hoa don (nhan Enter de giu nguyen gia tri cu)
    void updateData() override {
        string temp;
        cout << "  (Nhan Enter de giu nguyen gia tri cu)\n\n";
        cout << "  Ma hop dong [" << maHopDong << "]: ";
        getline(cin, temp); if (!temp.empty()) maHopDong = temp;
        cout << "  Ky cuoc [" << kyCuoc << "]: ";
        getline(cin, temp); if (!temp.empty()) kyCuoc = temp;
        cout << "  Ngay lap [" << ngayLap << "]: ";
        getline(cin, temp); if (!temp.empty()) ngayLap = temp;
        cout << "  Han thanh toan [" << hanThanhToan << "]: ";
        getline(cin, temp); if (!temp.empty()) hanThanhToan = temp;
        cout << "  So tien [" << fixed << setprecision(0) << soTien << "]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            try {
                size_t pos = 0;
                double v = stod(temp, &pos);
                if (pos != temp.size() || v < 0) throw invalid_argument("");
                soTien = v;
            } catch (...) {
                throw invalid_argument("So tien phai la so khong am!");
            }
        }
        cout << "  Trang thai [" << trangThaiThanhToan << "]: ";
        getline(cin, temp); if (!temp.empty()) trangThaiThanhToan = temp;
        if (maHopDong.empty() || kyCuoc.empty())
            throw invalid_argument("Ma hop dong va ky cuoc khong duoc de trong!");
    }
};

#endif