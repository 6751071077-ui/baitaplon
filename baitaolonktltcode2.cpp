#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MENU 50
#define MAX_TABLES 20
#define MAX_ITEMS_PER_ORDER 30

typedef struct {
    int id;
    char name[60];
    double price;
    int salesCount;
} MenuItem;

typedef struct {
    int drinkId;
    char name[60];
    int quantity;
    double price;
    double subtotal;
} OrderItem;

typedef struct {
    char tableCode[10];
    int tableIndex;
    int customerCount;
    OrderItem items[MAX_ITEMS_PER_ORDER];
    int itemCount;
    double totalAmount;
    int isActive;
} Table;

MenuItem menu[MAX_MENU];
int menuCount = 0;
Table tables[MAX_TABLES];

void clearBuffer() {
    while (getchar() != '\n');
}

void initTableData() {
    int i;
    MenuItem m0 = {1, "Ca phe Den", 18000, 12}; menu[0] = m0;
    MenuItem m1 = {2, "Ca phe Sua", 20000, 25}; menu[1] = m1;
    MenuItem m2 = {3, "Bac Xiu", 25000, 18}; menu[2] = m2;
    menuCount = 3;

    for (i = 0; i < MAX_TABLES; i++) {
        sprintf(tables[i].tableCode, "S%02d", i + 1);
        tables[i].tableIndex = i + 1;
        tables[i].customerCount = 0;
        tables[i].itemCount = 0;
        tables[i].totalAmount = 0.0;
        tables[i].isActive = 0;
    }
}

int parseTableIndex(const char *input) {
    int num = 0;
    if (toupper(input[0]) == 'S') {
        num = atoi(input + 1);
    } else {
        num = atoi(input);
    }
    if (num >= 1 && num <= MAX_TABLES) {
        return num - 1;
    }
    return -1;
}

void viewTableStatus() {
    int i;
    printf("\n================ TRANG THAI CAC BAN (S01 - S20) ================\n");
    printf("%-10s %-15s %-12s %-15s\n", "Ma Ban", "Trang Thai", "So Khach", "Tong Bill");
    printf("----------------------------------------------------------------\n");
    for (i = 0; i < MAX_TABLES; i++) {
        printf("Ban %-6s %-15s %-12d %-15.0f VND\n", 
               tables[i].tableCode, 
               tables[i].isActive ? "[Co Khach]" : "[Trong]", 
               tables[i].customerCount,
               tables[i].totalAmount);
    }
    printf("================================================================\n");
}

void orderDrink() {
    char inputStr[20];
    int idx, drinkId, qty, foundIdx, i;

    printf("\nNhap Ma Ban (S01 den S20 hoac 1-20): ");
    scanf("%s", inputStr);

    idx = parseTableIndex(inputStr);
    if (idx == -1) {
        printf("Loi: Ma ban khong hop le!\n");
        return;
    }

    Table *t = &tables[idx];

    if (!t->isActive) {
        printf("Ban %s dang trong. Nhap so luong khach: ", t->tableCode);
        scanf("%d", &t->customerCount);
        if (t->customerCount <= 0) t->customerCount = 1;
        t->isActive = 1;
    }

    while (1) {
        printf("\nNhap Ma mon (1-Ca phe Den, 2-Ca phe Sua, 3-Bac Xiu, nhap 0 de dung): ");
        scanf("%d", &drinkId);
        if (drinkId == 0) break;

        foundIdx = -1;
        for (i = 0; i < menuCount; i++) {
            if (menu[i].id == drinkId) {
                foundIdx = i;
                break;
            }
        }

        if (foundIdx == -1) {
            printf("Ma mon khong ton tai!\n");
            continue;
        }

        printf("Nhap so luong: ");
        scanf("%d", &qty);
        if (qty <= 0) continue;

        OrderItem *item = &t->items[t->itemCount];
        item->drinkId = menu[foundIdx].id;
        strcpy(item->name, menu[foundIdx].name);
        item->price = menu[foundIdx].price;
        item->quantity = qty;
        item->subtotal = qty * menu[foundIdx].price;

        t->totalAmount += item->subtotal;
        t->itemCount++;
        menu[foundIdx].salesCount += qty;

        printf("-> Da them %d x %s vao Ban %s (Thanh tien: %.0f VND)\n",
               qty, menu[foundIdx].name, t->tableCode, item->subtotal);
    }
}

void moveTable() {
    char strFrom[20], strTo[20];
    int idxFrom, idxTo, i;

    printf("\nNhap ma ban CHUYEN DI (VD: S01): ");
    scanf("%s", strFrom);
    printf("Nhap ma ban CHUYEN DEN (VD: S08): ");
    scanf("%s", strTo);

    idxFrom = parseTableIndex(strFrom);
    idxTo = parseTableIndex(strTo);

    if (idxFrom == -1 || idxTo == -1) {
        printf("Loi: Ma ban khong hop le!\n");
        return;
    }

    Table *src = &tables[idxFrom];
    Table *dst = &tables[idxTo];

    if (!src->isActive || dst->isActive) {
        printf("Loi: Trang thai ban khong hop le de chuyen!\n");
        return;
    }

    dst->customerCount = src->customerCount;
    dst->itemCount = src->itemCount;
    dst->totalAmount = src->totalAmount;
    dst->isActive = 1;

    for (i = 0; i < src->itemCount; i++) {
        dst->items[i] = src->items[i];
    }

    src->customerCount = 0;
    src->itemCount = 0;
    src->totalAmount = 0;
    src->isActive = 0;

    printf("-> Da chuyen toan bo tu ban %s sang ban %s!\n", src->tableCode, dst->tableCode);
}

void mergeTable() {
    char strFrom[20], strTo[20];
    int idxFrom, idxTo, i;

    printf("\nNhap ban CAN GOP (Ban A): ");
    scanf("%s", strFrom);
    printf("Nhap ban NHAN GOP (Ban B): ");
    scanf("%s", strTo);

    idxFrom = parseTableIndex(strFrom);
    idxTo = parseTableIndex(strTo);

    if (idxFrom == -1 || idxTo == -1 || idxFrom == idxTo) {
        printf("Loi: Ma ban khong hop le!\n");
        return;
    }

    Table *src = &tables[idxFrom];
    Table *dst = &tables[idxTo];

    if (!src->isActive || !dst->isActive) {
        printf("Loi: Ca 2 ban phai dang co khach moi gop duoc!\n");
        return;
    }

    dst->customerCount += src->customerCount;
    for (i = 0; i < src->itemCount; i++) {
        if (dst->itemCount < MAX_ITEMS_PER_ORDER) {
            dst->items[dst->itemCount++] = src->items[i];
        }
    }
    dst->totalAmount += src->totalAmount;

    src->customerCount = 0;
    src->itemCount = 0;
    src->totalAmount = 0;
    src->isActive = 0;

    printf("-> Da gop ban %s vao ban %s thanh cong!\n", src->tableCode, dst->tableCode);
}

int main() {
    int choice;
    initTableData();

    do {
        printf("\n--- DEV 2: QUAN LY BAN & SO DO BAN ---\n");
        printf("1. Xem trang thai cac ban\n");
        printf("2. Mo ban & Order do uong\n");
        printf("3. Chuyen ban\n");
        printf("4. Gop ban\n");
        printf("0. Thoat\n");
        printf("Chon chuc nang: ");
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: viewTableStatus(); break;
            case 2: orderDrink(); break;
            case 3: moveTable(); break;
            case 4: mergeTable(); break;
            case 0: printf("Thoat module Dev 2!\n"); break;
            default: printf("Lua chon khong hop le!\n");
        }
    } while (choice != 0);

    return 0;
}
