#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "company.h"
#include "company_file.h"

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

#define INMEMORY_MODE 1
#define FILE_MODE 2

#define MAX_INPUT_LENGTH 100

// Function prototypes
void display_menu();
char* get_string();
int get_int();
int select_company_index();
void handle_user_input(int option);

CompanyContainer* companyContainer;
CompaniesMeta* fCompanyContainer;
char* filename;
int mode;


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
    int prevSize = mode == INMEMORY_MODE ? get_companies_count(companyContainer) : fget_companies_count(fCompanyContainer);
    if(prevSize == 0){
        printf("No companies!\n");
        return -1;
    }
    mode == INMEMORY_MODE ? print_all_companies(companyContainer) : fprint_all_companies(fCompanyContainer);
    printf("Enter index:\n");
    int index =  get_int();
    int count = mode == INMEMORY_MODE ? get_companies_count(companyContainer) : fget_companies_count(fCompanyContainer);
    if(index >= count){
        printf("Out of boundary!\n");
        return -1;
    }
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

int fselect_department_index(int companyIndex){
    int prevSize = fget_departments_count(fCompanyContainer, companyIndex);
    if(prevSize == 0){
        printf("No departments!\n");
        return -1;
    }
    fprint_all_departments(fCompanyContainer, companyIndex);
    printf("Enter index:\n");
    int index =  get_int();
    if(index >= prevSize){
        printf("Out of boundary!\n");
        return -1;
    }
}

CompaniesMeta* resolve_file(char *path) {
    CompaniesMeta* meta = open_file(path);
    if(meta == NULL){
        printf("You are setting up a file settings.\n");
        printf("Set max company name length. Minimum is 1:\n");
        int max_company_name = get_int();
        if(max_company_name < 1){
            return NULL;
        }
        printf("Set max department name length. Minimum is 1:\n");
        int max_department_name = get_int();
        if(max_department_name < 1){
            return NULL;
        }
        printf("Set max departments that company could have. Minimum is 1:\n");
        int max_company_departments = get_int();
        if(max_department_name < 1){
            return NULL;
        }
        meta = create_file(max_company_name, max_department_name, max_company_departments, path);
    }
    return meta;
}

int main(int argc, char* argv[]) {
    if(argc == 2){
        fCompanyContainer = resolve_file(argv[1]);
        if(fCompanyContainer == NULL) {
            printf("File could not be resolved");
            return 1;
        }
        mode = FILE_MODE;
        close_file(fCompanyContainer);
        filename = argv[1];
    } else {
        companyContainer = init_companies();
        mode = INMEMORY_MODE;
    }

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
    if(mode == INMEMORY_MODE){
        companyContainer = add_company(companyContainer, name);

    } else {
        fCompanyContainer = open_file(filename);
        fadd_company(fCompanyContainer, name);
        close_file(fCompanyContainer);
    }
    printf("Added!\n");
    free(name);
}

void remove_company_ui(){
    if(mode == INMEMORY_MODE){
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
    } else {
        fCompanyContainer = open_file(filename);
        int index = select_company_index();
        int size = fget_companies_count(fCompanyContainer);
        if(index != -1){
            fremove_company(fCompanyContainer, index);
            if(fget_companies_count(fCompanyContainer) == size){
                printf("Not removed!\n");
            } else {
                printf("Removed!\n");
            }
        }
        close_file(fCompanyContainer);
    }

}

void set_company_name_ui(){
    char* name = get_string();
    if(mode == INMEMORY_MODE){
        int index = select_company_index();
        if(index == -1){
            return;
        }
        Company* company = get_company(companyContainer, index);
        if(company == NULL){
            printf("Not found!\n");
            free(name);
            return;
        }

        int success = set_company_name(company, name);
        if(success == 0){
            printf("Renamed!\n");
        } else {
            printf("Failed!\n");
        }
    } else {
        fCompanyContainer = open_file(filename);
        int index = select_company_index();
        if(index == -1){
            return;
        }
        int success = fset_company_name(fCompanyContainer, index, name);
        if(success == 0){
            printf("Renamed!\n");
        } else {
            printf("Failed!\n");
        }
        close_file(fCompanyContainer);
    }

    free(name);
}

void add_department_ui(){
    char* name = get_string();
    printf("Enter outcome: (neg or pos integer)\n");
    int outcome = get_int();
    if(mode == INMEMORY_MODE){
        int index = select_company_index();
        if(index == -1){
            return;
        }
        Company* company = get_company(companyContainer, index);
        if(company == NULL){
            printf("Not found!\n");
            free(name);
            return;
        }

        int depIndex = add_department(company, name, outcome);
        if(depIndex != -1){
            printf("Created %d!\n", depIndex);
        } else {
            printf("Failed!\n");
        }
    } else {
        fCompanyContainer = open_file(filename);
        int index = select_company_index();
        if(index == -1){
            return;
        }
        int depIndex = fadd_department(fCompanyContainer, index, name, outcome);
        if(depIndex != -1){
            printf("Created %d!\n", depIndex);
        } else {
            printf("Failed!\n");
        }
        close_file(fCompanyContainer);
    }
    free(name);
}

void remove_department_ui(){

    if(mode == INMEMORY_MODE){
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
    } else {
        fCompanyContainer = open_file(filename);
        int index = select_company_index();
        if(index == -1){
            return;
        }
        int depIndex = fselect_department_index(index);
        if(depIndex == -1){
            return;
        }
        int success = fremove_department(fCompanyContainer, index, depIndex);
        if(success == 0){
            printf("Removed!\n");
        } else {
            printf("Failed!\n");
        }
        close_file(fCompanyContainer);
    }

}

void set_department_name_ui(){

    char* name = get_string();
    if(mode == INMEMORY_MODE){
        int index = select_company_index();
        if(index == -1){
            return;
        }
        Company* company = get_company(companyContainer, index);
        if(company == NULL){
            printf("Not found!\n");
            free(name);
            return;
        }
        DepartmentsContainer* departments = get_company_departments(company);
        if(departments == NULL){
            printf("Not found!\n");
            free(name);
            return;
        }
        int depIndex = select_department_index(company);
        if(depIndex == -1){
            free(name);
            return;
        }
        Department* department = get_department(departments, index);
        if(department == NULL){
            printf("Not found!\n");
            free(name);
            return;
        }
        int success = set_department_name(department, name);
        if(success == 0){
            printf("Renamed!\n");
        } else {
            printf("Failed!\n");
        }
    } else {
        fCompanyContainer = open_file(filename);
        int index = select_company_index();
        if(index == -1){
            return;
        }
        int depIndex = fselect_department_index(index);
        if(depIndex == -1){
            free(name);
            close_file(fCompanyContainer);
            return;
        }
        int success = fset_department_name(fCompanyContainer, index, depIndex, name);
        if(success == 0){
            printf("Renamed!\n");
        } else {
            printf("Failed!\n");
        }
        close_file(fCompanyContainer);
    }
    free(name);
}

void print_company_departments_ui(){

    if(mode == INMEMORY_MODE){
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
    } else {
        fCompanyContainer = open_file(filename);
        int index = select_company_index();
        if(index == -1){
            return;
        }
        fprint_all_departments(fCompanyContainer, index);
        close_file(fCompanyContainer);
    }
}

void set_department_outcome_ui(){
    if(mode == INMEMORY_MODE){
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
    } else {
        fCompanyContainer = open_file(filename);
        int index = select_company_index();
        if(index == -1){
            return;
        }
        int depIndex = fselect_department_index(index);
        if(depIndex == -1){
            close_file(fCompanyContainer);
            return;
        }
        printf("Enter new outcome: \n");
        int outcome = get_int();
        int success = fset_department_outcome(fCompanyContainer, index, depIndex, outcome);
        if(success == 0){
            printf("Edited!\n");
        } else {
            printf("Failed!\n");
        }
        close_file(fCompanyContainer);
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
            if(mode == INMEMORY_MODE){
                print_all_companies(companyContainer);
            } else {
                printf("FPRINT\n");
                fCompanyContainer = open_file(filename);
                fprint_all_companies(fCompanyContainer);
                close_file(fCompanyContainer);
            }
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
