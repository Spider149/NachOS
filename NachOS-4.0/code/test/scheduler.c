#include "syscall.h"

#define MAX_STUDENT 9	// Số lượng sinh viên tối đa

void main()
{
	/*Tạo các biến cần thiết:
		outputFile	:	OpenFileID của file output.txt
		mailFile	:	OpenFileID của file mailbox.txt
		numStudent	:	số lượng sinh viên lấy nước
		id			:	id của sinh viên
		SIDs		:	SpaceID của các tiến trình con
		output		:	chuỗi chứa kết quả
		i, j		:	biến đếm
	*/
	int outputFile, mailFile;
	int numStudent;
	char id;
	int SIDs[10];
	char* output;
	int i, j;


	/* Tạo các semaphore cần thiết:
		WaterTap	:	semaphore quản lí việc sử dụng vòi nước,
						trong một thời điểm chỉ có tối đa một người dùng
		send		:	semaphore quản lí việc nhận message,
						chỉ khi nhận được message từ process cha thì process con mới thực thi tiếp
		send		:	semaphore quản lí việc gửi message,
						chỉ khi process con nhận được message thì process cha mới thực thi tiếp
	*/
	CreateSemaphore("WaterTap", 0);		// Ban đầu vòi nước đóng. Vòi nước sẽ được mở phía bên dưới.
	CreateSemaphore("send", 0);
	CreateSemaphore("recv", 0);

	/* Tạo các file cần thiết:
		output.txt	:	file chứa kết quả
		mailbox.txt	:	file chứa các message được trao đổi giữa các process,
						cụ thể ở bài này, chúng em tạo ra file mainbox.txt để process cha 
						có thể gửi cho các process sinh viên biết về mã sinh viên của nó
	*/
	CreateFile("output.txt");
	CreateFile("mailbox.txt");


	// Nhập số lượng sinh viên lấy nước, phải là một số từ 1 đến 9, nếu không đúng sẽ yêu cầu nhập lại.
	do {
		PrintString("Enter number of student: ");
		numStudent = ReadNum();
	} while (numStudent < 1 || MAX_STUDENT < numStudent);


	// Tạo các process con để thực thi chương trình student
	for (i=1; i<=numStudent; i++) {
		// Tạo nhánh mới thực thi chương trình student. Lưu lại SpaceID trong mảng SIDs.
		SIDs[i] = Exec("student");

		// Tạo ID cho process con vừa được phân nhánh
		id = (char)(i + '0');

		// Ghi message chứa ID của process con vào file mailbox.txt
		mailFile = Open("mailbox.txt", 0);
		Write(&id, 1, mailFile);
		Close(mailFile);

		// Đợi process con nhận được message rồi tiếp tục
		Signal("send");		// Đánh dấu đã gửi
		Wait("recv");		// Đợi phản hồi
	}


	// --- Mở vòi nước và cho các sinh viên lấy nước ---
	PrintString("-------------------------------\n");
	// Mở vòi nước.
	Signal("WaterTap");

	// Đợi các sinh viên lấy nước xong.
	for (i=1; i<=numStudent; i++) 
		if (SIDs[i] >= 0)
			Join(SIDs[i]);

	PrintString("-------------------------------\n");


	// Kiểm tra file output.txt
	PrintString("\nThu tu lay nuoc la: \n");

	outputFile = Open("output.txt", 0);
	while(Read(output, 1, outputFile) > 0) PrintString(output);
	Close(outputFile);


	// Kết thúc chương trình
    Exit(0);
}
