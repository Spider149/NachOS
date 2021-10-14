#ifdef UNIX
#include <stdio.h>
#define Exit exit
#else
#include "syscall.h"
#endif /* UNIX */

#define SIZE (1024)

int A[SIZE];	/* size of physical memory; with code, we'll run out of space!*/

int
main()
{
    int i, j, tmp, n, type;

    /* Nguoi dung nhap vao so phan tu cua mang - n */
    PrintString("Moi ban nhap so luong phan tu n (1 <= n <= 100):\n");
    n = ReadNum();
    /* Kiem tra dieu kien cua n */
    if (n < 1 || n > 100){
        PrintString("Khong thoa man yeu cau 1 <= n <= 100\n");
        Halt();
        return 0;
    }
    
    /* Nguoi dung nhap mang gom n so nguyen */
    PrintString("Nhap cac phan tu cua mang:\n");
    for (i = 0; i < n; i++) {
        A[i] = ReadNum();
    }

    /* Nguoi dung chon cach sap xep mang tang dan hay giam dan */
    PrintString("Chon cach sap xep mang tang dan hay giam dan \n  Tang dan: 0 \n  Giam dan: 1\n");
    type = ReadNum();
    if (type != 0 && type != 1){
        PrintString("Khong thoa man yeu cau chon loai\n");
        Halt();
        return 0;
    }

    /* Sap xep theo thuat toan Bubble Sort */
    PrintString("Dang sap xep...\n");
    for (i = 0; i < n; ++i){
        for (j = n - 1; j  > i; --j){
            if ((type == 0 && A[j] >= A[j-1]) || (type == 1 && A[j] <= A[j-1]))
                continue;
            tmp = A[j];
            A[j] = A[j-1];
            A[j-1] = tmp;
        }
    }

    /* In ket qua sap xep ra man hinh */
    PrintString("Mang sau khi sap xep la:\n");
    for (i = 0; i < n; ++i){
        PrintNum(A[i]);
        PrintChar(' ');
    }
    PrintChar('\n');

    Halt();
    /* not reached */

}
