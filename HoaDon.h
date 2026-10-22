// Use Case: Quan ly Hoa don

#ifndef HOADON_H
#define HOADON_H

#include "Entity.h"
#include <iomanip>
#include <stdexcept>

class HoaDon : public Entity {
private:
    string maHoaDon;
    string maHopDong;
    string kyCuoc;
    string ngayLap;
    string hanThanhToan;
    double soTien;
    string trangThaiThanhToan;

public:
    HoaDon()
        : maHoaDon(""), maHopDong(""), kyCuoc(""), ngayLap(""),
          hanThanhToan(""), soTien(0), trangThaiThanhToan("Chua thanh toan") {}

    HoaDon(const string& ma, const string& mhd, const string& ky,
           const string& nl, const string& han, double tien, const string& tt)
        : maHoaDon(ma), maHopDong(mhd), kyCuoc(ky), ngayLap(nl),
          hanThanhToan(han), soTien(tien), trangThaiThanhToan(tt) {
        if (soTien < 0) throw invalid_argument("So tien khong duoc am!");
    }

    string getMaHoaDon() const { return maHoaDon; }
    string getMaHopDong() const { return maHopDong; }
    string getKyCuoc() const { return kyCuoc; }
    string getNgayLap() const { return ngayLap; }
    string getHanThanhToan() const { return hanThanhToan; }
    double getSoTien() const { return soTien; }
    string getTrangThaiThanhToan() const { return trangThaiThanhToan; }

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

    string getId() const override { return maHoaDon; }
    string getDisplayName() const override { return maHopDong + " - " + kyCuoc; }

    void display() const override {
        cout << "  Ma hoa don       : " << maHoaDon << '\n'
             << "  Ma hop dong      : " << maHopDong << '\n'
             << "  Ky cuoc          : " << kyCuoc << '\n'
             << "  Ngay lap         : " << ngayLap << '\n'
             << "  Han thanh toan   : " << hanThanhToan << '\n'
             << "  So tien          : " << fixed << setprecision(0) << soTien << " VND\n"
             << "  Trang thai       : " << trangThaiThanhToan << endl;
    }

    void displayTableHeader() const override {
        cout << "  " << left << setw(6) << "STT" << setw(14) << "Ma HD"
             << setw(14) << "Ma hop dong" << setw(18) << "Ky cuoc"
             << setw(14) << "Ngay lap" << setw(18) << "Han thanh toan"
             << setw(18) << "So tien (VND)" << "Trang thai\n";
        cout << "  " << string(112, '-') << '\n';
    }

    void displayTableRow(int stt) const override {
        cout << "  " << left << setw(6) << stt << setw(14) << maHoaDon
             << setw(14) << maHopDong << setw(18) << kyCuoc
             << setw(14) << ngayLap << setw(18) << hanThanhToan
             << setw(18) << fixed << setprecision(0) << soTien
             << trangThaiThanhToan << '\n';
    }

    string toFileString() const override {
        ostringstream out;
        out << maHoaDon << '|' << maHopDong << '|' << kyCuoc << '|'
            << ngayLap << '|' << hanThanhToan << '|'
            << setprecision(15) << soTien << '|' << trangThaiThanhToan;
        return out.str();
    }

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
        try { size_t pos = 0; soTien = stod(temp, &pos);
              if (pos != temp.size() || soTien < 0) throw invalid_argument(""); }
        catch (...) { throw invalid_argument("So tien phai la so khong am!"); }

        cout << "  Nhap trang thai (Chua thanh toan/Da thanh toan/Qua han): ";
        getline(cin, trangThaiThanhToan);
        if (trangThaiThanhToan.empty()) trangThaiThanhToan = "Chua thanh toan";
    }

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
            try { size_t pos = 0; double v = stod(temp, &pos);
                  if (pos != temp.size() || v < 0) throw invalid_argument("");
                  soTien = v; }
            catch (...) { throw invalid_argument("So tien phai la so khong am!"); }
        }
        cout << "  Trang thai [" << trangThaiThanhToan << "]: ";
        getline(cin, temp); if (!temp.empty()) trangThaiThanhToan = temp;
        if (maHopDong.empty() || kyCuoc.empty())
            throw invalid_argument("Ma hop dong va ky cuoc khong duoc de trong!");
    }
};

#endif // HOADON_H
