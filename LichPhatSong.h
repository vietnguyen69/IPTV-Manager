/*
 * File   : LichPhatSong.h
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 * Mo ta  : Lop LichPhatSong - doi tuong "Lich phat song" (use case Quan ly lich phat song).
 *          Ke thua Entity. Tham chieu den KenhTruyenHinh qua maKenh_ (khoa ngoai).
 *
 * Dinh dang dong trong file: MaLich|MaKenh|TenChuongTrinh|Ngay|GioBatDau|GioKetThuc|LoaiPhat
 */
#ifndef LICHPHATSONG_H
#define LICHPHATSONG_H

#include <functional>
#include <string>
#include "DateTime.h"
#include "Entity.h"

class LichPhatSong : public Entity {
public:
    /** Loai phat song cua chuong trinh. */
    enum class LoaiPhat { TrucTiep = 1, PhatLai = 2, Moi = 3 };

    /** Cac ham kiem tra rang buoc lien doi tuong (do menu gan vao), nem ValidationException neu vi pham. */
    using KiemTraMa = std::function<void(const std::string&)>;
    using KiemTraKhungGio = std::function<void(const LichPhatSong&)>;

private:
    std::string ma_;         // Lxxx
    std::string maKenh_;  // khoa ngoai -> KenhTruyenHinh
    std::string tenChuongTrinh_;
    Date ngay_;
    TimeOfDay gioBatDau_, gioKetThuc_;
    LoaiPhat loai_ = LoaiPhat::Moi;

    KiemTraMa kiemTraMa_;         // kiem tra trung ma lich
    KiemTraMa kiemTraKenh_;    // kiem tra ma kenh co ton tai
    KiemTraKhungGio kiemTraKhungGio_;     // kiem tra khung gio khong trung lich khac

    void nhapKhungGio(bool update);  // nhap/sua ngay + gio bat dau + gio ket thuc
    void nhapLoai(bool update);

public:
    LichPhatSong() = default;

    // ----- getter -----
    const std::string& layMaKenh() const { return maKenh_; }
    const std::string& layTenChuongTrinh() const { return tenChuongTrinh_; }
    const Date& layNgay() const { return ngay_; }
    const TimeOfDay& layGioBatDau() const { return gioBatDau_; }
    const TimeOfDay& layGioKetThuc() const { return gioKetThuc_; }
    LoaiPhat layLoai() const { return loai_; }

    // ----- setter co kiem tra hop le -----
    void datMa(const std::string& id);
    void datMaKenh(const std::string& cid);
    void datTenChuongTrinh(const std::string& title);
    void datNgay(const Date& d) { ngay_ = d; }
    void datKhungGio(const TimeOfDay& start, const TimeOfDay& end);
    void datLoai(LoaiPhat t) { loai_ = t; }

    void datKiemTraMa(KiemTraMa f) { kiemTraMa_ = std::move(f); }
    void datKiemTraKenh(KiemTraMa f) { kiemTraKenh_ = std::move(f); }
    void datKiemTraKhungGio(KiemTraKhungGio f) { kiemTraKhungGio_ = std::move(f); }
    void xoaKiemTra() { kiemTraMa_ = nullptr; kiemTraKenh_ = nullptr; kiemTraKhungGio_ = nullptr; }

    /** Kiem tra gio ket thuc phai sau gio bat dau (nem ValidationException). */
    static void kiemTraKhoang(const TimeOfDay& start, const TimeOfDay& end);

    /** Hai lich co trung khung gio (cung kenh, cung ngay) hay khong. */
    bool trungKhungGio(const LichPhatSong& other) const;

    static std::string loaiThanhChuoi(LoaiPhat t);
    static LoaiPhat loaiTuChuoi(const std::string& s);  // nem ValidationException

    // ----- cai dat giao dien Entity -----
    std::string getId() const override { return ma_; }
    std::string getDisplayName() const override { return tenChuongTrinh_; }
    void display() const override;
    void displayTableHeader() const override;
    void displayTableRow(int stt) const override;
    std::string toFileString() const override;
    void fromFileString(const std::string& line) override;
    void inputData() override;
    void updateData() override;

    /** In mot dong kem ten kenh (do menu tra tu ma kenh). */
    void inDongCoTenKenh(int stt, const std::string& tenKenh) const;
};

#endif
