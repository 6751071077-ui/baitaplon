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

void clearBuffer() {
    while (getchar() != '\n');
}

void initMenuData() {
    MenuItem m0 = {1, "Ca phe Den", 18000, 12}; menu[0] = m0;
    MenuItem m1 = {2, "Ca phe Sua", 20000, 25}; menu[1] = m1;
    MenuItem m2 = {3, "Bac Xiu", 25000, 18}; menu[2] = m2;
    MenuItem m3 = {4, "Ca phe Muoi", 25000, 30}; menu[3] = m3;
    MenuItem m4 = {5, "Dalgona Coffee", 29000, 8}; menu[4] = m4;
    MenuItem m5 = {6, "Tra Dao Cam Sa (Size S)", 33000, 35}; menu[5] = m5;
    MenuItem m6 = {7, "Tra Vai Lai (Size S)", 33000, 15}; menu[6] = m6;
    MenuItem m7 = {8, "Tra Xoai Chanh Day Tuoi", 33000, 14}; menu[7] = m7;
    MenuItem m8 = {9, "Tra Luu Do Thanh Long", 33000, 10}; menu[8] = m8;
    MenuItem m9 = {10, "Matcha Latte Kem Trung", 33000, 88}; menu[9] = m9;
    MenuItem m10 = {11, "TS Kem Trung Nuong", 33000, 42}; menu[10] = m10;
    MenuItem m11 = {12, "Olong Lai Sua Cheese Ball", 33000, 28}; menu[11] = m11;
    MenuItem m12 = {13, "Sua Tuoi Tran Chau Duong Den", 33000, 31}; menu[12] = m12;
    MenuItem m13 = {14, "Latte Khoai Mon Sua Dua", 29000, 19}; menu[13] = m13;
    MenuItem m14 = {15, "Matcha Da Xay (Size S)", 33000, 16}; menu[14] = m14;
    MenuItem m15 = {16, "Topping: Cheese Ball", 7000, 40}; menu[15] = m15;
    MenuItem m16 = {17, "Topping: Tran Chau Duong Den", 7000, 50}; menu[16] = m16;
    menuCount = 17;
}

void showMenu() {
    int i;
    printf("\n==================== THUC DON QUAN CA PHE ====================\n");
    printf("%-6s %-35s %-15s\n", "Ma", "Ten Do Uong / Topping", "Gia Tien (VND)");
    printf("--------------------------------------------------------------\n");
    for (i = 0; i < menuCount; i++) {
        printf("%-6d %-35s %-15.0f", menu[i].id, menu[i].name, menu[i].price);
        if (menu[i].id == 10) {
            printf(" [BEST SELLER]");
        }
        printf("\n");
    }
    printf("==============================================================\n");
}

void searchMenu() {
    char keyword[50];
    int found = 0, i;
    printf("\nNhap ten mon can tim: ");
    clearBuffer();
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    printf("\n--- KET QUA TIM KIEM ---\n");
    for (i = 0; i < menuCount; i++) {
        if (strstr(menu[i].name, keyword) != NULL) {
            printf("ID: %d | Ten: %s | Gia: %.0f VND\n", menu[i].id, menu[i].name, menu[i].price);
            found = 1;
        }
    }
    if (!found) {
        printf("Khong tim thay mon phu hop!\n");
    }
}

void showBestSeller() {
    int maxIdx = 0, i;
    for (i = 1; i < menuCount; i++) {
        if (menu[i].salesCount > menu[maxIdx].salesCount) {
            maxIdx = i;
        }
    }
    printf("\n================ MON BAN CHAY NHAT (BEST SELLER) ================\n");
    printf("Mon hot nhat: [%s]\n", menu[maxIdx].name);
    printf("Gia ban: %.0f VND | Da ban: %d ly\n", menu[maxIdx].price, menu[maxIdx].salesCount);
    printf("=================================================================\n");
}

int main() {
    int choice;
    initMenuData();

    do {
        printf("\n--- DEV 1: QUAN LY MENU & THONG KE ---\n");
        printf("1. Xem thuc don\n");
        printf("2. Tim kiem mon theo ten\n");
        printf("3. Xem mon ban chay nhat\n");
        printf("0. Thoat\n");
        printf("Chon chuc nang: ");
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: showMenu(); break;
            case 2: searchMenu(); break;
            case 3: showBestSeller(); break;
            case 0: printf("Thoat module Dev 1!\n"); break;
            default: printf("Lua chon khong hop le!\n");
        }
    } while (choice != 0);

    return 0;
}
