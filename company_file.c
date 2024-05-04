#define _XOPEN_SOURCE 700

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "company_struct.h"
#include "company_file.h"
#include "company.h"


size_t size_of_company(CompaniesMeta* file);
size_t size_of_department(CompaniesMeta* file);
// FILE OPERATIONS

CompaniesMeta* create_file(int max_company_name, int max_department_name, int max_company_departments, char* filename){
    // Неправильная структура
    if(max_company_departments < 1 || max_company_name < 1 || max_department_name < 1){

        return NULL;
    }
    // Создаем мету
    CompaniesMeta* meta = malloc(sizeof(CompaniesMeta));

    meta->max_company_departments = max_company_departments;
    meta->max_company_name = max_company_name;
    meta->max_department_name = max_department_name;
    meta->file = open(filename, O_RDWR | O_TRUNC | O_CREAT, 0666);
    if(meta->file < 1){
        return NULL;
    }
    // Пишем мету в файл
    write(meta->file, &meta->max_company_departments, sizeof(int));
    write(meta->file, &meta->max_company_name, sizeof(int));
    write(meta->file, &meta->max_department_name, sizeof(int));
    
    return meta;
}

CompaniesMeta* open_file(char *path) {
    // Создаем мету
    CompaniesMeta* meta = malloc(sizeof(CompaniesMeta));
    if (access(path, F_OK) == 0) {
        meta->file = open(path, O_RDWR);
        if(meta->file < 1){
            free(meta);
            return NULL;
        }
        // Записываем мету
        ssize_t bytes_read = read(meta->file, &meta->max_company_departments, sizeof(int));
        if(bytes_read != sizeof(int)){
            free(meta);
            return NULL;
        }
        bytes_read = read(meta->file, &meta->max_company_name, sizeof(int));
        if(bytes_read != sizeof(int)){
            free(meta);
            return NULL;
        }
        bytes_read = read(meta->file, &meta->max_department_name, sizeof(int));
        if(bytes_read != sizeof(int)){
            free(meta);
            return NULL;
        }
        return meta;
    }
    return NULL;
}

void close_file(CompaniesMeta* file) {
    if(file->file < 1){
        return;
    }
    close(file->file);
}
// \brief Пропускает мета информацию, встает на первую компанию
off_t move_to_data(CompaniesMeta* file){
    return lseek(file->file, sizeof(int)*4, SEEK_SET);
}
// \brief Встает на компанию по индексу
off_t move_to_company(CompaniesMeta* file, int index){
    move_to_data(file);
    return lseek(file->file, index * size_of_company(file), SEEK_CUR);
}
// \brief Встает на департамент в компании
off_t move_to_department(CompaniesMeta* file, int companyIndex, int depIndex){
    move_to_company(file, companyIndex);
    lseek(file->file, file->max_company_name, SEEK_CUR);
    return lseek(file->file, depIndex * size_of_department(file), SEEK_CUR);
}
// \brief Размер структуры компании целиком
size_t size_of_company(CompaniesMeta* file) {
    return file->max_company_name + (file->max_company_departments * (file->max_department_name + sizeof(int)));
}
// \brief Размер структуры департамента
size_t size_of_department(CompaniesMeta* file) {
    return file->max_department_name + sizeof(int);
}
// \brief Читает департамент из файла по индексу компании и индексу департамента
Department* read_department(CompaniesMeta *file, int companyIndex, int depIndex){
    if(depIndex >= file->max_company_departments) {
        return NULL;
    }
    move_to_department(file, companyIndex, depIndex);
    char* departmentName = malloc(file->max_department_name);
    size_t bytes_read = read(file->file, departmentName, file->max_department_name);
    if(bytes_read != file->max_department_name || feof(fdopen(file->file, "r")) || departmentName[0] == '\0'){
        free(departmentName);
        return NULL;
    }
    Department *department = malloc(sizeof(Department));
    read(file->file, &department->outcome, sizeof(int));
    department->name = departmentName;
    return department;
}
// \brief Читает компанию из файла по индексу
Company* read_company(CompaniesMeta *file, int index){
    move_to_company(file, index);
    char* companyName = malloc(file->max_company_name);
    size_t bytes_read = read(file->file, companyName, file->max_company_name);
    if(bytes_read != file->max_company_name || feof(fdopen(file->file, "r")) || companyName[0] == '\0'){
        free(companyName);
        return NULL;
    }
    Company* company = malloc(sizeof(Company));
    company->name = companyName;
    company->departments = malloc(sizeof(DepartmentsContainer));
    company->departments->departments = malloc(sizeof(Department) * file->max_company_departments);
    company->departments->size = 0;
    int depIndex = 0;
    Department *department = NULL;
    do {
        department = read_department(file, index, depIndex);
        if(department != NULL){
            company->departments->departments[depIndex] = *department;
            company->departments->size++;
        }
        depIndex++;

    } while (depIndex < file->max_company_departments);
    return company;
}
// \brief Записывает депортамент в текущее положение курсора
void write_department(CompaniesMeta *file, Department *department){
    // Проверяем обязательное
    if (department == NULL || department->name == NULL) {
        char *null_bytes = calloc(size_of_department(file), sizeof(char));
        write(file->file, null_bytes, size_of_department(file));
        free(null_bytes);
        return;
    }
    char departmentName[file->max_department_name];
    memcpy(departmentName, department->name, file->max_department_name);
    // Обрезаем строку или заполняем нулями
    size_t name_length = strlen(department->name);
    if (name_length > file->max_department_name) {
        departmentName[file->max_department_name] = '\0';
        name_length = file->max_department_name;
    }
    if (name_length < file->max_department_name) {
        memset(departmentName + name_length, '\0', file->max_department_name - name_length);
    }
    // Пишем название компании
    write(file->file, departmentName, file->max_department_name);
    // Пишем прибыль
    write(file->file, &department->outcome, sizeof(int));
}
// \brief Записывает компанию в текущее положение курсора
void write_company(CompaniesMeta *file, Company *company){
    // Проверяем обязательное
    if (company == NULL || company->name == NULL) {
        char *null_bytes = calloc(size_of_company(file), sizeof(char));
        write(file->file, null_bytes, size_of_company(file));
        free(null_bytes);
        return;
    }
    char companyName[file->max_company_name];
    memcpy(companyName, company->name, file->max_company_name);
    // Обрезаем строку или заполняем нулями
    size_t name_length = strlen(company->name);
    if (name_length > file->max_company_name) {
        companyName[file->max_company_name] = '\0';
        name_length = file->max_company_name;
    }

    if (name_length < file->max_company_name) {
        memset(companyName + name_length, '\0', file->max_company_name - name_length);
    }
    // Пишем название компании
    write(file->file, companyName, file->max_company_name);
    // Пишем нули вместо подразделений
    off_t current_pos = lseek(file->file, 0, SEEK_CUR);
    char *null_bytes = calloc(size_of_department(file) * file->max_company_departments, sizeof(char));
    write(file->file, null_bytes, size_of_department(file));
    free(null_bytes);
    lseek(file->file, current_pos, SEEK_SET);
    if(company->departments == NULL || company->departments->size == 0){
        return;
    }
    // Пишем подразделения, сейчас мы на месте первого подразделения
    for(int i = 0; i < company->departments->size; i++){
        write_department(file, &company->departments->departments[i]);
    }
}

int fget_companies_count(CompaniesMeta* file) {
    int index = -1;
    do {
        char probe;
        move_to_company(file, index + 1);
        size_t bytes_read = read(file->file, &probe, 1);
        if(bytes_read < 1 || feof(fdopen(file->file, "r")) || probe == '\0'){
            break;
        }
        index ++;
    } while (1);
    return index + 1;
}

int fget_departments_count(CompaniesMeta* file, int companyIndex) {
    int index = -1;

    while(index < file->max_company_departments){
        char probe;
        move_to_department(file, companyIndex, index + 1);
        size_t bytes_read = read(file->file, &probe, 1);
        if(bytes_read < 1 || feof(fdopen(file->file, "r")) || probe == '\0'){
            break;
        }
        index++;
    }
    // На случай, если он уже прочитает следующую компанию
    if(index == file->max_company_departments){
        index--;
    }
    return index + 1;
}


void fadd_company(CompaniesMeta *file, char* name) {
    int count = fget_companies_count(file);
    Company company = {
        .name = name
    };
    move_to_company(file, count);
    write_company(file, &company);
}

void fremove_company(CompaniesMeta* file, int index) {
    int count = fget_companies_count(file);
    if (index < 0 || index >= count) {
        return; // Return if index is out of bounds
    }

    // If the company to remove is not the last one
    if (index != count - 1) {
        int to_move_index = index;
        do {
            // Read the next company until reaching the last index
            Company* company = read_company(file, to_move_index + 1);
            if (company == NULL) {
                break; // Stop if unable to read the next company
            }
            // Write the next company to the current position (overwriting the removed one)
            move_to_company(file, to_move_index);
            write_company(file, company);

            // Free the memory allocated for the company
            free(company->name);
            if (company->departments != NULL) {
                if (company->departments->departments != NULL) {
                    free(company->departments->departments);
                }
                free(company->departments);
            }
            free(company);

            to_move_index++;
        } while (to_move_index != count - 1);
    }

    // Truncate the file to remove the data of the last company
    ftruncate(file->file, (count - 1) * size_of_company(file));
}

Company* fget_company(CompaniesMeta* file, int index) {
    int count = fget_companies_count(file);
    if(index < 0 || index > count - 1){
        return NULL;
    }
    return read_company(file, index);
}

int fset_company_name(CompaniesMeta* file, int index, char* newName) {
    int count = fget_companies_count(file);
    if(index < 0 || index >= count){
        return -1; // Return an error code indicating index out of bounds
    }

    move_to_company(file, index);
    // Write the new company name
    size_t name_length = strlen(newName);
    char companyName[file->max_company_name];
    memcpy(companyName, newName, file->max_company_name);

    // Truncate the file to overwrite the old company name
    if (write(file->file, companyName, file->max_company_name) != file->max_company_name) {
        return -1; // Return error code if write operation fails
    }

    return 0; // Return 0 to indicate success
}


int fadd_department(CompaniesMeta* file, int index, char* name, int outcome) {
    if(index < 0 && index >= fget_companies_count(file)){
        return -1;
    }
    int count = fget_departments_count(file, index);

    if(count == file->max_company_departments){
        return -1;
    }
    move_to_department(file, index, count);
    size_t name_length = strlen(name);
    char departmentName[file->max_department_name];
    memcpy(departmentName, name, file->max_department_name);
    Department department = {
        .name = departmentName,
        .outcome = outcome
    };
    write_department(file, &department);
    return count;
}

int fremove_department(CompaniesMeta* file, int index, int depIndex) {
    int count = fget_companies_count(file);
    if (index < 0 || index >= count) {
        return -1; // Return if index is out of bounds
    }
    int depCount = fget_departments_count(file, index);
    if (depIndex < 0 || depIndex >= depCount) {
        return -1; // Return if index is out of bounds
    }
    // If the company to remove is not the last one

    int to_move_index = depIndex;
    while (to_move_index < depCount){
            // Read the next company until reaching the last index
        Department* department = read_department(file, index, to_move_index + 1);
        // Write the next company to the current position (overwriting the removed one)
        move_to_department(file, index, to_move_index);
        write_department(file, department);
        if(department != NULL){
            free(department->name);
            free(department);
        }
        // Free the memory allocated for the company
        to_move_index++;
    }
    return 0;
}

int fset_department_name(CompaniesMeta* file, int index, int depIndex, char* newName) {
    int count = fget_companies_count(file);
    if(index < 0 || index >= count){
        return -1; // Return an error code indicating index out of bounds
    }

    int depCount = fget_departments_count(file, index);
    if (depIndex < 0 || depIndex >= depCount) {
        return -1; // Return if index is out of bounds
    }

    move_to_department(file, index, depIndex);

    size_t name_length = strlen(newName);
    char departmentName[file->max_department_name];
    memcpy(departmentName, newName, file->max_department_name);
    if (name_length > file->max_department_name) {
        departmentName[file->max_department_name] = '\0';
        name_length = file->max_department_name;
    }
    if (name_length < file->max_department_name) {
        memset(departmentName + name_length, '\0', file->max_department_name - name_length);
    }


    // Truncate the file to overwrite the old company name
    if (write(file->file, departmentName, file->max_department_name) != file->max_department_name) {
        return -1; // Return error code if write operation fails
    }

    return 0; // Return 0 to indicate success
}

int fset_department_outcome(CompaniesMeta* file, int index, int depIndex, int outcome) {
    int count = fget_companies_count(file);
    if(index < 0 || index >= count){
        return -1; // Return an error code indicating index out of bounds
    }

    int depCount = fget_departments_count(file, index);

    if (depIndex < 0 || depIndex >= depCount) {
        return -1; // Return if index is out of bounds
    }

    move_to_department(file, index, depIndex);
    lseek(file->file, file->max_department_name, SEEK_CUR);
    // Truncate the file to overwrite the old company name
    if (write(file->file, &outcome, sizeof(int)) != sizeof(int)) {
        return -1; // Return error code if write operation fails
    }

    return 0; // Return 0 to indicate success
}

// \brief Печатает информацию о компаниях
// \param file Файл
// 
void fprint_all_companies(CompaniesMeta* file) {
    printf("#\t\tName\t\tMargin\t\tDeps\n");
    if(file == NULL){
        return;
    }
    int count = fget_companies_count(file);
    if(count < 1){
        return;
    }
    for(int i = 0; i < count; i++){
        Company* company = fget_company(file, i);
        if(company != NULL){
            printf("%d\t\t%s\t\t%d\t\t%d\n", i, get_company_name(company), get_company_outcome(company), get_departments_count(get_company_departments(company)));
        }
        if(company->name != NULL){
            free(company->name);
        }
        if(company->departments->departments != NULL){
            free(company->departments->departments);
        }
        free(company);
    }
}

// \brief Печатает информацию о компаниях
// \param file Файл
// \param Index Индекс компании
// 
void fprint_all_departments(CompaniesMeta* file, int index) {
    printf("#\t\tName\t\tMargin\n");
    if(file == NULL){
        return;
    }
    int count = fget_departments_count(file, index);
    if(count < 1){
        return;
    }
    for(int i = 0; i < count; i++){
        Department* department = read_department(file, index, i);
        if(department != NULL)
        {
            printf("%d\t\t%s\t\t%d\n", i, get_department_name(department), get_department_outcome(department));
        }
        if(department->name != NULL){
            free(department->name);
        }
        free(department);
    }
}