#define _CRT_SECURE_NO_WARNINGS  // Disable the warning about using unsafe functions

#include <iostream>
#include "User.h"


int main() 
{
    User user;

    user.MakeRequest();
    user.MakeRequest();
    user.MakeRequest();

    user.ShowHistory();
    
    return 0;
}





