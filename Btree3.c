#include <stdio.h>


struct Btree3 {
    int keys[2];
    struct Btree3 *childs[3];
};

struct Btree3* create(int data) {
    struct Btree3* newB = (struct Btree3*)malloc(sizeof(struct Btree3));
    newB->keys[0] = data; newB->keys[1] = 0;
    newB->childs[0] = NULL; newB->childs[1] = NULL; newB->childs[2] = NULL;
    return newB;
}

int isLeaf(struct Btree3 *P) {
    return !P->childs[0];
}

int insertIn(struct Btree3 *P, int data) {
    if (!P->keys[1]) {
        // insertar ordenadamente
        if (P->keys[0] < data) {
            P->keys[1] = data;
        }
        else {
            P->keys[1] = P->keys[0];
            P->keys[0] = data;
        }
    }
    else { // Sube un dato
        int ax;
        
        if (P->keys[0] > data) {
            ax = P->keys[0];
            P->keys[0] = data;
        }
        else if (P->keys[1] > data) {
            return data;
        }
        else {
            ax = P->keys[1];
            P->keys[1] = data;
        }
        return ax;
    }

    return 0;
}

int hasMin(struct Btree3*P) {
    return P->keys[0] && !P->keys[1];
}

struct Btree3* splitL(struct Btree3 *P, int res) {
    struct Btree3 *newB = NULL; 
    newB = create(res);
    newB->childs[0] = P;
    newB->childs[1] = create(P->keys[1]);
    P->keys[1] = 0;

    return newB;
}

void rprHead(struct Btree3 **P, struct Btree3 *ax, int res, int i) {
    if (i == 2) { 
        ax->childs[2] = splitL(ax->childs[2], res);
        *P = create(ax->keys[1]);
        ax->keys[1] = 0;
        (*P)->childs[0] = ax;
        (*P)->childs[1] = ax->childs[2];
        ax->childs[2] = NULL;
    }
    else if (i == 0) { 
        ax->childs[0] = splitL(ax->childs[0], res);
        *P = create(ax->keys[0]);
        ax->keys[0] = ax->keys[1]; ax->keys[1] = 0;
        (*P)->childs[1] = ax;
        (*P)->childs[0] = ax->childs[0];
        ax->childs[0] = ax->childs[1];
        ax->childs[1] = ax->childs[2]; ax->childs[2] = NULL;
    }
    else { 
        ax->childs[1] = splitL(ax->childs[1], res);
        *P = ax->childs[1];
        ax->childs[1] = (*P)->childs[0];
        (*P)->childs[0] = ax;

        (*P)->childs[2] = create(ax->keys[1]);
        (*P)->childs[2]->childs[0] = (*P)->childs[1];
        (*P)->childs[1] = (*P)->childs[2]; (*P)->childs[2] = NULL;
        (*P)->childs[1]->childs[1] = ax->childs[2];

        ax->keys[1] = 0; ax->childs[2] = NULL;
    }
}

int insert( struct Btree3 **P, struct Btree3*ax, int data) {
    int res;
    if (!(*P)) {
        *P = create(data);
    }
    else {
        
        if (isLeaf(ax)) {
            res = insertIn(ax, data);
            if (res) {
                if (ax == *P) {
                    *P = splitL(ax, res);
                }
                else return res;
            }
        }
        else {
            int i;
            for (i = 0; i < 3; i++) {
                if (!ax->keys[i] || ax->keys[i] > data) break; 
            }
            res = insert(P, ax->childs[i], data);

            if (res) {
                if (hasMin(ax)) {
                    insertIn(ax, res);
                    if (i == 0) { 
                        ax->childs[2] = ax->childs[1];
                        ax->childs[1] = create(ax->childs[0]->keys[1]);
                        ax->childs[0]->keys[1] = 0;
                    }
                    else { 
                        ax->childs[2] = ax->childs[1];
                        ax->childs[1] = create(ax->childs[1]->keys[0]);
                        
                        ax->childs[2]->keys[0] = ax->childs[2]->keys[1];
                        ax->childs[2]->keys[1] = 0;
                    }
                }
                else {
                    if (ax == *P) {
                        rprHead(P,ax,res, i);
                    }//
                }
            }
        }
    }
    return 0;
}

void inorder(struct Btree3 *P) {
    if (P) {
        inorder(P->childs[0]);
        printf(" %d - ", P->keys[0]);
        inorder(P->childs[1]);
        if (P->keys[1]) printf(" %d - ", P->keys[1]);
        inorder(P->childs[2]);
    }
}

void freeM(struct Btree3*P) {
    if (P) {
        freeM(P->childs[0]);
        freeM(P->childs[1]);
        freeM(P->childs[2]);
        free(P);
    }
}

void pause() {
    printf("\n\t<Press ENTER>");
    getchar();
    getchar();
}

int main() {
    int op, x;
    struct Btree3* head = NULL;


    do {
        system("clear");

        printf("\n\n\n== MENU BTREE 3 ==\n");
        printf("1. Insert\n");
        printf("2. Show BTREE3\n");
        printf("0. EXIT\n");
        printf("Your option => ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("\nEnter the number: ");
                scanf("%d", &x);
                insert(&head, head, x);
                break;
            case 2:
                if (head) printf("\n\tHead: %d - %d \n", head->keys[0], head->keys[1]);
                inorder(head);
                printf("\n\n");
                break;
        }

        pause();

    } while (op != 0);
    
    freeM(head);
    printf("\n\nSuccesfully");
    return 0;
}