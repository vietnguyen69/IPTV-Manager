/*
 * File   : LichPhatSongMenu.h
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 * Mo ta  : Giao dien console cho use case Quan ly lich phat song (CRUD).
 */
#ifndef LICHPHATSONGMENU_H
#define LICHPHATSONGMENU_H

#include <string>
#include <vector>
#include "KenhTruyenHinh.h"
#include "Repository.h"
#include "LichPhatSong.h"

class LichPhatSongMenu {
    Repository<KenhTruyenHinh>& dsKenh_;
    Repository<LichPhatSong>& dsLich_;

    void themLich();
    void xemDanhSach();
    void timKiem();
    void sapXep();
    void capNhat();
    void xoa();

    /** Gan cac rang buoc lien doi tuong (ma kenh ton tai, khong trung khung gio) vao lich sc. */
    void ganRangBuoc(LichPhatSong& sc);
    std::string tenKenh(const std::string& channelId);
    void inBang(const std::vector<LichPhatSong>& rows);

public:
    LichPhatSongMenu(Repository<KenhTruyenHinh>& channels, Repository<LichPhatSong>& schedules)
        : dsKenh_(channels), dsLich_(schedules) {}

    /** Vong lap menu con cua use case. */
    void chay();
};

#endif
