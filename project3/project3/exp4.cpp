//
//  exp4.cpp
//  project3
//
//  Created by 彭瀚林 on 2019/11/20.
//  Copyright © 2019 penghanlin. All rights reserved.
//

#include <iostream>
#include "unistd.h"

using namespace std;

struct Pcb {
    char name[3];
    char number[3];
    char status[2];
    struct Pcb *next;
    struct Pcb *prior;
} stu;
struct Pcb *fir;
struct Pcb *back;

void enter(), display(), remove();
char str[12][3]={"a", "2", "0", "b", "10", "0", "c", "30", "0", "d", "35", "0"};

int main() {
    fir = back = NULL;
    enter(0);
    for(;;) {
        switch (menu()) {
            case 1 : enter(1); break; /*插入或初始化*/
            case 2 : remove(); break; /*删除*/
            case 3 : display(); break; /*显示*/
            case 4 : exit(0);
           }
   }
    return 0;
}

/*主菜单*/
menu( )
{
char ch[2];
int n;
printf("\t就绪队列操作,请选择功能:\n");
printf("\t1.插入进程的信息\n");
printf("\t2.删除进程的信息\n");
printf("\t3.显示进程链表内容\n");
printf("\t4.退出\n");
do{
   printf("\n\t请按数字选择:");
   gets(ch);
   n=atoi(ch);
   }while(n<0 || n>4);
return(n);
}

void enter(int q) {
    int qq;
    struct Pcb *inf, *bc(*char inf, *char fir);

    if(q == 1) {
        inf = (struct Pcb *)malloc(sizeof(stu));
        if (!inf) {
            cout << "User up!" << endl;
            return;
       }
  
        inputs("\t请输入进程标识符(最多两位):",inf->name,2);
  inputs("\t请输入进程优先数(最多两位):",inf->number,2);
  inputs("\t请输入进程当前状态(一位):",inf->status,1);
  fir=bc(inf,fir); /*插入新结点*/
}
  else
    {for(qq=0;qq<4;qq++)     /*结点初始化*/
       {inf=(struct Pcb *)malloc(sizeof(stu));
     if(!inf){
       printf("\tuse up!\n");
       return;
       }
    strcpy(inf->name,str[qq*3]);
    strcpy(inf->number,str[qq*3+1]);
    strcpy(inf->status,str[qq*3+2]);
    fir=bc(inf,fir);
       }
    }
}

Pcb inputs(char *sm, char *s, int count) {
    char q[3];
    do {
        cout << sm;
        gets(q);
        cout << "输入字符串过长" << endl;
    } while (strlen(q) > count);
    strcpy(s, q);

    struct Pcb *bc(Pcb *i, Pcb *st) {
        
    }
    struct Pcb *i;
    struct Pcb *st;
}

{
struct pcb *j,*k;
if(atoi(i->status)==0)  /*判断进程状态是否就绪*/
{
  if(back==NULL)     /*链表为空时的插入*/
  {
  i->next=NULL;
  i->prior=NULL;
  back=i;
  return(i);
 }
 j=st;          /*令J指向链表的头*/
 while(j)
 {if(strcmp(j->name,i->name)==0) /*进程标识符要唯一*/
    {printf("\n\t该进程已存在就绪队列中。\n\n");
    return(st);
    }
  j=j->next;
 }
 j=st;
 k=NULL;
 while(j){
 if(atoi(j->number)<atoi(i->number)) /*判断进程优先数,找到要插入的位置*/
  {
   k=j;
   j=j->next;
  }
  else
   {
     if(j->prior)          /*判断J是否为头指针*/
     {
     j->prior->next=i;     /*在链表的中间插入*/
     i->next=j;
     i->prior=j->prior;
     j->prior=i;
     return(st);
     }
     i->next=j;            /*在链表的头插入*/
     i->prior=NULL;
     j->prior=i;
     return(i);
   }
}
  k->next=i;          /*在链表的尾插入*/
  i->next=NULL;
  i->prior=k;
  back=i;
  return(st);
}
printf("\n\t该进程不是就绪状态,不能插入就绪队列中。\n\n");
return(st);
}

void remove( )
{
struct pcb *in;
char s[2];
printf("\t请输入进程标识符:");
gets(s);
in=fir;
while(in){
if(strcmp(s,in->name)==0)   break; /*寻找要删除的进程*/
  else  in=in->next;
}
if(in==NULL)
  printf("\t未找到此进程!\n");
if(in){            /*找到进程*/
  if(fir==in)     /*该进程为链表头时*/
    {
    fir=in->next;
    if(fir) fir->prior=NULL;  /*该链表删除进程结点后不为空*/
    else back=NULL;
    }
  else{
      in->prior->next=in->next;
      if(in!=back)            /*该进程不在链表尾*/
    in->next->prior=in->prior;
      else back=in->prior;
      }
      free(in);            /*释放进程空间*/
      }
}

void display() {
    struct Pcb *j;
    j = fir;
    
    while (j) {
        cout << j -> name;
        cout << j -> number;
        cout << j -> status;
        cout << endl;
        j = j -> next;
    }
}
