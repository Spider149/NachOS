#include "syscall.h"

#define SWATER 10   // Số lít nước mỗi sinh viên cần lấy

void main()
{
	/* Tạo các biến cần thiết:
		outputFile	:	OpenFileID của file output.txt
		mailFile	:	OpenFileID của file mailbox.txt
		waitTime    :   thời gian lấy nước của sinh viên
		id			:	id của sinh viên
        tmp         :   một chuỗi phụ để phụ vụ việc đọc ghi file sẽ dùng bên dưới.
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
    for (i=0; i<SWATER; i++){
        // Đợi vòi nước mở
        Wait("WaterTap");

        // Lấy nước, vòng for thể hiện thời gian lấy nước của sinh viên
        for (j=0; j<waitTime; j++);

        // Ghi vào file kết quả
        while(Read(tmp, 1, outputFile) > 0);    // Tận dụng lệnh read để di chuyển con trỏ đến cuối file
        Write(&id, 1, outputFile);              // Ghi ID sinh viên vào file cho biết thứ tự sinh viên này lấy nước
        tmp = " ";
        Write(tmp, 1, outputFile);              // GHi dấu khoảng trắng

        // Phần in ra màn hình để theo dõi
        for (k = 1; k < (int)(id-'0'); k++) PrintString("      ");
        PrintChar(id);
        PrintString("\n");

        // Trả lại vòi nước
        Signal("WaterTap");
    }
    Close(outputFile);  // Đóng file kết quả
    
    // Thông báo lên màn hình đã thực hiện xong
    PrintString("(Student ");
    PrintChar(id);
    PrintString(" done)\n");

    // Kết thúc process
    Exit(0);
}
