#include "syscall.h"

#define SWATER 10   // Số lít nước mỗi sinh viên cần lấy

void main()
{
	/* Tạo các biến cần thiết:
		outputFile	:	OpenFileID của file output.txt
		mailFile	:	OpenFileID của file mailbox.txt
		waitTime    :   thời gian lấy 1 lít nước của sinh viên
		id			:	id của sinh viên
        tmp         :   một chuỗi phụ để phục vụ việc đọc ghi file sẽ dùng bên dưới.
        i, j, k     :   các biến đếm
	*/
    int outputFile, mailFile;
    int waitTime = RandomNum() % 100000;    // Thời gian lấy nước là một số nguyên dương ngẫu nhiên bé hơn 10.000
    char id;
    char* tmp;
    int i, j, k;


    // Quá trình process sinh viên nhận ID từ process cha.
    Wait("send");                       // Đợi process cha gửi message chứa ID
    mailFile = Open("mailbox.txt", 0);  // Mở mailbox
    Read(&id, 1, mailFile);             // Lấy message
	Close(mailFile);                    // Đóng mailbox
    Signal("recv");                     // Thông báo nhận thành công để process thực thi tiếp


    // --- Tiến hành lấy nước ---

    // Mở output.txt để ghi kết quả
    outputFile = Open("output.txt", 0);

    // Mỗi sinh viên lấy nước 10 lần, mỗi lần được mô tả trong vòng lặp sau đây
    for (i=1; i<=SWATER; i++){
        // Đợi vòi nước mở
        Wait("OpenTap");


        // Lấy nước, vòng for thể hiện thời gian lấy nước của sinh viên
        for (j=0; j<waitTime; j++);

        // Ghi vào file kết quả
        while(Read(tmp, 1, outputFile) > 0);    // Tận dụng lệnh read để di chuyển con trỏ đến cuối file
        Write(&id, 1, outputFile);              // Ghi ID sinh viên vào file cho biết thứ tự sinh viên này lấy nước
        Write(" ", 1, outputFile);              // Ghi dấu khoảng trắng


        // Phần in ra màn hình để theo dõi
        for (k = 1; k < (int)(id-'0'); k++) PrintString("      ");
        PrintChar(id);
        PrintString("\n");
        if (i == SWATER) {
            PrintString("(Student ");
            PrintChar(id);
            PrintString(" done)\n");
        }


        // Trả lại vòi nước
        Signal("CloseTap");
    }
    // Đóng file kết quả
    Close(outputFile);

    // Kết thúc process
    Exit(0);
}
