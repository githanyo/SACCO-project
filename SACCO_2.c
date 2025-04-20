#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Farmer structure
typedef struct Farmer {
    int id;
    char name[50];
    float deposit;
    float loan;
    struct Farmer* next;
} Farmer;
// Hash table to store farmers
Farmer* hashTable[TABLE_SIZE];

// Hash function
int hash(int id) {
    return id % TABLE_SIZE;
}

// Create new farmer node
Farmer* createFarmer(int id, const char* name, float deposit) {
    Farmer* newFarmer = (Farmer*)malloc(sizeof(Farmer));
    newFarmer->id = id;
    strcpy(newFarmer->name, name);
    newFarmer->deposit = deposit;
    newFarmer->next = NULL;
    return newFarmer;
}

// Add farmer to hash table
void registerFarmer(int id, const char* name) {
    int index = hash(id);
    Farmer* current = hashTable[index];

    // Check if farmer already exists
    while (current != NULL) {
        if (current->id == id) {
            printf("Farmer with ID %d already exists.\n", id);
            return;
        }
        current = current->next;
    }

    Farmer* newFarmer = createFarmer(id, name, 0.0);
    newFarmer->next = hashTable[index];
    hashTable[index] = newFarmer;
    printf("Farmer %s registered successfully.\n", name);
}

// Deposit money
void deposit(int id, float amount) {
    int index = hash(id);
    Farmer* current = hashTable[index];

    while (current != NULL) {
        if (current->id == id) {
            current->deposit += amount;
            printf("Deposited %.2f to %s. New balance: %.2f\n", amount, current->name, current->deposit);
            return;
        }
        current = current->next;
    }

    printf("Farmer with ID %d not found.\n", id);
}
// Withdraw money
void withdraw(int id, float amount) {
    int index = hash(id);
    Farmer* current = hashTable[index];

    while (current != NULL) {
        if (current->id == id) {
            if (amount > current->deposit) {
                printf("Insufficient balance. Current deposit: %.2f\n", current->deposit);
            } else {
                current->deposit -= amount;
                printf("Withdrawn %.2f from %s. New balance: %.2f\n", amount, current->name, current->deposit);
            }
            return;
        }
        current = current->next;
    }

    printf("Farmer with ID %d not found.\n", id);
}

// Check balance
void checkBalance(int id) {
    int index = hash(id);
    Farmer* current = hashTable[index];

    while (current != NULL) {
        if (current->id == id) {
            printf("Farmer: %s, Balance: %.2f\n", current->name, current->deposit);
            return;
        }
        current = current->next;
    }

    printf("Farmer with ID %d not found.\n", id);
}
// Apply for a loan
void applyLoan(int id, float amount) {
    int index = hash(id);
    Farmer* current = hashTable[index];

    while (current != NULL) {
        if (current->id == id) {
            current->loan += amount;
            printf("%s has taken a loan of %.2f. Total Loan: %.2f\n", current->name, amount, current->loan);
            return;
        }
        current = current->next;
    }

    printf("Farmer not found.\n");
}

// Repay loan
void repayLoan(int id, float amount) {
    int index = hash(id);
    Farmer* current = hashTable[index];

    while (current != NULL) {
        if (current->id == id) {
            if (amount > current->loan) amount = current->loan;
            current->loan -= amount;
            printf("%s repaid %.2f. Remaining Loan: %.2f\n", current->name, amount, current->loan);
            return;
        }
        current = current->next;
    }

    printf("Farmer not found.\n");
}

// List all farmers
void listFarmers() {
    printf("\nAll Registered Farmers:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Farmer* current = hashTable[i];
        while (current != NULL) {
            printf("ID: %d | Name: %s | Balance: %.2f\n", current->id, current->name, current->deposit);
            current = current->next;
        }
    }
}
void saveToFile() {
    FILE* fp = fopen("farmers.txt", "w");
    if (!fp) {
        printf("Error saving to file.\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        Farmer* current = hashTable[i];
        while (current != NULL) {
            fprintf(fp, "%d,%s,%.2f,%.2f\n", current->id, current->name, current->deposit, current->loan);
            current = current->next;
        }
    }

    fclose(fp);
    printf("Data saved successfully.\n");
}

void loadFromFile() {
    FILE* fp = fopen("farmers.txt", "r");
    if (!fp) return;

    int id;
    char name[50];
    float deposit, loan;
    while (fscanf(fp, "%d,%49[^,],%f,%f\n", &id, name, &deposit, &loan) == 4) {
        Farmer* newFarmer = createFarmer(id, name, deposit);
        newFarmer->loan = loan;
        int index = hash(id);
        newFarmer->next = hashTable[index];
        hashTable[index] = newFarmer;
    }

    fclose(fp);
}
// Menu
void menu() {
    int choice, id;
    float amount;
    char name[50];

    while (1) {
        printf("\n--- SACCO Management System ---\n");
        printf("1. Register Farmer\n");
        printf("2. Deposit Money\n");
        printf("3. Check Balance\n");
        printf("4. List All Farmers\n");
        printf("5. Apply for Loan\n");
        printf("6. Repay Loan\n");
        printf("7. Withdraw Money\n");
        printf("8. Save & Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // clear buffer

        switch (choice) {
            case 1:
                printf("Enter Farmer ID: ");
                scanf("%d", &id);
                getchar();
                printf("Enter Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;  // remove newline
                registerFarmer(id, name);
                break;
            case 2:
                printf("Enter Farmer ID: ");
                scanf("%d", &id);
                printf("Enter Deposit Amount: ");
                scanf("%f", &amount);
                deposit(id, amount);
                break;
            case 3:
                printf("Enter Farmer ID: ");
                scanf("%d", &id);
                checkBalance(id);
                break;
            case 4:
                listFarmers();
                break;
            case 5:
                printf("Enter Farmer ID: ");
                scanf("%d", &id);
                printf("Enter Loan Amount: ");
                scanf("%f", &amount);
                applyLoan(id, amount);
                break;
            case 6:
                printf("Enter Farmer ID: ");
                scanf("%d", &id);
                printf("Enter Repayment Amount: ");
                scanf("%f", &amount);
                repayLoan(id, amount);
                break;
            case 7:
                printf("Enter Farmer ID: ");
                scanf("%d", &id);
                printf("Enter Withdraw Amount: ");
                scanf("%f", &amount);
                withdraw(id, amount);
                break;
            case 8:
                saveToFile();
                exit(0);
            default:
                printf("Invalid option.\n");
        }
    }
}


int main() {
    loadFromFile();
    menu();
    saveToFile();
    return 0;
}

