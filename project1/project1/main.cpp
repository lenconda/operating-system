//
//  main.cpp
//  project1
//
//  Created by 彭瀚林 on 2019/11/13.
//  Copyright © 2019 penghanlin. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFFERSIZE 80

extern char *get_current_dir_name(void);
extern char *getenv(const char *name);
extern pid_t waitpid(pid_t pid, int *status, int options);

char buffer[BUFFERSIZE+1];

int file_exsists(char *comm) {
    char *path, *p;
    int i;

    i = 0;
    path = getenv("PATH");
    p = path;
    while (*p != '\0') {
        if (*p != ':')
            buffer[i++] = *p;
        else {
          buffer[i++] = '/';
          buffer[i] = '\0';

          strcat(buffer, comm);
         
          if (access(buffer, F_OK) == 0)
              return 0;
          else
             i = 0;
       }
       p++;
    }
                
    return -1;
}

int pipel(char *input, int len) {
    char *argv[2][30];
    int i, j, k, count, is_back = 0;
    int li_comm = 0, fd[2], fpip[2];
    char lc_char, lc_end[1];
    pid_t child1, child2;

    //管道的命令解析过程
    for (i = 0, j = 0, k = 0; i <= len; i++) {
        if (input[i]== ' ' || input[i] == '\t' || input[i] == '\0' || input[i] == '|') {
            if (input[i] == '|' ) {
                if (j > 0) {
                    buffer[j++] = '\0';
                    argv[li_comm][k] = (char *) malloc(sizeof(char) * j);
                    strcpy(argv[li_comm][k++],buffer);
                }
        
                argv[li_comm][k++] = (char *) 0;
                li_comm++;
                count = k;
                k = 0;
                j = 0;
            }
      
            if (j == 0)
                continue;
            else {
                buffer[j++] = '\0';
                argv[li_comm][k] = (char *) malloc(sizeof(char) * j);
                strcpy(argv[li_comm][k], buffer);
                k++;
            }
            j = 0;
        } else {
            if (input[i] == '&' && input[i+1] == '\0') {
                is_back = 1;
                continue;
            }
            buffer[j++] = input[i];
        }
    }
    
    argv[li_comm][k++] = (char *) 0;

    if (file_exsists(argv[0][0]) == -1 ) {
        printf("This first command is not found!\n");
        for (i = 0; i < count; i++)
            free(argv[0][i]);
        return 0;
    }

    if (pipe(fd) == -1) {
        printf("open pipe error!\n");
        return -1;
    }

    if ((child1 = fork()) == 0) {
        close(fd[0]);
        if (fd[1] != STDOUT_FILENO) {
            if (dup2(fd[1], STDOUT_FILENO) == -1) {
                printf("Redirect Standard Out Error\n");
                return -1;
            }
            close(fd[1]);
        }
        
        execv(buffer, argv[0]);
    } else {
        waitpid(child1, &li_comm, 0);

        lc_end[0] = 0x1a;
        write(fd[1], lc_end, 1);
        close(fd[1]);

        if (file_exsists(argv[1][0]) == -1) {
            printf("This command is not founded!\n");
            for(i=0; i<k; i++)
                free(argv[1][i]);
            return 0;
        }

        if ((child2 = fork()) == 0) {
            if (fd[0] != STDIN_FILENO){
                if(dup2(fd[0], STDIN_FILENO) == -1) {
                    printf("Redirect Standard In Error!\n");
                    return -1;
                }
                
                close(fd[0]);
            }
      
            execv(buffer, argv[1]);
        } else
            if (is_back == 0)
                waitpid(child2, NULL, 0);
    }
  
    for (i=0; i<count; i++)
        free(argv[0][i]);
    for (i=0; i<k; i++)
        free(argv[1][i]);
    return 0;
}

int redirect(char *in, int len) {
    char *argv[30], *filename[2];
    pid_t pid;
    int i, j, k, fd_in, fd_out, is_in = -1, is_out = -1, num = 0;
    int is_back = 0, status = 0;

    for (i = 0, j = 0, k = 0; i <= len; i++) {
        if (in[i] == ' ' || in[i] == '\t' || in[i] == '\0' || in[i] == '<' || in[i] == '>') {
            if (in[i] == '>' || in[i] == '<') {
                if (num < 3) {
                    num ++;
                    if (in[i] == '<')
                        is_in = num - 1;
                    else
                        is_out = num - 1;

                    if (j > 0 && num == 1) {
                        buffer[j++] = '\0';
                        argv[k] = (char *) malloc(sizeof(char) * j);
                        strcpy(argv[k], buffer);
                        k++;
                        j = 0;
                    }
                } else {
                    printf("The format is error!\n");
                    return -1;
                }
            }

            if (j == 0)
                continue;
            else {
                buffer[j++] = '\0';
                if (num == 0) {
                    argv[k] = (char *) malloc(sizeof(char) * j);
                    strcpy(argv[k], buffer);
                    k++;
                } else {
                    filename[status] = (char *) malloc(sizeof(char) * j);
                    strcpy(filename[status++], buffer);
                }
                j = 0;
            }
        } else {
            if (in[i] == '&' && in[i+1] == '\0') {
                is_back = 1;
                continue;
            }
            
            buffer[j++] = in[i];
        }
    }

    argv[k] = (char *) 0;

    if (file_exsists(argv[0]) == -1 ) {
        printf("This command is not founded!\n");
        for(i = 0; i < k; i++)
            free(argv[i]);
        return 0;
    }

    if ((pid = fork()) == 0) {
        if (is_out != -1)
            if((fd_out = open(filename[is_out], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {
                printf("Open out %s Error\n", filename[is_out]);
                return -1;
            }

            if (is_in != -1)
                if((fd_in = open(filename[is_in], O_RDONLY, S_IRUSR | S_IWUSR)) == -1) {
                    printf("Open in %s Error\n", filename[is_out]);
                return -1;
            }

            if (is_out != -1)
                if(dup2(fd_out, STDOUT_FILENO) == -1) {
                    printf("Redirect Standard Out Error\n");
                    exit(1);
                }

            if (is_in != -1)
               if(dup2(fd_in, STDIN_FILENO) == -1) {
                   printf("Redirect Standard Out Error\n");
                   exit(1);
               }
            
        execv(buffer,argv);
    } else
        if (is_back == 0)
            waitpid(pid,&status,0);
    
    for (i=0;i<k;i++)
            free(argv[i]);

    if (is_in != -1) {
        free(filename[is_in]);
        close(fd_in);
    }
    
    if (is_out != -1) {
        free(filename[is_out]);
        close(fd_out);
    }

    return 0;
}

int copy_file(char* src, char* dest) {
    char ch;
    FILE* pfr = fopen(src, "r");
    FILE* pfw = fopen(dest, "w");
    
    if (NULL == pfw) {
        perror("open file test2.txt");
    }
     
    if (NULL == pfr) {
        perror("open file test1.txt");
    }
    
    while ((ch = fgetc(pfr)) != EOF) {
        fputc(ch, pfw);
    }
    
    fclose(pfr);
    fclose(pfw);
    pfr = NULL;
    pfw = NULL;
    return 0;
}

int main() {
    char *path, *arg[10], *input;
    int  li_inputlen, is_bj, is_back, i, j, k, pid, status;
    char lc_char;

    while (1) {
        is_bj = 0;
        is_back = 0;

        path = get_current_dir_name();
        printf("%s>$", path);

        li_inputlen = 0;
        lc_char = getchar();
        while (lc_char != '\n') {
            if (li_inputlen < BUFFERSIZE)
                buffer[li_inputlen++] = lc_char;
            lc_char = getchar();
        }
        
        if (li_inputlen >= BUFFERSIZE){
            printf("Your command is too long! Please re-enter your command!\n");
            li_inputlen = 0;
            continue;
        } else
            buffer[li_inputlen] = '\0';
        
        input = (char *) malloc(sizeof(char) * (li_inputlen + 1));
        strcpy(input,buffer);
        
        for (i = 0, j = 0, k = 0; i <= li_inputlen; i++) {
            if (input[i] == '<' || input[i] == '>' || input[i] == '|') {
                if (input[i] == '|')
                    pipel(input, li_inputlen);
                else
                    redirect(input, li_inputlen);
                is_bj = 1;
                break;
            }
        
            if (input[i] == ' ' || input[i] == '\t' || input[i] == '\0') {
                if (j == 0)
                    continue;
                else {
                    buffer[j++] = '\0';
                    arg[k] = (char *) malloc(sizeof(char) * j);
                    strcpy(arg[k], buffer);
                    j = 0;
                    k++;
                }
            } else {
                if (input[i] == '&' && input[i + 1] == '\0') {
                    is_back = 1;
                    continue;
                }
                buffer[j++] = input[i];
            }
        }
        
        free(input);
        
        if (strcmp(arg[0], "end") == 0 ) {
            printf("结束并退出\n");
            break;
        }
        
        if (strcmp(arg[0], "cop") == 0) {
            copy_file(arg[1], arg[2]);
        }
        
        if (strcmp(arg[0], "era") == 0) {
            remove(arg[1]);
        }
        
        if (strcmp(arg[0], "dis") == 0) {
            printf("%s\n", (char*)arg[1]);
        }
     
        if (is_bj == 0) {
            arg[k] = (char *) 0;
            if (file_exsists(arg[0]) == -1) {
//                printf("This command is not found?!\n");
                for (i = 0; i < k; i++)
                    free(arg[i]);
                continue;
            }
            
            if ((pid = fork()) == 0)
                execv(buffer, arg);
            else
                if (is_back == 0)
                    waitpid(pid, &status, 0);
            for (i=0;i<k;i++)
                free(arg[i]);
        }
    }
}
