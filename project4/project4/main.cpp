//
//  main.cpp
//  project4
//
//  Created by 彭瀚林 on 2019/11/27.
//  Copyright © 2019 penghanlin. All rights reserved.
//

#include <iostream>
#include <stdlib.h>

using namespace std;

typedef struct {
    char name[20];
    int time;
    int priv;
    char state;
} Data;

typedef struct Pro {
    Data data;
    Pro *next;
} Node;

typedef Node *Head;

Head head;

int h = 5, k = 0, y = 5;

Head create_process() {
    Head head = (Node *)malloc(sizeof(Node));
    Node *p, *head1 = head;
    int i = 1, d, o, m;
    cout << "Process count: ";
    cin >> m;
    k = m;
    h = m;
    
    while (i <= m) {
        p = (Node *)malloc(sizeof(Node));
        cout << "The No." << i << " process info    name    time    priority: " << endl;
        cin >> p -> data.name >> d >> o;
        p -> data.time = d;
        p -> data.priv = o;
        p -> data.state = 'R';
        head1 -> next = p;
        head1 = p;
        i += 1;

    }
    
    head1 -> next = NULL;
    system("cls");
    return head;
}

void print_processes(Head head) {
    Node *p;
    int i = 1;
    if (h != 0) {
        p = head -> next;
        while (i < k) {
            cout << "name: " << p -> data.name << ", time: " << p -> data.time << ", priority: " << p -> data.priv << ", state: " << p -> data.state;
            i += 1;
        }
    } else {
        cout << "process ended";
    }
    
    system("cls");
}

Head arrenge_processes(Head head) {
    Node *p, *p1, *p2, *p3;
    p3 = head;
    int t = 0;
    p = head -> next;
    while (true) {
        if (t == 0) {
            break;
        }
        p = head -> next;
        p1 = p -> next;
        p2 = head;
        int n = 0;
        for (int l = 1; l < h; ++l) {
            if (p -> data.priv < p1 -> data.priv) {
                p -> next = p1 -> next;
                p1 -> next = p;
                if (p == p3 -> next) {
                    p3 -> next = p1;
                } else {
                    p2 -> next = p1;
                }
                p2 = p1;
                p1 = p -> next;
                n += 1;
            } else {
                p2 =  p;
                p = p -> next;
                p1 = p1 -> next;
                if (p == p3 -> next) {
                    p3 -> next = p;
                }
            }
            
            if (l == h - 1 && n == 0) {
                t = 0;
            }
        }
    }
    
    return p3;
}

Head run_processes(Head head) {
    Node *p, *p1, *p2;
    if (h != 0) {
        p = head -> next;
        cout << "process: " << p -> data.name << ", remains: " << y << ", count: " << h;
        if (p -> data.priv != 0) {
            p -> data.time = p -> data.time - 1;
            y -= 1;
            if (y == 0) {
                y = 5;
                p -> data.priv = p -> data.priv - 1;
                p2 = p;
                for (int i = 1; i < h; ++i) {
                    p2 = p2 -> next;
                }
            }
        } else {
            p -> data.time = p -> data.time - 1;
            p2 = p;
            for (int i = 1; i < h; i++) {
                p2 = p2 -> next;
                p2 -> data.priv = p2 -> data.priv + 1;
            }
        }
        
        if (p -> data.time == 0) {
            head -> next = p -> next;
            h -= 1;
            p -> data.priv = 0;
            p -> data.state = 'E';
            p1 = p;
            for (int i = k - 1; i > 0; --i) {
                p1 = p1 -> next;
            }
            p1 -> next = p;
            p -> next = NULL;
        }
        
        cout << "Current process info. time: " << p -> data.time << ", priority: " << p -> data.priv << ", state: " << p -> data.state;
    }
    
    return head;
}

int main(int argc, const char * argv[]) {
    Head head;
    head = create_process();
    head = arrenge_processes(head);
    while (true) {
        if (h > 1) {
            head = arrenge_processes(head);
        }
        print_processes(head);
    }
    
    return 0;
}
