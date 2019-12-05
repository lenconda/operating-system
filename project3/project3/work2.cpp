//
//  work2.cpp
//  project3
//
//  Created by 彭瀚林 on 2019/11/20.
//  Copyright © 2019 penghanlin. All rights reserved.
//

#include <iostream>
#include "unistd.h"

using namespace std;

int main() {
    int r, p1, p2, fd[2];
    char buf[50], s[50];
    pipe(fd);
    while ((p1 = fork()) == -1);
    if (p1 == 0) {
        lockf(fd[1], 1, 0);
        cout << "Child process P1 is sending messages!" << endl;
        cout << "Child proces P1" << endl;
        write(fd[1], buf, 50);
        sleep(1);
        lockf(fd[1], 0, 0);
        exit(0);
    } else {
        while ((p2 = fork()) == -1);
        if (p2 == 0) {
            lockf(fd[1], 1, 0);
            cout << "Child process P2 is sending messages!" << endl;
            cout << "Child process P2" << endl;
            write(fd[1], buf, 50);
            sleep(1);
            lockf(fd[1], 0, 0);
            exit(0);
        }
        
        wait(0);
        
        if ((r = read(fd[0], s, 50)) == -1) {
            cout << "Cannot read the pipe!" << endl;
        } else {
            cout << s << endl;
        }
        
        wait(0);
        
        if ((r = read(fd[0], s, 50)) == -1) {
            cout << "Cannot read the pipe!" << endl;
        } else {
            cout << s << endl;
        }
        
        exit(0);
    }
}
