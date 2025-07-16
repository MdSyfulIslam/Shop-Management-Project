

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 100
#define MAX_TRANSACTIONS 100
typedef struct {
    int code;
    char name[50];
    float price;
    int quantity;
    char expirationDate[20];
} Item;

typedef struct {
    int itemCode;
    int quantitySold;
    float totalPrice;
    char saleDate[20];
} Transaction;

Item inventory[MAX_ITEMS];
int itemCount = 0;

Transaction transactions[MAX_TRANSACTIONS];
int transactionCount = 0;

void addItem() {
    Item newItem;
    printf("Enter item code: ");
    scanf("%d", &newItem.code);
    printf("Enter item name: ");
    scanf("%s", newItem.name);
    printf("Enter item price: ");
    scanf("%f", &newItem.price);
    printf("Enter item quantity: ");
    scanf("%d", &newItem.quantity);
    printf("Enter item expiration date (dd/mm/yyyy): ");
    scanf("%s", newItem.expirationDate);

    inventory[itemCount++] = newItem;
    printf("Item added successfully!\n");
}

int searchByCode(int code) {
    int low = 0;
    int high = itemCount - 1;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (inventory[mid].code == code) {
            return mid;
        } else if (inventory[mid].code < code) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

void displayItem(int index) {
    Item item = inventory[index];
    printf("Item Code: %d\n", item.code);
    printf("Item Name: %s\n", item.name);
    printf("Item Price: %.2f\n", item.price);
    printf("Item Quantity: %d\n", item.quantity);
    printf("Item Expiration Date: %s\n", item.expirationDate);
}

void displayInventory() {
    printf("Inventory:\n");
    for (int i = 0; i < itemCount; i++) {
        displayItem(i);
        printf("---------------------\n");
    }
}

int isExpired(int index) {
    Item item = inventory[index];
    time_t t = time(NULL);
    struct tm* currentTime = localtime(&t);

    int currentDay = currentTime->tm_mday;
    int currentMonth = currentTime->tm_mon + 1;
    int currentYear = currentTime->tm_year + 1900;

    int itemDay, itemMonth, itemYear;
    sscanf(item.expirationDate, "%d/%d/%d", &itemDay, &itemMonth, &itemYear);

    if (itemYear < currentYear)
        return 1;

    if (itemYear == currentYear && itemMonth < currentMonth)
        return 1;

    if (itemYear == currentYear && itemMonth == currentMonth && itemDay < currentDay)
        return 1;
    return 0;
}

void recordSale() {
    int code, quantity;
    printf("Enter item code to sell: ");
    scanf("%d", &code);
    printf("Enter quantity to sell: ");
    scanf("%d", &quantity);

    int index = searchByCode(code);
    if (index != -1) {
        if (inventory[index].quantity >= quantity) {

            inventory[index].quantity -= quantity;

            Transaction newTransaction;
            newTransaction.itemCode = code;
            newTransaction.quantitySold = quantity;
            newTransaction.totalPrice = inventory[index].price * quantity;

            time_t t = time(NULL);
            struct tm* currentTime = localtime(&t);
            strftime(newTransaction.saleDate, sizeof(newTransaction.saleDate), "%d/%m/%Y", currentTime);

            transactions[transactionCount++] = newTransaction;

            printf("Sale recorded successfully!\n");
        } else {
            printf("Insufficient quantity available!\n");
        }
    } else {
        printf("Item not found!\n");
    }
}

void displayTransaction(int index) {
    Transaction transaction = transactions[index];
    printf("Item Code: %d\n", transaction.itemCode);
    printf("Quantity Sold: %d\n", transaction.quantitySold);
    printf("Total Price: %.2f\n", transaction.totalPrice);
    printf("Sale Date: %s\n", transaction.saleDate);
}

void displayTransactionHistory() {
    printf("Transaction History:\n");
    for (int i = 0; i < transactionCount; i++) {
        displayTransaction(i);
        printf("---------------------\n");
    }
}
int main() {
    int choice;

    while (1) {
        printf("Shop Management System\n");
        printf("1. Add Item\n");
        printf("2. Search Item by Code\n");
        printf("3. Display Inventory\n");
        printf("4. Record Sale\n");
        printf("5. Display Transaction History\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addItem();
                break;
            case 2:
                {
                    int code;
                    printf("Enter item code to search: ");
                    scanf("%d", &code);
                    int index = searchByCode(code);
                    if (index != -1) {
                        printf("Item found:\n");
                        displayItem(index);
                        if (isExpired(index)) {
                            printf("Item has expired!\n");
                        } else {
                            printf("Item is not expired.\n");
                        }
                    } else {
                        printf("Item not found!\n");
                    }
                }
                break;
            case 3:
                displayInventory();
                break;
            case 4:
                recordSale();
                break;
            case 5:
                displayTransactionHistory();
                break;
            case 6:
                printf("Exiting program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }

        while (getchar() != '\n');
    }
    return 0;
}
