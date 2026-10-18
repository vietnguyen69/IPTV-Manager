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

#include <functional>
#include <string>
#include "Entity.h"

class KenhTruyenHinh : public Entity {
public:
    /** Ham kiem tra rang buoc lien doi tuong (do menu gan vao), nem ValidationException neu vi pham. */
    using KiemTraMa = std::function<void(const std::string&)>;

private:
    std::string ma_;          // Kxxx
    std::string ten_;
    std::string maTheLoai_;     // ma the loai, dang TLxx (tham chieu use case The loai kenh)
    std::string doPhanGiai_;  // SD / HD / FHD / 4K
    int soKenh_ = 1;          // so kenh tren dau thu (1..999)
    bool dangPhat_ = true;      // true: dang phat, false: tam ngung
    KiemTraMa kiemTraMa_;         // vd: kiem tra trung ma trong Repository

public:
    KenhTruyenHinh() = default;

    // ----- getter -----
    const std::string& layTen() const { return ten_; }
    const std::string& layMaTheLoai() const { return maTheLoai_; }
    const std::string& layDoPhanGiai() const { return doPhanGiai_; }
    int laySoKenh() const { return soKenh_; }
    bool laDangPhat() const { return dangPhat_; }

    // ----- setter co kiem tra hop le -----
    void datMa(const std::string& id);
    void datTen(const std::string& name);
    void datMaTheLoai(const std::string& genreId);
    void datDoPhanGiai(const std::string& res);
    void datSoKenh(int number);
    void datTrangThai(bool a) { dangPhat_ = a; }

    /** Gan ham kiem tra ma kenh (vd: khong duoc trung). Goi xoaKiemTra() khi xong. */
    void datKiemTraMa(KiemTraMa f) { kiemTraMa_ = std::move(f); }
    void xoaKiemTra() { kiemTraMa_ = nullptr; }

    // ----- cai dat giao dien Entity -----
    std::string getId() const override { return ma_; }
    std::string getDisplayName() const override { return ten_; }
    void display() const override;
    void displayTableHeader() const override;
    void displayTableRow(int stt) const override;
    std::string toFileString() const override;
    void fromFileString(const std::string& line) override;
    void inputData() override;   // nhap moi tu ban phim
    void updateData() override;  // sua tung truong, Enter = giu nguyen (khong sua ma)
};

#endif
