#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "company.h"

// Define constants for menu options
#define ADD_COMPANY_OPTION 1
#define REMOVE_COMPANY_OPTION 2
#define SET_COMPANY_NAME_OPTION 3
#define PRINT_COMPANIES_OPTION 4
#define ADD_DEPARTMENT_OPTION 5
#define REMOVE_DEPARTMENT_OPTION 6
#define SET_DEPARTMENT_NAME_OPTION 7
#define SET_DEPARTMENT_OUTCOME_OPTION 8
#define PRINT_COMPANY_DEPARTMENTS_OPTION 9
#define QUIT_OPTION 10

#define MAX_INPUT_LENGTH 100

// Function prototypes
void display_menu();
char* get_string();
int get_int();
int select_company_index();
void handle_user_input(int option);

CompanyContainer* companyContainer;


char* get_string() {
    printf("Enter a string with max length of %d: \n", MAX_INPUT_LENGTH);
    char input[MAX_INPUT_LENGTH];

    // Read input as a string
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        exit(EXIT_FAILURE);
    }

    // Remove trailing newline character
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    } else {
        // Clear input buffer if newline character is not included
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Input too long.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the string and copy the input
    char *string = malloc(strlen(input) + 1);
    if (string == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(string, input);

    return string;
}

int get_int() {
    char input[20]; // Assuming a maximum input length of 20 characters
    int num;

    // Read input as a string
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        exit(EXIT_FAILURE);
    }

    // Check if newline character is included in input
    if (input[strlen(input) - 1] != '\n') {
        // Clear input buffer if newline character is not included
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Input too long.\n");
        exit(EXIT_FAILURE);
    }

    // Convert string to integer
    if (sscanf(input, "%d", &num) != 1) {
        printf("Invalid input. Please enter an integer.\n");
        exit(EXIT_FAILURE);
    }

    return num;
}

int select_company_index(){
    int prevSize = get_companies_count(companyContainer);
    if(prevSize == 0){
        printf("No companies!\n");
        return -1;
    }
    print_all_companies(companyContainer);
    printf("Enter index:\n");
    return get_int();
}

int select_department_index(Company* company){
    DepartmentsContainer* departments = get_company_departments(company);
    int prevSize = get_departments_count(departments);
    if(prevSize == 0){
        printf("No departments!\n");
        return -1;
    }
    print_all_departments(departments);
    printf("Enter index:\n");
    return get_int();
}



int main() {
    companyContainer = init_companies();
    int option;

    do {
        display_menu();
        printf("Enter your choice: ");
        option = get_int();
        handle_user_input(option);
    } while (option != QUIT_OPTION);

    return 0;
}

void display_menu() {
    printf("\n--- Main Menu ---\n");
    printf("1. Add Company\n");
    printf("2. Remove Company\n");
    printf("3. Set Company Name\n");
    printf("4. Print All Companies\n");
    printf("5. Add Department To Company\n");
    printf("6. Remove Department From Company\n");
    printf("7. Set Company's Department Name\n");
    printf("8. Set Company's Department Outcome\n");
    printf("9. Print Company's Departments\n");
    printf("10. Exit\n");
}

void add_company_ui(){
    char* name = get_string();
    companyContainer = add_company(companyContainer, name);
    printf("Added!\n");
    free(name);
}

void remove_company_ui(){
    int index = select_company_index();
    int size = get_companies_count(companyContainer);
    if(index != -1){
        remove_company(companyContainer, index);
        if(get_companies_count(companyContainer) == size){
            printf("Not removed!\n");
        } else {
            printf("Removed!\n");
        }
    }
}

void set_company_name_ui(){
    int index = select_company_index();
    if(index == -1){
        return;
    }
    Company* company = get_company(companyContainer, index);
    if(company == NULL){
        printf("Not found!\n");
        return;
    }
    char* name = get_string();
    int success = set_company_name(company, name);
    if(success == 0){
        printf("Renamed!\n");
    } else {
        printf("Failed!\n");
    }
    free(name);
}

void add_department_ui(){
    int index = select_company_index();
    if(index == -1){
        return;
    }
    Company* company = get_company(companyContainer, index);
    if(company == NULL){
        printf("Not found!\n");
        return;
    }
    char* name = get_string();
    printf("Enter outcome: (neg or pos integer)\n");
    int outcome = get_int();
    int depIndex = add_department(company, name, outcome);
    if(depIndex != -1){
        printf("Created %d!\n", depIndex);
    } else {
        printf("Failed!\n");
    }
    free(name);
}

void remove_department_ui(){
    int index = select_company_index();
    if(index == -1){
        return;
    }
    Company* company = get_company(companyContainer, index);
    if(company == NULL){
        printf("Not found!\n");
        return;
    }
    int depIndex = select_department_index(company);
    if(depIndex == -1){
        return;
    }
    int success = remove_department(company, index);
    if(success == 0){
        printf("Removed!\n");
    } else {
        printf("Failed!\n");
    }
}

void set_department_name_ui(){
    int index = select_company_index();
    if(index == -1){
        return;
    }
    Company* company = get_company(companyContainer, index);
    if(company == NULL){
        printf("Not found!\n");
        return;
    }
    DepartmentsContainer* departments = get_company_departments(company);
    if(departments == NULL){
        printf("Not found!\n");
        return;
    }
    int depIndex = select_department_index(company);
    if(depIndex == -1){
        return;
    }
    Department* department = get_department(departments, index);
    if(department == NULL){
        printf("Not found!\n");
        return;
    }
    char* name = get_string();
    int success = set_department_name(department, name);
    if(success == 0){
        printf("Renamed!\n");
    } else {
        printf("Failed!\n");
    }
    free(name);
}

void print_company_departments_ui(){
    int index = select_company_index();
    if(index == -1){
        return;
    }
    Company* company = get_company(companyContainer, index);
    if(company == NULL){
        printf("Not found!\n");
        return;
    }
    DepartmentsContainer* departments = get_company_departments(company);
    if(departments == NULL){
        printf("Not found!\n");
        return;
    }
    print_all_departments(departments);
}

void set_department_outcome_ui(){
        int index = select_company_index();
    if(index == -1){
        return;
    }
    Company* company = get_company(companyContainer, index);
    if(company == NULL){
        printf("Not found!\n");
        return;
    }
    DepartmentsContainer* departments = get_company_departments(company);
    if(departments == NULL){
        printf("Not found!\n");
        return;
    }
    int depIndex = select_department_index(company);
    if(depIndex == -1){
        return;
    }
    Department* department = get_department(departments, index);
    if(department == NULL){
        printf("Not found!\n");
        return;
    }
    printf("Enter new outcome: \n");
    int outcome = get_int();
    int success = set_department_outcome(department, outcome);
    if(success == 0){
        printf("Edited!\n");
    } else {
        printf("Failed!\n");
    }
}

void handle_user_input(int option) {
    switch (option) {
        case ADD_COMPANY_OPTION:
            add_company_ui();
            break;
        case REMOVE_COMPANY_OPTION:
            remove_company_ui();
            break;
        case SET_COMPANY_NAME_OPTION:
            set_company_name_ui();
            break;

        case PRINT_COMPANIES_OPTION:
            print_all_companies(companyContainer);
            break;
        
        case ADD_DEPARTMENT_OPTION:
            add_department_ui();
            break;

        case REMOVE_DEPARTMENT_OPTION:
            remove_department_ui();
            break;

        case SET_DEPARTMENT_NAME_OPTION:
            set_department_name_ui();
            break;
        case SET_DEPARTMENT_OUTCOME_OPTION:
            set_department_outcome_ui();
            break;
        
        case PRINT_COMPANY_DEPARTMENTS_OPTION:
            print_company_departments_ui();
            break;

        case QUIT_OPTION:
            printf("Exiting program.\n");
            exit(0);
            break;
        default:
            printf("Invalid option. Please try again.\n");
    }
}
