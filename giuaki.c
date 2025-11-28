#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct {
    char matId[10];
    char name[50];
    char unit[10];
    int qty;
    int status;    
} Material;

// Khai báo mang và bien toàn cc
Material materials[MAX];
int materialQuantity = 0;

// ================== HAM NHAP CHUOI AN TOAN ===================
void inputString(char *str, int size, char *label) {
    do {
        printf("%s", label);
        fgets(str, size, stdin);
        str[strcspn(str, "\n")] = '\0';
        if (strlen(str) == 0) {
            printf("Loi! Khong duoc de trong\n");
        } else break;
    } while (1);
}

// ================== HAM NHAP SO AN TOAN ===================
void inputNumber(char *suggest, int min, int max, int *number) {
    char buffer[100];
    int valid;

    while (1) {
        printf("%s: ", suggest);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0) {
            printf("Loi! Khong duoc de trong\n");
            continue;
        }

        valid = 1;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] < '0' || buffer[i] > '9') {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("Loi! Vui long nhap so hop le\n");
            continue;
        }

        *number = 0;
        for (int i = 0; buffer[i] != '\0'; i++) {
            *number = (*number) * 10 + (buffer[i] - '0');
        }

        if (*number < min || *number > max) {
            printf("Loi! Vui long nhap trong khoang %d - %d\n", min, max);
            continue;
        }

        break;
    }
}

void initSampleData() {
	Material m[10] = {
	{"001","K","CAI",1,1},
	{"002","I","HOP",10,0},
	{"003","M","KG",4,1},
	{"004","N","BAO",13,0},
	{"005","G","BICH",9,2},
	{"006","A","TA",1,4},
	{"007","N","YEN",23,87},
	{"008","X","TAN",5,5},
	{"009","I","MG",3,5},
	};
	for(int i=0;i<9;i++){
		materials[materialQuantity++] = m[i];
	}
} 

// ================== TIM VAT TU ===================
int findMaterial(Material arr[], int n, char *id) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].matId, id) == 0)
            return i;
    }
    return -1;
}

// ================== F01 - THEM VAT TU ===================
void addMaterial(Material arr[], int *n) {
    int size;
    inputNumber("Nhap so luong vat tu muon them", 1, 100, &size);

    for (int i = 0; i < size; i++) {
        printf("\n=== THEM VAT TU %d ===\n", i + 1);

        inputString(arr[*n].matId, sizeof(arr[*n].matId), "Nhap matId: ");

        if (findMaterial(arr, *n, arr[*n].matId) != -1) {
            printf("Loi! matId da ton tai!\n");
            return;
        }

        inputString(arr[*n].name, sizeof(arr[*n].name), "Nhap ten vat tu: ");
        inputString(arr[*n].unit, sizeof(arr[*n].unit), "Nhap don vi: ");
        inputNumber("Nhap so luong", 0, 10000, &arr[*n].qty);

        arr[*n].status = 1;
        (*n)++;
    }

    printf("Them thanh cong!\n");
}

// ================== F02 - CAP NHAT ===================
void updateMaterial(Material arr[], int n) {
    char id[20];
    printf("\n=== CAP NHAT VAT TU ===\n");

    inputString(id, sizeof(id), "Nhap matId: ");

    int pos = findMaterial(arr, n, id);
    if (pos == -1) {
        printf("Khong tim thay vat tu!\n");
        return;
    }

    Material *mat = &arr[pos];

    inputString(mat->name, sizeof(mat->name), "Nhap ten moi: ");
    inputString(mat->unit, sizeof(mat->unit), "Nhap don vi moi: ");
    inputNumber("Nhap so luong moi", 0, 10000, &mat->qty);

    printf("Cap nhat thanh cong!\n");
}

// ================== F03 - KHOA/XOA VAT TU ===================
void lockOrDeleteMaterial(Material arr[], int *n) {
    char id[20];
    int choice;

    printf("\n=== KHOA / XOA VAT TU ===\n");

    inputString(id, sizeof(id), "Nhap matId: ");

    int pos = findMaterial(arr, *n, id);
    if (pos == -1) {
        printf("Khong tim thay vat tu!\n");
        return;
    }

    printf("\n-- Ban muon lam gi voi vat tu nay? --\n");
    printf("1. Khoa vat tu (Soft delete - status = 0)\n");
    printf("2. Xoa vat tu (Hard delete)\n");

    inputNumber("Chon chuc nang", 1, 2, &choice);

    if (choice == 1) {  
        if (arr[pos].status == 0) {
            printf("Vat tu da bi khoa truoc do!\n");
        } else {
            arr[pos].status = 0;
            printf("Da khoa vat tu thanh cong! (status = 0)\n");
        }
    }
    else if (choice == 2) {  
        for (int i = pos; i < *n - 1; i++) {
            arr[i] = arr[i + 1];
        }
        (*n)--;

        printf("Da xoa vat tu thanh cong!\n");
    }
}

// ================== F04 - TRA CUU VAT TU ===================
void searchMaterial(Material arr[], int n) {
    int choice;
    char keyword[50];

    printf("\n=== TRA CUU VAT TU ===\n");
    printf("1. Tim theo ID (chinh xac)\n");
    printf("2. Tim theo ten (gan dung)\n");

    inputNumber("Chon chuc nang", 1, 2, &choice);

    if (choice == 1) {  
        inputString(keyword, sizeof(keyword), "Nhap ID can tim: ");

        int pos = findMaterial(arr, n, keyword);
        if (pos == -1) {
            printf("Khong tim thay vat tu!\n");
        } else {
            printf("\n-- Ket qua --\n");
            printf("ID: %s | Name: %s | Unit: %s | Qty: %d | Status: %d\n",
                   arr[pos].matId, arr[pos].name, arr[pos].unit,
                   arr[pos].qty, arr[pos].status);
        }
    }
    else if (choice == 2) {  
        inputString(keyword, sizeof(keyword), "Nhap ten gan dung: ");

        int found = 0;
        printf("\n-- Ket qua tim kiem --\n");

        for (int i = 0; i < n; i++) {
            if (strstr(arr[i].name, keyword) != NULL) {
                printf("ID: %s | Name: %s | Unit: %s | Qty: %d | Status: %d\n",
                       arr[i].matId, arr[i].name, arr[i].unit,
                       arr[i].qty, arr[i].status);
                found = 1;
            }
        }

        if (!found)
            printf("Khong tim thay vat tu phu hop!\n");
    }
}

void bubbleSort(Material arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j].name, arr[j + 1].name) > 0) {
                Material temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    printf("\nSap xep thanh cong theo ten vat tu!\n");
}

// ================== HIEN THI DANH SACH ===================
void displayList(Material arr[], int n) {
    printf("\n===== DANH SACH VAT TU =====\n");

    if (n == 0) {
        printf("Danh sach vat tu rong!\n");
        return;
    }

    int itemPerPage = 5;
    int totalPages = (n + itemPerPage - 1) / itemPerPage;
    int currentPage = 1;
    int page;

    while (1) {
        int startIndex = (currentPage - 1) * itemPerPage;
        int endIndex = startIndex + itemPerPage;
        if (endIndex > n) {
            endIndex = n;
        }
        // startIndex = so trang dau
        // endIndex = so trang cuoi
        // itemPerPage = so 

        printf("\n--- Trang %d/%d ---\n", currentPage, totalPages);
        printf("%-10s %-30s %-10s %-10s %-15s\n", 
               "Ma VT", "Ten VT", "Don vi", "So luong", "Trang thai");
        printf("-----------------------------------------------------------------------------\n");

        for (int i = startIndex; i < endIndex; i++) {
            printf("%-10s %-30s %-10s %-10d %-15s\n",
                   arr[i].matId,
                   arr[i].name,
                   arr[i].unit,
                   arr[i].qty,
                   arr[i].status == 1 ? "Con su dung" : "Het han");
        }

        printf("\nTong so: %d vat tu\n", n);

        if (totalPages > 1) {
            inputNumber("\nNhap trang (1-%d) hoac 0 de thoat", 0, totalPages, &page);

            if (page == 0) {
                break;
            }

            if (page >= 1 && page <= totalPages) {
                currentPage = page;
            } else {
                printf("Trang khong hop le!\n");
            }
        } else {
            break;
        }
    }
}

// ================== MAIN ===================
int main() {
	initSampleData();
    int choice;

    do {
        printf("\n||================= MENU ================||\n");
        printf("||      1. Them vat tu       (F01)       ||\n");
        printf("||      2. Cap nhat vat tu   (F02)       ||\n");
        printf("||      3. Quan ly vat tu    (F03)       ||\n");
        printf("||      4. Tra cuu vat tu    (F04)       ||\n");
        printf("||      5. Danh sach         (F05)       ||\n");
        printf("||      6. Sap xep danh sach (F06)       ||\n");
        printf("||      7. Giao dich         (F07)       ||\n");
        printf("||      8. Lich su giao dich (F08)       ||\n");
        printf("||      0. Thoat                         ||\n");
        printf("||=======================================||\n");

        inputNumber("Vui long chon", 0, 5, &choice);

        switch (choice) {
            case 1: {
            	addMaterial(materials, &materialQuantity);
				break;
			}
            case 2: {
            	updateMaterial(materials, materialQuantity); 
				break;
			}
            case 3: {
            	lockOrDeleteMaterial(materials, &materialQuantity);
				break;
			}
            case 4: {
            	searchMaterial(materials, materialQuantity); 
				break;
			}
			case 5: {
            	displayList(materials, materialQuantity); 
				break;
			}
			case 6: {
				bubbleSort(materials, materialQuantity);
				break;
			}
			case 7: {
				
				break;
			}
            case 0: {
            	printf("Da thoat!\n");
				break;
			}
            default: {
            	printf("Lua chon khong hop le!\n");
            	break;
            }
        }

    } while (choice != 0);

    return 0;
}
