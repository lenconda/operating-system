//
//  exp1.cpp
//  project3
//
//  Created by 彭瀚林 on 2019/11/20.
//  Copyright © 2019 penghanlin. All rights reserved.
//

#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    int p1, p2;
    while ((p1 = fork()) == -1);
    if (p1 == 0)
        cout << 'b';
    else {
        while((p2 = fork()) == -1);
        if(p2 == 0)
            cout << 'c';
        else
            cout << 'a';
    }
    
    cout << endl;
    return 0;
}
