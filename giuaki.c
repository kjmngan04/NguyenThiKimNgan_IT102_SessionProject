#include <stdio.h>
#include <string.h>
#define MAX 100  // So luong vat tu toi da

#define BRIGHT_YELLOW "\x1b[93m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

// ================== CAU TRUC DU LIEU ===================
typedef struct {
    char matId[10];   // Ma vat tu
    char name[50];    // Ten vat tu
    char unit[10];    // Don vi tinh (kg, cai, hop...)
    int qty;          // So luong
    int status;       // Trang thai: 1=con su dung, 0=da het han  
} Material;

// ================= CAU TRUC XUAT/NHAP TU ===============
typedef struct {
    char transId[20];   // Ma giao dich (Sinh ngau nhien hoac tu tang)
    char matId[10];     // Ma vat tu
    char type[5];       // Kieu xuat/nhap (IN = nhap; OUT = xuat)
    char date[15];      // Thoi gian giao dich
    int amount; 
} Transaction; 

// ================== BIEN TOAN CUC ===================
// ================== KHOI TAO DU LIEU MAU ===================
// Muc dich: Tao 9 vat tu mau de test chuong trinh
    Material materials[9] = {             // luu y neu muon thay doi, ta xoa tu dau =
        {"001", "Kim loai", "CAI", 1, 1}, // den het roi thay so 9 thanh MAX
        {"002", "Inox", "HOP", 10, 0},    // o duoi thay so 9 thanh 0
        {"003", "Mang", "KG", 4, 1},
        {"004", "Nhua", "BAO", 13, 0},
        {"005", "Go", "BICH", 9, 1},
        {"006", "Aluminium", "TA", 1, 1},
        {"007", "Nhom", "YEN", 23, 1},
        {"008", "Xi mang", "TAN", 5, 0},
        {"009", "Inox 304", "MG", 3, 1},
    };
    
int materialQuantity = 9;    // So luong vat tu hien co

Transaction transList[1000]; // Mang chua so lan giao dich  
int transQuantity = 0;       // So luong giao dich hien co  
int autoTransId = 1; // dung de tu tang ID giao dich


// ================== HAM NHAP CHUOI AN TOAN ===================
// Muc dich: Nhap chuoi tu ban phim, khong cho phep de trong
// Tham so:
//   - str: con tro den chuoi can nhap
//   - size: kich thuoc toi da cua chuoi
//   - label: thong bao hien thi cho nguoi dung
void inputString(char *str, int size, char *label) {
    do {
        printf("%s", label);
        fgets(str, size, stdin);  // Doc chuoi tu ban phim
        str[strcspn(str, "\n")] = '\0';  // Xoa ky tu xuong dong

        if (strlen(str) == 0) {
            printf("Loi! Khong duoc de trong\n");
        } else {
            break;  // Nhap hop le, thoat vong lap
        }
    } while (1);
}

// ================== HAM NHAP SO AN TOAN ===================
// Muc dich: Nhap so nguyen tu ban phim, kiem tra tinh hop le
// Tham so:
//   - suggest: thong bao goi y nhap 
//   - min: gia tri nho nhat cho phep
//   - max: gia tri lon nhat cho phep
//   - number: con tro luu so vua nhap
//   - buffer[100]: bo dem tam de nhap chuoi truoc khi chuyen thanh so 
//   - valid: co kiem tra tinh hop le
void inputNumber(char *suggest, int min, int max, int *number) {
    char buffer[100];  // Bo dem de nhap chuoi
    int valid;

    while (1) {
        printf("%s: ", suggest);
        fgets(buffer, sizeof(buffer), stdin); // Dung de doc
        buffer[strcspn(buffer, "\n")] = '\0'; // Dung de loai bo \n

        // Kiem tra khong duoc de trong
        if (strlen(buffer) == 0) {
            printf(RED"Loi! Khong duoc de trong\n"RESET);
            continue;
        }

        // Kiem tra tat ca ky tu phai la so
        valid = 1;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] < '0' || buffer[i] > '9') {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf(RED"Loi! Vui long nhap so hop le\n"RESET);
            continue;
        }

        // Chuyen chuoi thanh so nguyen
        *number = 0;
        for (int i = 0; buffer[i] != '\0'; i++) {
            *number = (*number) * 10 + (buffer[i] - '0');
        }

        // Kiem tra so co nam trong khoang cho phep khong
        if (*number < min || *number > max) {
            printf("Loi! Vui long nhap trong khoang %d - %d\n", min, max);
            continue;
        }

        break;  // Nhap hop le, thoat vong lap
    }
}


// ================== TIM VAT TU THEO ID ===================
// Muc dich: Tim kiem vat tu trong mang theo ma ID
// Tra ve: vi tri cua vat tu trong mang, hoac -1 neu khong tim thay
int findMaterial(Material arr[], int n, char *id) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].matId, id) == 0)  // So sanh ID
            return i;
    }
    return -1;  // Khong tim thay
}

// ================== F01 - THEM VAT TU ===================
// Muc dich: Them mot hoac nhieu vat tu moi vao danh sach
// LOI DA SUA: Khi them nhieu vat tu ma co 1 ID trung, chi them duoc vat tu dau tien
void addMaterial(Material arr[], int *n) { // 
    int size;
    int maxMaterial = sizeof(materials)/sizeof(materials[0]);
    
    if(maxMaterial>materialQuantity) {
	inputNumber("Nhap so luong vat tu muon them", 1, 100, &size);
    for (int i = 0; i < size; i++) {
        printf("\n=== THEM VAT TU %d ===\n", i + 1);

        // Nhap ma vat tu
        inputString(arr[*n].matId, sizeof(arr[*n].matId), "Nhap matId: ");

        // Kiem tra ma da ton tai chua
        if (findMaterial(arr, *n, arr[*n].matId) != -1) {
            printf(RED"Loi! matId da ton tai! Bo qua vat tu nay.\n"RESET);
            continue;  // SUA LOI: Dung "continue" thay vi "return" de tiep tuc them cac vat tu con lai
        }

        // Nhap thong tin vat tu
        inputString(arr[*n].name, sizeof(arr[*n].name), "Nhap ten vat tu: ");
        inputString(arr[*n].unit, sizeof(arr[*n].unit), "Nhap don vi: ");
        inputNumber("Nhap so luong", 0, 10000, &arr[*n].qty);

        arr[*n].status = 1;  // Mac dinh: dang hoat dong
        (*n)++;  // Tang so luong vat tu

        printf("Them vat tu thanh cong!\n");
    }

    printf("\nHoan thanh qua trinh them vat tu!\n");
	}else{
		printf(RED"Danh sach vat tu da day"RESET);
	}
}

// ================== F02 - CAP NHAT VAT TU ===================
// Muc dich: Cap nhat thong tin vat tu da co trong danh sach
void updateMaterial(Material arr[], int n) {
    char id[20];
    printf("\n=== CAP NHAT VAT TU ===\n");

    // Nhap ma vat tu can cap nhat
    inputString(id, sizeof(id), "Nhap matId: ");

    // Tim vat tu trong danh sach
    int pos = findMaterial(arr, n, id);
    if (pos == -1) {
        printf("Khong tim thay vat tu!\n");
        return;
    }

    // Cap nhat thong tin moi
    inputString(arr[pos].name, sizeof(arr[pos].name), "Nhap ten moi: ");
    inputString(arr[pos].unit, sizeof(arr[pos].unit), "Nhap don vi moi: ");
    inputNumber("Nhap so luong moi", 0, 10000, &arr[pos].qty);

    printf("Cap nhat thanh cong!\n");
}

// ================== F03 - KHOA/XOA VAT TU ===================
// Muc dich: Khoa (soft delete) hoac xoa vinh vien (hard delete) vat tu
void lockOrDeleteMaterial(Material arr[], int *n) {
    char id[20];
    int choice;

    printf("\n=== KHOA / XOA VAT TU ===\n");

    // Nhap ma vat tu can xu ly
    inputString(id, sizeof(id), "Nhap matId: ");

    // Tim vat tu trong danh sach
    int pos = findMaterial(arr, *n, id);
    if (pos == -1) {
        printf("Khong tim thay vat tu!\n");
        return;
    }

    // Hien menu lua chon
    printf("\n-- Ban muon lam gi voi vat tu nay? --\n");
    printf("1. Khoa vat tu (Soft delete - status = 0)\n");
    printf("2. Xoa vat tu (Hard delete)\n");

    inputNumber("Chon chuc nang", 1, 2, &choice);

    if (choice == 1) {  // Khoa vat tu (soft delete)
        if (arr[pos].status == 0) {
            printf("Vat tu da bi khoa truoc do!\n");
        } else {
            arr[pos].status = 0;  // Dat trang thai thanh 0
            printf("Da khoa vat tu thanh cong! (status = 0)\n");
        }
    }
    else if (choice == 2) {  // Xoa vat tu (hard delete)
        // Dich chuyen cac phan tu phia sau len de xoa
        for (int i = pos; i < *n - 1; i++) {
            arr[i] = arr[i + 1];
        }
        (*n)--;  // Giam so luong vat tu

        printf("Da xoa vat tu thanh cong!\n");
    }
}

// ================== F04 - TRA CUU VAT TU ===================
// Muc dich: Tim kiem vat tu theo ID (chinh xac) hoac ten (gan dung)
void searchMaterial(Material arr[], int n) {
    int choice;
    char keyword[50];

    printf("\n=== TRA CUU VAT TU ===\n");
    printf("1. Tim theo ID (chinh xac)\n");
    printf("2. Tim theo ten (gan dung)\n");

    inputNumber("Chon chuc nang", 1, 2, &choice);

    if (choice == 1) {  // Tim theo ID
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
    else if (choice == 2) {  // Tim theo ten
        inputString(keyword, sizeof(keyword), "Nhap ten gan dung: ");

        int found = 0;
        printf("\n-- Ket qua tim kiem --\n");

        // Duyet qua tung vat tu de tim ten chua keyword
        for (int i = 0; i < n; i++) {
            if (strstr(arr[i].name, keyword) != NULL) {  // Neu ten chua keyword
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

// ================== F06 - SAP XEP THEO TEN ===================
// Muc dich: Sap xep danh sach vat tu theo ten (thu tu A-Z)
// Su dung thuat toan Bubble Sort
void bubbleSort(Material arr[], int n) {
    // Vong lap ngoai: chay n-1 lan
    for (int i = 0; i < n - 1; i++) {
        // Vong lap trong: so sanh va doi cho cac cap lien ke
        for (int j = 0; j < n - i - 1; j++) {
            // Neu ten o vi tri j lon hon ten o vi tri j+1 thi doi cho
            if (strcmp(arr[j].name, arr[j + 1].name) > 0) { // strcmp: so sanh 2 chuoi voi nhau
                Material temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    printf("\nSap xep thanh cong theo ten vat tu!\n");
}

// ================== F05 - HIEN THI DANH SACH ===================
// Muc dich: Hien thi danh sach vat tu theo trang (phan trang)
void displayList(Material arr[], int n) {
    printf("\n===== DANH SACH VAT TU =====\n");

    // Kiem tra danh sach co rong khong
    if (n == 0) {
        printf("Danh sach vat tu rong!\n");
        return;  // Thoat ham som
    }

    int itemPerPage = 5;  // So luong vat tu tren moi trang
    int totalPages = (n + itemPerPage - 1) / itemPerPage;  // Tinh tong so trang
    int currentPage = 1;  // Trang hien tai
    int page;

    while (1) {
        // Tinh chi so bat dau va ket thuc cua trang hien tai
        // startIndex: chi so bat dau
        // endIndex: chi so ket thuc 
        int startIndex = (currentPage - 1) * itemPerPage;
        int endIndex = startIndex + itemPerPage;
        if (endIndex > n) {
            endIndex = n;  // Dam bao khong vuot qua so luong vat tu
        }

        printf("\n--- Trang %d/%d ---\n", currentPage, totalPages);

        // In tieu de bang
        printf("%-10s %-30s %-10s %-10s %-15s\n", 
               "Ma VT", "Ten VT", "Don vi", "So luong", "Trang thai");
        printf("-----------------------------------------------------------------------------\n");

        // In cac vat tu trong trang hien tai
        for (int i = startIndex; i < endIndex; i++) {
            printf("%-10s %-30s %-10s %-10d %-15s\n",
                   arr[i].matId,
                   arr[i].name,
                   arr[i].unit,
                   arr[i].qty,
                   arr[i].status == 1 ? "Con su dung" : "Het han");
        }

        printf("\nTong so: %d vat tu\n", n);

        // Neu co nhieu hon 1 trang thi cho phep chuyen trang
        if (totalPages > 1) {
            inputNumber("\nNhap trang (1-%d) hoac 0 de thoat", 0, totalPages, &page);

            if (page == 0) {
                break;  // Thoat khoi che do xem danh sach
            }

            if (page >= 1 && page <= totalPages) {
                currentPage = page;  // Chuyen den trang nguoi dung chon
            } else {
                printf("Trang khong hop le!\n");
            }
        } else {
            break;  // Chi co 1 trang thi khong can phan trang
        }
    }
}

//================ HAM TAO MA GIAO DICH TU DONG ==================
void generateTransId(char *id) {
    sprintf(id, "T%04d", autoTransId++);   // VD: T0001, T0002
}

//================ HAM NHAP NGAY GIAO DICH =======================
void inputDate(char *date) {
    inputString(date, 15, "Nhap ngay (yyyy-mm-dd): ");
}

//=============== HAM GIAO DICH XUAT NHAP ========================
void transaction(Material arr[], int n) {
    char id[10];
    int choice, amount;

    printf("\n=== GIAO DICH VAT TU ===\n");

    inputString(id, sizeof(id), "Nhap matId: ");

    // Tim vat tu
    int pos = findMaterial(arr, n, id);
    if (pos == -1) {
        printf("Khong tim thay vat tu!\n");
        return;
    }

    // Kiem tra trang thai
    if (arr[pos].status == 0) {
        printf("Vat tu dang bi khoa! Khong the giao dich!\n");
        return;
    }

    printf("1. Nhap vat tu (IN)\n");
    printf("2. Xuat vat tu (OUT)\n");
    inputNumber("Chon loai giao dich", 1, 2, &choice);

    // Nhap so luong
    inputNumber("Nhap so luong", 1, 100000, &amount);

    if (choice == 1) {
        // --- GIAO DICH NHAP ---
        arr[pos].qty += amount;
        printf("Da nhap thanh cong! So luong moi: %d\n", arr[pos].qty);

        // Luu log
        generateTransId(transList[transQuantity].transId);
        strcpy(transList[transQuantity].matId, id);
        strcpy(transList[transQuantity].type, "IN");
        inputDate(transList[transQuantity].date);
        transList[transQuantity].amount = amount;
        transQuantity++;
    }
    else {
        // --- GIAO DICH XUAT ---
        if (amount > arr[pos].qty) {
            printf("Loi! So luong xuat vuot qua ton kho!\n");
            return;
        }

        arr[pos].qty -= amount;
        printf("Da xuat thanh cong! So luong moi: %d\n", arr[pos].qty);

        // Luu log
        generateTransId(transList[transQuantity].transId);
        strcpy(transList[transQuantity].matId, id);
        strcpy(transList[transQuantity].type, "OUT");
        inputDate(transList[transQuantity].date);
        transList[transQuantity].amount = amount;
        transQuantity++;
    }

    printf("Giao dich thanh cong!\n");
}

//============== HAM LICH SU GIAO DICH ==============
void historyTransaction() {
    char id[10];
    int found = 0;

    printf("\n=== LICH SU GIAO DICH ===\n");

    inputString(id, sizeof(id), "Nhap matId: ");

    printf("\n--- Danh sach giao dich cua vat tu %s ---\n", id);
    printf("%-10s %-10s %-10s %-15s %-10s\n",
           "TransID", "MatID", "Loai", "Ngay", "SoLuong");
    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < transQuantity; i++) {
        if (strcmp(transList[i].matId, id) == 0) {
            printf("%-10s %-10s %-10s %-15s %-10d\n",
                   transList[i].transId,
                   transList[i].matId,
                   transList[i].type,
                   transList[i].date,
                   transList[i].amount);
            found = 1;
        }
    }

    if (!found) {
        printf("Khong co giao dich nao cho vat tu nay!\n");
    }
}



// ================== HAM MAIN ===================
// Muc dich: Hien thi menu va xu ly lua chon cua nguoi dung
int main() {
    int choice;

    do {
        // Hien thi menu chinh
        
    
        printf(BRIGHT_YELLOW"\n<3*********************************************<3\n");
        printf("||============ QUAN LY VAT TU =================||\n");
        printf("<3*********************************************<3\n");
        printf("|+             1. Them vat tu                  +|\n");
        printf("|+             2. Cap nhat vat tu              +|\n");
        printf("|+             3. Quan ly vat tu               +|\n");
        printf("|+             4. Tra cuu vat tu               +|\n");
        printf("|+             5. Danh sach                    +|\n");
        printf("|+             6. Sap xep danh sach            +|\n");
        printf("|+             7. Giao dich                    +|\n");
        printf("|+             8. Lich su giao dich            +|\n");
        printf("|+             0. Thoat                        +|\n");
        printf("|***********************************************|\n"RESET);

        inputNumber("Vui long chon", 0, 8, &choice);

        // Xu ly lua chon cua nguoi dung
        switch (choice) {
            case 1: {  // Them vat tu
                addMaterial(materials, &materialQuantity);
                break;
            }
            case 2: {  // Cap nhat vat tu
                updateMaterial(materials, materialQuantity); 
                break;
            }
            case 3: {  // Khoa/xoa vat tu
                lockOrDeleteMaterial(materials, &materialQuantity);
                break;
            }
            case 4: {  // Tra cuu vat tu
                searchMaterial(materials, materialQuantity); 
                break;
            }
            case 5: {  // Hien thi danh sach
                displayList(materials, materialQuantity); 
                break;
            }
            case 6: {  // Sap xep danh sach
                bubbleSort(materials, materialQuantity);
                break;
            }
            case 7: {  // Giao dich nhap xuat
                transaction(materials, materialQuantity);
                break;
            }
            case 8: {  // Lich su giao dich
                historyTransaction();
                break;
            }
            case 0: {  // Thoat chuong trinh
                printf("Da thoat!\n");
                break;
            }
            default: {  // Lua chon khong hop le
                printf("Lua chon khong hop le!\n");
                break;
            }
        }

    } while (choice != 0);  // Lap lai cho den khi nguoi dung chon thoat

    return 0;
}
