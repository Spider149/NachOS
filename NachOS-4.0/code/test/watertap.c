#include "syscall.h"


void main()
{
    while (1) {
        /*  Mở vòi nước để sinh viên lấy nước để các sinh viên lần lượt lấy nước.
            Cần lấy lại vòi sau mỗi lượt để tránh tình trạng sinh viên không ngắt được chương trình lấy nước của 1 sinh viên.
            Chương trình sẽ kết thúc khi chương trình chính kết thúc.
        */
        Signal("OpenTap");
        Wait("CloseTap");
    }
}
