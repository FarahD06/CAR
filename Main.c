#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CarNode {
    char model[50];
    int matricule;
    int available;
    int working;
    struct CarNode *next;
} CarNode;
typedef struct {
    int carId;
    char startDate[20];
    char endDate[20];
} RentHistory;

#define FILENAME_CARS "cars.txt"
#define FILENAME_HISTORY "history.txt"

CarNode *carList = NULL;
RentHistory history[100];
int historyCount = 0;

void loadCars();
void saveCarToFile(CarNode *car);
void loadHistory();
void saveCars();
void saveHistory();
void printMenu();
void addCar();
void deleteCar();
void editCarDescription();
void editCarCondition();
void viewAllCars();
void viewRentHistory();
void rentCar();
void showCarDescription();
void returnCar();


void crewMemberMenu();
void customerMenu();

int main() {
    loadCars();
    loadHistory();

    int userType;
    while (1) {
    printf("Are you a crew member or a customer?\n");
    printf("1: Crew Member\n");
    printf("2: Customer\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &userType);

    switch (userType) {
        case 1:
            crewMemberMenu();
            break;
        case 2:
            customerMenu();
            break;
        default:
            printf("Invalid choice. Please choose 1 or 2.\n");

            while (getchar() != '\n');
    }
}

}

void crewMemberMenu() {
    int choice;
    while (1) {
        printMenu();
        printf("Enter your choice (1-->8): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addCar();
                break;
            case 2:
                deleteCar();
                break;
            case 3:
                showCarDescription();
                break;
            case 4:
                editCarDescription();
                break;
            case 5:
                editCarCondition();
                break;
            case 6:
                viewAllCars();
                break;
            case 7:
                viewRentHistory();
                break;
            case 8:
                saveCars();
                saveHistory();
                break;
            default:
                printf("Invalid choice, please choose a number from 1 to 8.\n");
        }
    }
}


void customerMenu() {
    int choice;
    while (1) {
        printf("\nCustomer Menu:\n");
        printf("1: Rent a car\n");
        printf("2: Return a car\n");
        printf("3: View all cars\n");
        printf("4: Show car description\n");
        printf("5: Exit\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                rentCar();
                break;
            case 2:
                printf("Do you want to claim and return a car?\n");
                printf("1: Yes\n");
                printf("2: No\n");
                printf("Enter your choice (1 or 2): ");
                int claimChoice;
                scanf("%d", &claimChoice);

                if (claimChoice == 1) {
                    returnCar();
                } else if (claimChoice != 2) {
                    printf("Invalid choice. Please choose 1 or 2.\n");
                }
                break;
            case 3:
                viewAllCars();
                break;
            case 4:
                showCarDescription();
                break;
            case 5:
                saveCars();
                saveHistory();
                break;
            default:
                printf("Invalid choice. Please choose a number from 1 to 5.\n");
        }
    }
}




void saveCars() {
    FILE *file = fopen(FILENAME_CARS, "w");
    if (file == NULL) {
        printf("Error saving cars data.\n");
        return;
    }

    CarNode *current = carList;
    while (current != NULL) {
        fprintf(file, "%s %d %d %d\n", current->model, current->matricule, current->available, current->working);
        current = current->next;
    }

    fclose(file);
}

void saveCarToFile(CarNode *car) {
    FILE *file = fopen(FILENAME_CARS, "a");
    if (file == NULL) {
        printf("Error saving car data.\n");
        return;
    }

    fprintf(file, "%s %d %d %d\n", car->model, car->matricule, car->available, car->working);

    fclose(file);
}

void addCar() {
    CarNode *newCar = (CarNode *)malloc(sizeof(CarNode));
    if (newCar == NULL) {
        printf("Memory allocation error.\n");
        return;
    }

    printf("Enter car details:\n");
    printf("Model: ");
    scanf("%s", newCar->model);
    printf("Matricule: ");
    scanf("%d", &newCar->matricule);
    printf("Available (1 for yes, 0 for no): ");
    scanf("%d", &newCar->available);
    printf("Working (1 for yes, 0 for no): ");
    scanf("%d", &newCar->working);

    newCar->next = carList;
    carList = newCar;

    saveCarToFile(newCar);

    printf("Car added successfully!\n");
}



void loadCars() {
    FILE *file = fopen(FILENAME_CARS, "r");
    if (file == NULL) {
        return;
    }

    while (!feof(file)) {
        CarNode *newCar = (CarNode *)malloc(sizeof(CarNode));
        if (fscanf(file, "%s %d %d %d", newCar->model, &newCar->matricule,
                   &newCar->available, &newCar->working) == 4) {
            newCar->next = carList;
            carList = newCar;
        } else {
            free(newCar);
            break;
        }
    }

    fclose(file);
}
void loadHistory() {
    FILE *file = fopen(FILENAME_HISTORY, "r");
    if (file == NULL) {
        return;
    }

    while (fscanf(file, "%d %s %s", &history[historyCount].carId,
                  history[historyCount].startDate, history[historyCount].endDate) == 3) {
        historyCount++;
    }

    fclose(file);
}

void saveHistory() {
    FILE *file = fopen(FILENAME_HISTORY, "w");
    if (file == NULL) {
        printf("Error saving rent history data.\n");
        return;
    }

    for (int i = 0; i < historyCount; i++) {
        fprintf(file, "%d %s %s\n", history[i].carId, history[i].startDate, history[i].endDate);
    }

    fclose(file);
}
void printMenu() {
    printf("\nE-Cars Menu:\n");
    printf("1: Add a new car\n");
    printf("2: Delete a car\n");
    printf("3: Show car description\n");
    printf("4: Edit car description\n");
    printf("5; Edit car condition\n");
    printf("6: View all cars\n");
    printf("7: View rent history\n");
    printf("8: Exit\n");
}
void deleteCar() {
    int matricule;
    printf("Enter the matricule of the car to delete: ");
    scanf("%d", &matricule);

    CarNode *current = carList;
    CarNode *prev = NULL;

    while (current != NULL && current->matricule != matricule) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        if (prev == NULL) {
            carList = current->next;
        } else {
            prev->next = current->next;
        }
        saveCars();
        free(current);
        printf("Car deleted successfully!\n");
    } else {
        printf("Car not found.\n");
    }
}
void editCarDescription() {
    int matricule;
    printf("Enter the matricule of the car to edit: ");
    scanf("%d", &matricule);

    CarNode *current = carList;
    while (current != NULL && current->matricule != matricule) {
        current = current->next;
    }
    if (current != NULL) {
        printf("Current details:\n");
        printf("Model: %s\n", current->model);
        printf("Matricule: %d\n", current->matricule);
        printf("Available: %s\n", current->available ? "Yes" : "No");
        printf("Working: %s\n", current->working ? "Yes" : "No");

        printf("Enter new availability (1 for yes, 0 for no): ");
        scanf("%d", &current->available);
        saveCars();
        printf("Description updated successfully!\n");
    } else {
        printf("Car not found.\n");
    }
}
void editCarCondition() {
    int matricule;
    printf("Enter the matricule of the car to edit: ");
    scanf("%d", &matricule);
    CarNode *current = carList;
    while (current != NULL && current->matricule != matricule) {
        current = current->next;
    }
    if (current != NULL) {
        printf("Current details:\n");
        printf("Model: %s\n", current->model);
        printf("Matricule: %d\n", current->matricule);
        printf("Available: %s\n", current->available ? "Yes" : "No");
        printf("Working: %s\n", current->working ? "Yes" : "No");
        printf("Enter new condition (1 for working, 0 for not working): ");
        scanf("%d", &current->working);
        saveCars();
        printf("Condition updated successfully!\n");
    } else {
        printf("Car not found.\n");
    }
}
void viewAllCars() {
    CarNode *current = carList;
    printf("\nAll Cars:\n");
    while (current != NULL) {
        printf("Model: %s\n", current->model);
        printf("Matricule: %d\n", current->matricule);
        printf("Available: %s\n", current->available ? "Yes" : "No");
        printf("Working: %s\n", current->working ? "Yes" : "No");
        printf("-------------------------\n");
        current = current->next;
    }
}



void viewRentHistory() {
    printf("\nRent History for All Cars:\n");
    printf("Car ID     Start Date       End Date\n");
    for (int i = 0; i < historyCount; ++i) {
        printf("%d        %s        %s\n", history[i].carId, history[i].startDate, history[i].endDate);
    }
}

void rentCar() {
    int matricule;
    printf("Enter the matricule of the car to rent: ");
    scanf("%d", &matricule);
    CarNode *current = carList;
    while (current != NULL && current->matricule != matricule) {
        current = current->next;
    }

    if (current != NULL) {
        if (current->available && current->working) {
            printf("Enter start date (format: DD-MM-YYYY): ");
            scanf("%s", history[historyCount].startDate);
            printf("Enter end date (format: DD-MM-YYYY): ");
            scanf("%s", history[historyCount].endDate);
            history[historyCount].carId = current->matricule;
            historyCount++;
            current->available = 0;

            printf("Car rented successfully!\n");
        } else {
            printf("Car is not available for rent.\n");
        }
    } else {
        printf("Car not found.\n");
    }
}
void showCarDescription() {
    int matricule;
    printf("Enter the matricule of the car to show description: ");
    scanf("%d", &matricule);

    CarNode *current = carList;
    while (current != NULL && current->matricule != matricule) {
        current = current->next;
    }

    if (current != NULL) {
        printf("Car Description:\n");
        printf("Model: %s\n", current->model);
        printf("Matricule: %d\n", current->matricule);
        printf("Available: %s\n", current->available ? "Yes" : "No");
        printf("Working: %s\n", current->working ? "Yes" : "No");
    } else {
        printf("Car not found.\n");
    }
}
void returnCar() {
    int carId;
    printf("Enter the car ID to return: ");
    scanf("%d", &carId);

    CarNode *current = carList;
    while (current != NULL && current->matricule != carId) {
        current = current->next;
    }

    if (current != NULL) {
        if (!current->available) {
            current->available = 1;
            printf("Car returned successfully.\n");
            saveCars();
        } else {
            printf("Car is already marked as available.\n");
        }
    } else {
        printf("Car not found.\n");
    }
}
