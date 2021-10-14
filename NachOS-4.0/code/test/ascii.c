#include "syscall.h"

int
main()
{
    int i;
    PrintString("Dec     |     Char \n");
    PrintString("--------|----------\n");
    /* Ki tu tu 32 den 127 co the nhin thay doc duoc */
    /* Ki tu 32 */
    PrintString("32\t|    [SPACE]\n");
    /* Ki tu tu 33 den 126 */
    for (i = 33; i < 126; ++i){
        PrintNum(i);
        PrintString("\t|\t");
        PrintChar(i);
        PrintChar('\n');
    }
    /*Ki tu 127 */
    PrintString("127\t|     [DEL]\n");
    Halt();
    /* not reached */
}