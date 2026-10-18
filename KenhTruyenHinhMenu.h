/*
 * File   : KenhTruyenHinhMenu.h
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 * Mo ta  : Giao dien console cho use case Quan ly kenh truyen hinh (CRUD).
 */
#ifndef KENHTRUYENHINHMENU_H
#define KENHTRUYENHINHMENU_H

#include <vector>
#include "KenhTruyenHinh.h"
#include "Repository.h"
#include "LichPhatSong.h"

class KenhTruyenHinhMenu {
    Repository<KenhTruyenHinh>& dsKenh_;
    Repository<LichPhatSong>& dsLich_;

    void themKenh();
    void xemDanhSach();
    void timKiem();
    void sapXep();
    void capNhat();
    void xoa();

    static void inBang(const std::vector<KenhTruyenHinh>& rows);

public:
    KenhTruyenHinhMenu(Repository<KenhTruyenHinh>& channels, Repository<LichPhatSong>& schedules)
        : dsKenh_(channels), dsLich_(schedules) {}

    /** Vong lap menu con cua use case. */
    void chay();
};

#endif
