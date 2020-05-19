#include "hint.h"

int Hint::getHint(QString text)
{
    int result = -1;
    for(int i=0;i<COUNT;i++)
    {
        if(text == hints[i][0])
        {
            result = i;
            break;
        }
    }
    return result;
}
