#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define INVALID -1
#define NULL 0

#define total_instruction 320
#define total_vp 32
#define clear_period 50

typedef struct {
    int pn, pfn, counter, time;
} pl_type;

pl_type pl[total_vp];

struct pfc_struct {
    int pn,pfn;
    struct pfc_struct *next;
};

struct pfc_struct pfc[total_vp], *freepf_head, *busypf_head, *busypf_tail;

int diseffect, a[total_instruction];
int page[total_instruction], offset[total_instruction];

void initialize(int total_pf) {
    int i;
    diseffect = 0;

    for(i = 0; i < total_vp; i += 1) {
        pl[i].pn = i;
        pl[i].pfn = INVALID;
        pl[i].counter = 0;
        pl[i].time = -1;
    }

    for(i = 1; i < total_pf; i += 1) {
        pfc[i-1].next = &pfc[i];
        pfc[i-1].pfn = i-1;
    }

    pfc[total_pf - 1].next = NULL;
    pfc[total_pf - 1].pfn = total_pf - 1;
    freepf_head = &pfc[0];
}

void LRU(int total_pf) {
    int min, minj, i, j, present_time;
        
    initialize(total_pf);
    present_time = 0;
    for (i = 0; i < total_instruction; i += 1) {
        if (pl[page[i]].pfn == INVALID) {
            diseffect += 1;
            if (freepf_head == NULL) {
                min = 32767;
                for (j = 0; j < total_vp; j += 1)
                    if (min > pl[j].time && (pl[j].pfn != INVALID)) {
                        min = pl[j].time;
                        minj = j;
                    }
                freepf_head = &pfc[pl[minj].pfn];
                pl[minj].pfn = INVALID;
                pl[minj].time = -1;
                freepf_head -> next = NULL;
            }
            
            pl[page[i]].pfn = freepf_head -> pfn;
            pl[page[i]].time = present_time;
            freepf_head = freepf_head -> next;
        } else
            pl[page[i]].time = present_time;
        present_time += 1;
    }

    printf("LRU:%6.4f ", 1 - (float)diseffect / 320);
}

void NRU(int total_pf)
{   }

void OPT(int total_pf)
{  }

void LFU(int total_pf)
{  }

void CLOCK(int total_pf)
{  }

void SCR(int total_pf)
{  }

void FIFO(int total_pf) {
    int i;
    struct pfc_struct *p;
    initialize(total_pf);
    busypf_head = busypf_tail = NULL;

    for (i = 0; i<total_instruction; i += 1) {
        if (pl[page[i]].pfn == INVALID) {
            diseffect += 1;
            if(freepf_head == NULL) {
                p = busypf_head->next;
                pl[busypf_head->pn].pfn = INVALID;
                freepf_head = busypf_head;
                freepf_head->next = NULL;
                busypf_head = p;
            }
            p = freepf_head -> next;
            freepf_head -> next = NULL;
            freepf_head -> pn = page[i];
            pl[page[i]].pfn = freepf_head -> pfn;
            if(busypf_tail == NULL)
                busypf_head = busypf_tail = freepf_head;
            else {
                busypf_tail -> next = freepf_head;
                busypf_tail = freepf_head;
            }
            freepf_head = p;
        }
    }

    printf("FIFO: %6.4f  ", 1 - (float)diseffect / 320);
}


int main() {
    int S, i;
    srand(10 * getpid());
    S = (int)(320.0 * rand() / RAND_MAX) + 1;
    for (i = 0; i < total_instruction; i += 4) {
        a[i] = S;
        a[i + 1] = a[i] + 1;
        a[i + 2] = (int)(1.0 * a[i] * rand() / RAND_MAX);
        a[i + 3] = a[i + 2] + 1;
        S = (int)(1.0 * rand() * (318 - a[i + 2]) / RAND_MAX) + a[i + 2] + 2;
    }

    for (i = 0; i<total_instruction; i += 1) {
        page[i] = a[i] / 10;
        offset[i] = a[i] % 10;
    }

    for (i = 4; i<= 32; i += 1) {
        printf("%2d page frames\t", i);
        FIFO(i);
        LRU(i);
        NRU (i);
        OPT(i);
        SCR(i);
        CLOCK(i);
    }

    return 0;
}


