#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_CVS 100
#define MAX_LEN 100

typedef struct {
    char username[MAX_LEN];
    char password[MAX_LEN];
    char status[MAX_LEN];  
} User;

User users[MAX_USERS];
int userCount = 0;

void registerUser();
int loginUser();
void submitCV(int userIndex);
void reviewCVs();
void resubmitCV(int userIndex);
void appointmentStatus(int userIndex);
void loadUsers();
void saveUsers();

int main() {
    int choice, userIndex;
    
    loadUsers();

    while (1) {
        printf("\n=== Online CV Checking System ===\n");
        printf("1. Register\n2. Login\n3. Review CVs (Admin)\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 
        
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2: 
                userIndex = loginUser();
                if (userIndex != -1) {
                    int cvChoice;
                    while (1) {
                        printf("\n1. Submit CV\n2. Resubmit CV\n3. Check Appointment Status\n4. Logout\n");
                        printf("Enter choice: ");
                        scanf("%d", &cvChoice);
                        getchar();
                        
                        if (cvChoice == 1) submitCV(userIndex);
                        else if (cvChoice == 2) resubmitCV(userIndex);
                        else if (cvChoice == 3) appointmentStatus(userIndex);
                        else break;
                    }
                }
                break;
            case 3:
                reviewCVs();
                break;
            case 4:
                saveUsers();
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached!\n");
        return;
    }

    printf("\nEnter Username: ");
    fgets(users[userCount].username, MAX_LEN, stdin);
    strtok(users[userCount].username, "\n"); 

    printf("Enter Password: ");
    fgets(users[userCount].password, MAX_LEN, stdin);
    strtok(users[userCount].password, "\n");

    strcpy(users[userCount].status, "Pending"); 

    userCount++;
    printf("Registration Successful!\n");
}

int loginUser() {
    char username[MAX_LEN], password[MAX_LEN];
    
    printf("\nEnter Username: ");
    fgets(username, MAX_LEN, stdin);
    strtok(username, "\n");

    printf("Enter Password: ");
    fgets(password, MAX_LEN, stdin);
    strtok(password, "\n");
int i;
    for ( i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login Successful!\n");
            return i;
        }
    }
    printf("Invalid Credentials!\n");
    return -1;
}

void submitCV(int userIndex) {
    char filename[MAX_LEN];
    sprintf(filename, "%s_cv.txt", users[userIndex].username);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error saving CV!\n");
        return;
    }

    char name[MAX_LEN], email[MAX_LEN], skills[MAX_LEN];

    printf("Enter Full Name: ");
    fgets(name, MAX_LEN, stdin);
    strtok(name, "\n");

    printf("Enter Email: ");
    fgets(email, MAX_LEN, stdin);
    strtok(email, "\n");

    printf("Enter Skills: ");
    fgets(skills, MAX_LEN, stdin);
    strtok(skills, "\n");

    fprintf(file, "Name: %s\nEmail: %s\nSkills: %s\n", name, email, skills);
    fclose(file);

    strcpy(users[userIndex].status, "Pending");
    printf("CV Submitted Successfully!\n");
}

void reviewCVs() {
    char username[MAX_LEN];
    printf("\nEnter username to review: ");
    fgets(username, MAX_LEN, stdin);
    strtok(username, "\n");
int i;
    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("1. Approve CV\n2. Reject CV\nEnter choice: ");
            int choice;
            scanf("%d", &choice);
            getchar();

            if (choice == 1) {
                strcpy(users[i].status, "Approved");
                printf("CV Approved!\n");
            } else {
                strcpy(users[i].status, "Rejected");
                printf("CV Rejected! User can resubmit.\n");
            }
            return;
        }
    }
    printf("User not found!\n");
}
void resubmitCV(int userIndex) {
    if (strcmp(users[userIndex].status, "Rejected") != 0) {
        printf("CV not rejected, resubmission not required.\n");
        return;
    }
    submitCV(userIndex);
}

void appointmentStatus(int userIndex) {
    if (strcmp(users[userIndex].status, "Approved") == 0) {
        printf("Congratulations! You have an appointment.\n");
    } else if (strcmp(users[userIndex].status, "Rejected") == 0) {
        printf("Sorry, your CV was rejected. Please resubmit.\n");
    } else {
        printf("Your CV is still under review.\n");
    }
}

void saveUsers() {
    FILE *file = fopen("users.txt", "w");
    if (!file) return;
int i;
    for (i = 0; i < userCount; i++) {
        fprintf(file, "%s %s %s\n", users[i].username, users[i].password, users[i].status);
    }

    fclose(file);
}

void loadUsers() {
    FILE *file = fopen("users.txt", "r");
    if (!file) return;

    while (fscanf(file, "%s %s %s", users[userCount].username, users[userCount].password, users[userCount].status) != EOF) {
        userCount++;
    }

    fclose(file);
}