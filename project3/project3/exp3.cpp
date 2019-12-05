//
//  exp3.cpp
//  project3
//
//  Created by 彭瀚林 on 2019/11/20.
//  Copyright © 2019 penghanlin. All rights reserved.
//

#include <iostream>
#include "unistd.h"

using namespace std;

int main() {
    int p1, p2, i;
    while ((p1 = fork()) == -1);
    if (p1 == 0) {
        lockf(1, 1, 0);
        for (i = 0; i < 500; i++) {
            cout << "Child " << i << endl;
        }
        lockf(1, 0, 0);
    } else {
        while ((p2 = fork()) == -1);
        if (p2 == 0) {
            lockf(1, 1, 0);
            for (i = 0; i < 500; i++) {
                cout << "Son " << i << endl;
            }
            lockf(1, 0, 0);
        } else {
            lockf(1, 1, 0);
            for (i = 0; i < 500; i++) {
                cout << "Daughter " << i << endl;
            }
            lockf(1, 1, 0);
        }
    }
    return 0;
}
