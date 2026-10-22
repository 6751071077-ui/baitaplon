#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_TABLES 20
#define MAX_ITEMS_PER_ORDER 30

typedef struct {
    int drinkId;
    char name[60];
    int quantity;
    double price;
    double subtotal;
} OrderItem;

typedef struct {
    char tableCode[10];
    int customerCount;
    OrderItem items[MAX_ITEMS_PER_ORDER];
    int itemCount;
    double totalAmount;
    int isActive;
} Table;

Table tables[MAX_TABLES];
double totalRevenue = 0.0;

void clearBuffer() {
    while (getchar() != '\n');
}

void initSampleBillingData() {
    int i;
    for (i = 0; i < MAX_TABLES; i++) {
        sprintf(tables[i].tableCode, "S%02d", i + 1);
        tables[i].customerCount = 0;
        tables[i].itemCount = 0;
        tables[i].totalAmount = 0.0;
        tables[i].isActive = 0;
    }

    // Gia lap Ban S01 dang co mon can thanh toan
    tables[0].isActive = 1;
    tables[0].customerCount = 2;
    strcpy(tables[0].items[0].name, "Ca phe Sua");
    tables[0].items[0].quantity = 2;
    tables[0].items[0].price = 20000;
    tables[0].items[0].subtotal = 40000;

    strcpy(tables[0].items[1].name, "Matcha Latte Kem Trung");
    tables[0].items[1].quantity = 1;
    tables[0].items[1].price = 33000;
    tables[0].items[1].subtotal = 33000;

    tables[0].itemCount = 2;
    tables[0].totalAmount = 73000;
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

void checkoutAndExportCSV() {
    char inputStr[20], timeStr[50];
    int idx, i;
    time_t now;
    struct tm *localTime;
    FILE *f;

    printf("\nNhap Ma Ban can thanh toan (VD: S01): ");
    scanf("%s", inputStr);

    idx = parseTableIndex(inputStr);
    if (idx == -1) {
        printf("Loi: Ma ban khong hop le!\n");
        return;
    }

    Table *t = &tables[idx];
    if (!t->isActive || t->itemCount == 0) {
        printf("Ban %s hien dang trong hoac chua co mon!\n", t->tableCode);
        return;
    }

    // Lay thoi gian hien tai tu thu vien <time.h>
    now = time(NULL);
    localTime = localtime(&now);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localTime);

    // In hoa don
    printf("\n=================== HOA DON THANH TOAN ===================\n");
    printf(" Ban: %-10s | Thoi gian: %s\n", t->tableCode, timeStr);
    printf("----------------------------------------------------------\n");
    printf("%-5s %-25s %-6s %-12s %-12s\n", "STT", "Ten Mon", "SL", "Don Gia", "Thanh Tien");
    printf("----------------------------------------------------------\n");
    
    for (i = 0; i < t->itemCount; i++) {
        printf("%-5d %-25s %-6d %-12.0f %-12.0f\n", 
               i + 1, 
               t->items[i].name, 
               t->items[i].quantity, 
               t->items[i].price, 
               t->items[i].subtotal);
    }
    
    printf("----------------------------------------------------------\n");
    printf(" TONG CONG BILL BAN %s : %.0f VND\n", t->tableCode, t->totalAmount);
    printf("==========================================================\n");

    // Xuat tep du lieu ra file CSV (xuat log)
    f = fopen("hoa_don_log.csv", "a");
    if (f != NULL) {
        fprintf(f, "%s,%s,%.0f\n", t->tableCode, timeStr, t->totalAmount);
        fclose(f);
        printf("-> Da ghi nhan du lieu hoa don vao file hoa_don_log.csv!\n");
    }

    totalRevenue += t->totalAmount;

    t->customerCount = 0;
    t->itemCount = 0;
    t->totalAmount = 0;
    t->isActive = 0;

    printf("-> Thanh toan thanh cong! Ban %s da duoc tra ve trang thai trong.\n", t->tableCode);
}

void showRevenue() {
    printf("\n================ DOANH THU CUA QUAN ================\n");
    printf("Tong doanh thu tich luy: %.0f VND\n", totalRevenue);
    printf("====================================================\n");
}

int main() {
    int choice;
    initSampleBillingData();

    do {
        printf("\n--- DEV 3: TINH TIEN, THOI GIAN & TEP DU LIEU ---\n");
        printf("1. Thanh toan hoa don & Xuat file CSV log\n");
        printf("2. Xem tong doanh thu tich luy\n");
        printf("0. Thoat\n");
        printf("Chon chuc nang: ");
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: checkoutAndExportCSV(); break;
            case 2: showRevenue(); break;
            case 0: printf("Thoat module Dev 3!\n"); break;
            default: printf("Lua chon khong hop le!\n");
        }
    } while (choice != 0);

    return 0;
}
