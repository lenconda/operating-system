//
//  main.cpp
//  project3
//
//  Created by 彭瀚林 on 2019/11/20.
//  Copyright © 2019 penghanlin. All rights reserved.
//

#include <iostream>
#include "unistd.h"
#include "signal.h"

using namespace std;

void child(int signo) {
    cout << "Child process " << getpid() << " signal: SIGUSR1, signo: " << signo << endl;
}

void parent(int signo) {
    cout << "Parent process " << getpid() << " signal: SIGUSR2, signo: " << signo << endl;
}

int main() {
    int pid, ppid;
    signal(SIGCHLD, SIG_IGN);
    
    if ((pid = fork()) == 0) {
        signal(SIGUSR1,child);
        ppid=getppid();
        sleep(1);
        cout << "Parent process " << getpid() << " send signal " << SIGUSR2 << " to process " << pid << endl;
        kill(ppid,SIGUSR2);
    } else {
        signal(SIGUSR2,parent);
        sleep(1);
        cout << "Parent process " << getpid() << " send signal " << SIGUSR1 << " to process " << pid << endl;
        kill(pid, SIGUSR1);
    }
    
    return 0;
}
