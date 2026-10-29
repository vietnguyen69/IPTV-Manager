// Tac gia: Duong Duc Manh - B24DCVT240
// Use case quan ly lich phat song

#ifndef LICHPHATSONG_H
#define LICHPHATSONG_H

#include "Entity.h"
#include <iomanip>
#include <stdexcept>

class LichPhatSong : public Entity {
private:
    string maLich;           // Ma dinh danh lich phat song (Khoa chinh, VD: L001, L002...)
    string maKenh;           // Ma kenh truyen hinh (Khoa ngoai -> KenhTruyenHinh, VD: K001)
    string tenChuongTrinh;   // Ten chuong trinh phat song (VD: Thoi su 19h, Ngoai Hang Anh...)
    string ngayPhatSong;     // Ngay phat song (dinh dang DD/MM/YYYY)
    string gioBatDau;        // Gio bat dau phat (dinh dang HH:MM)
    string gioKetThuc;       // Gio ket thuc phat (dinh dang HH:MM, phai > gioBatDau)
    string moTa;             // Mo ta chi tiet ve chuong trinh

    // Ham kiem tra dinh dang va tinh hop le cua chuoi ngay DD/MM/YYYY
    bool isValidDate(const string& date) const {
        if (date.length() != 10) return false;
        if (date[2] != '/' || date[5] != '/') return false;
        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 5) continue;
            if (date[i] < '0' || date[i] > '9') return false;
        }
        int day   = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year  = stoi(date.substr(6, 4));

        if (year < 2020 || year > 2030) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        return true;
    }

    // Ham kiem tra dinh dang va tinh hop le cua chuoi gio HH:MM
    bool isValidTime(const string& time) const {
        if (time.length() != 5) return false;
        if (time[2] != ':') return false;
        for (int i = 0; i < 5; i++) {
            if (i == 2) continue;
            if (time[i] < '0' || time[i] > '9') return false;
        }
        int hour   = stoi(time.substr(0, 2));
        int minute = stoi(time.substr(3, 2));

        if (hour < 0 || hour > 23) return false;
        if (minute < 0 || minute > 59) return false;
        return true;
    }

public:
    // Constructor mac dinh khoi tao cac gia tri rong
    LichPhatSong()
        : maLich(""), maKenh(""), tenChuongTrinh(""),
          ngayPhatSong(""), gioBatDau(""), gioKetThuc(""), moTa("") {}

    // Constructor day du tham so
    LichPhatSong(const string& ml, const string& mk, const string& tcp,
                 const string& nps, const string& gbd, const string& gkt,
                 const string& mt)
        : maLich(ml), maKenh(mk), tenChuongTrinh(tcp),
          ngayPhatSong(nps), gioBatDau(gbd), gioKetThuc(gkt), moTa(mt) {}

    // Cung cap Getter
    string getMaLich() const { return maLich; }
    string getMaKenh() const { return maKenh; }
    string getTenChuongTrinh() const { return tenChuongTrinh; }
    string getNgayPhatSong() const { return ngayPhatSong; }
    string getGioBatDau() const { return gioBatDau; }
    string getGioKetThuc() const { return gioKetThuc; }
    string getMoTa() const { return moTa; }

    // Cung cap Setter co kiem tra hop le
    void setMaLich(const string& ml) { maLich = ml; }
    void setMaKenh(const string& mk) { maKenh = mk; }

    void setTenChuongTrinh(const string& tcp) {
        if (tcp.empty()) throw invalid_argument("Ten chuong trinh khong duoc de trong!");
        tenChuongTrinh = tcp;
    }

    void setNgayPhatSong(const string& nps) {
        if (!isValidDate(nps)) throw invalid_argument("Ngay phat song khong hop le (DD/MM/YYYY)!");
        ngayPhatSong = nps;
    }

    // Thiet lap khung gio kem kiem tra gio bat dau phai truoc gio ket thuc
    void setKhungGio(const string& gbd, const string& gkt) {
        if (!isValidTime(gbd) || !isValidTime(gkt)) {
            throw invalid_argument("Gio phai co dinh dang HH:MM!");
        }
        if (gbd >= gkt) {
            throw invalid_argument("Gio bat dau phai truoc gio ket thuc!");
        }
        gioBatDau = gbd;
        gioKetThuc = gkt;
    }

    void setMoTa(const string& mt) { moTa = mt; }

    // Cai dat 9 ham thuan ao tu lop Entity
    // 1. Khoa chinh dai dien cho lich phat song
    string getId() const override { return maLich; }
    // 2. Ten dai dien dung trong tim kiem
    string getDisplayName() const override { return tenChuongTrinh; }
    // 3. Hien thi chi tiet thong tin lich phat song theo chieu doc
    void display() const override {
        cout << "  Ma lich        : " << maLich << endl;
        cout << "  Ma kenh        : " << maKenh << endl;
        cout << "  Chuong trinh   : " << tenChuongTrinh << endl;
        cout << "  Ngay phat song : " << ngayPhatSong << endl;
        cout << "  Khung gio      : " << gioBatDau << " - " << gioKetThuc << endl;
        cout << "  Mo ta          : " << moTa << endl;
    }

    // 4. In dong tieu de bang danh sach
    void displayTableHeader() const override {
        cout << "  " << left
             << setw(6)  << "STT"
             << setw(10) << "Ma lich"
             << setw(10) << "Ma kenh"
             << setw(26) << "Ten chuong trinh"
             << setw(14) << "Ngay phat"
             << setw(16) << "Khung gio"
             << endl;
        cout << "  " << string(82, '-') << endl;
    }

    // 5. In dong du lieu trong bang
    void displayTableRow(int stt) const override {
        string tcpShort = tenChuongTrinh.length() > 24 ? tenChuongTrinh.substr(0, 21) + "..." : tenChuongTrinh;
        cout << "  " << left
             << setw(6)  << stt
             << setw(10) << maLich
             << setw(10) << maKenh
             << setw(26) << tcpShort
             << setw(14) << ngayPhatSong
             << setw(16) << (gioBatDau + " - " + gioKetThuc)
             << endl;
    }

    // 6. Tuan tu hoa du lieu thanh chuoi de ghi vao file
    string toFileString() const override {
        return maLich + "|" + maKenh + "|" + tenChuongTrinh + "|"
             + ngayPhatSong + "|" + gioBatDau + "|" + gioKetThuc + "|" + moTa;
    }

    // 7. Giai tuan tu hoa tu dong van ban doc tu file
    void fromFileString(const string& line) override {
        stringstream ss(line);
        getline(ss, maLich, '|');
        getline(ss, maKenh, '|');
        getline(ss, tenChuongTrinh, '|');
        getline(ss, ngayPhatSong, '|');
        getline(ss, gioBatDau, '|');
        getline(ss, gioKetThuc, '|');
        getline(ss, moTa, '|');
    }

    // 8. Nhap moi du lieu tu ban phim kem kiem tra rang buoc
    void inputData() override {
        cout << "  Nhap ma lich (VD: L001): ";
        getline(cin, maLich);
        if (maLich.empty()) throw invalid_argument("Ma lich khong duoc de trong!");

        cout << "  Nhap ma kenh (VD: K001): ";
        getline(cin, maKenh);
        if (maKenh.empty()) throw invalid_argument("Ma kenh khong duoc de trong!");

        cout << "  Nhap ten chuong trinh: ";
        getline(cin, tenChuongTrinh);
        if (tenChuongTrinh.empty()) throw invalid_argument("Ten chuong trinh khong duoc de trong!");

        cout << "  Nhap ngay phat song (DD/MM/YYYY): ";
        getline(cin, ngayPhatSong);
        if (!isValidDate(ngayPhatSong)) throw invalid_argument("Ngay phat song khong hop le (DD/MM/YYYY)!");

        cout << "  Nhap gio bat dau (HH:MM): ";
        getline(cin, gioBatDau);
        cout << "  Nhap gio ket thuc (HH:MM): ";
        getline(cin, gioKetThuc);
        setKhungGio(gioBatDau, gioKetThuc);

        cout << "  Nhap mo ta: ";
        getline(cin, moTa);
    }

    // 9. Cap nhat thong tin lich (nhan Enter de giu nguyen gia tri cu)
    void updateData() override {
        string temp;
        cout << "  (Nhan Enter de giu nguyen gia tri cu)\n" << endl;

        cout << "  Ma kenh [" << maKenh << "]: ";
        getline(cin, temp);
        if (!temp.empty()) maKenh = temp;

        cout << "  Ten chuong trinh [" << tenChuongTrinh << "]: ";
        getline(cin, temp);
        if (!temp.empty()) tenChuongTrinh = temp;

        cout << "  Ngay phat song [" << ngayPhatSong << "]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            if (!isValidDate(temp)) throw invalid_argument("Ngay phat song khong hop le (DD/MM/YYYY)!");
            ngayPhatSong = temp;
        }

        cout << "  Gio bat dau [" << gioBatDau << "]: ";
        string tempStart;
        getline(cin, tempStart);

        cout << "  Gio ket thuc [" << gioKetThuc << "]: ";
        string tempEnd;
        getline(cin, tempEnd);

        string start = tempStart.empty() ? gioBatDau : tempStart;
        string end   = tempEnd.empty() ? gioKetThuc : tempEnd;
        setKhungGio(start, end);

        cout << "  Mo ta [" << moTa << "]: ";
        getline(cin, temp);
        if (!temp.empty()) moTa = temp;
    }
};

#endif