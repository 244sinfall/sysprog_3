#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "company.h"

//
// CONTAINERS
//

// \brief Создает многоэлементную структуру компаний
// \returns Ссылка на область памяти, где будет располагаться структура
CompanyContainer* init_companies(){
    CompanyContainer* container =  malloc(sizeof(CompanyContainer));
    container->size = 0;
    return container;
}
// \brief Создает многоэлементную структуру подразделений
// \returns Ссылка на область памяти, где будет располагаться структура
DepartmentsContainer* init_departments(){
    DepartmentsContainer* container =  malloc(sizeof(DepartmentsContainer));
    container->size = 0;
    return container;
}

// COMPANY

// \brief Добавляет компанию в многоэлементую структуру
// \param array Ссылка на многоэлементую структуру компаний (NULL, чтобы создать)
// \param name Имя компании
// \returns Ссылка на многоэлементую структуру компаний
CompanyContainer* add_company(CompanyContainer* array, char* name){
    if(array == NULL){
        array = init_companies();
    }
    if(array->size == 0){
        array->companies = malloc(sizeof(Company));
    } else if(array->size > 0){
        array->companies = realloc(array->companies, sizeof(Company) * (array->size + 1));
    }
    if(array->companies == NULL){
        return array;
    }
    Company* company = &array->companies[array->size];
    company->name = malloc(strlen(name) + 1);
    memcpy(company->name, name, strlen(name) + 1);
    company->departments = init_departments();
    if(errno == ENOMEM){
        return array;
    }
    array->size++;
    return array;
}

// \brief Удаляет компанию из многоэлементной структуры
// \param array Ссылка на многоэлементную структуру компаний
// \param index Индекс компании, которую нужно удалить
// \returns Ссылка на многоэлементную структуру компаний
CompanyContainer* remove_company(CompanyContainer* array, int index) {
    if (array == NULL || index < 0 || index >= array->size) {
        return array;
    }

    Company* companyToRemove = &array->companies[index];

    // Освобождаем память всех департаментов
    if(companyToRemove ->departments != NULL){
        for (int i = 0; i < companyToRemove->departments->size; i++) {
            free(companyToRemove->departments->departments[i].name);
        }
        // освобождаем память всего массива
        free(companyToRemove->departments->departments);
    }
    // Освобождаем память названия компании
    free(companyToRemove->name);
    // Если это - не последний элемент массива
    if(index != array->size - 1){
        memmove(&array->companies[index], &array->companies[index + 1], array->size - (index + 1));
    }

    array->size--;

    // Перевыделяем память
    if (array->size == 0) {
        // Удаляем первый элемент, то есть сам массив
        free(array->companies);
        return array;
    } else {
        array->companies = realloc(array->companies, sizeof(Company) * array->size);
        if (array->companies == NULL) {
            free(array);
            return NULL;
        }
    }

    return array;
}

// \brief Получить размер массива компаний
// \param array Массив компаний
// \returns Размер массива компаний, -1 в случае ошибки
int get_companies_count(CompanyContainer* array){
    if(array == NULL){
        return -1;
    }
    return array->size;
}

// \brief Получает компанию по индексу
// \param array Ссылка на многоэлементную структуру компаний
// \param index Индекс компании
// \returns Компанию, NULL в случае, если она не найдена.
Company* get_company(CompanyContainer* array, int index) {
    if(array == NULL){
        return NULL;
    }
    if(array->size <= index){
        return NULL;
    }
    return &array->companies[index];
}

// \brief Получает название компании
// \param company Ссылка на компанию
// \returns Имя, NULL в случае, если она не найдена.
char* get_company_name(Company* company) {
    if(company == NULL){
        return NULL;
    }
    return company->name;
}

// \brief Переименовывает компанию
// \param company Ссылка на компанию
// \param newName Новое название компании
// \returns 0 в случае успеха, -1 в случае неудачи
int set_company_name(Company* company, char* newName){
    if(company == NULL){
        return -1;
    }
    if(newName == NULL || strlen(newName) < 1){
        return -1;
    }
    company->name = realloc(company->name, strlen(newName) + 1);
    if(errno == ENOMEM){
        return -1;
    }
    memcpy(company->name, newName, strlen(newName) + 1);
    if(errno == ENOMEM){
        return -1;
    }
    return 0;
}

// \brief Получает подразделения компании
// \param company Ссылка на компанию
// \param index Индекс компании
// \returns Многоэлементная структура департаментов, NULL в случае ошибки, или если ее нет.
DepartmentsContainer* get_company_departments(Company* company) {
    if(company == NULL){
        return NULL;
    }
    return company->departments;
}

// \brief Получить общий результат компании (+ или -)
// \param company Компания
// \returns + или -, 0 в случае ошибки или равного соотношения департаментов
int get_company_outcome(Company* company) {
    if(company == NULL){
        return 0;
    }
    DepartmentsContainer* departmentsContainer = get_company_departments(company);
    if(departmentsContainer->size < 1){
        return 0;
    }
    int current = 0;
    for(int i = 0; i < departmentsContainer->size; i++){
        current += get_department_outcome(get_department(departmentsContainer, i));
    }
    return current;
}


// DEPARTMENTS

// \brief Добавляет подразделение в компанию
// \param company Компания
// \param name Имя подразделения
// \param outcome + или -: прибыльность компании
// \returns Индекс добавленного подразделения, -1 в случае неудачи
int add_department(Company* company, char* name, int outcome) {

    if(company->departments == NULL){
        company->departments = init_departments();
    }
    if(company->departments->size == 0){
        company->departments->departments = malloc(sizeof(Department));
    } else if(company->departments->size > 0){
        company->departments->departments = realloc(company->departments->departments, sizeof(Department) * (company->departments->size + 1));
    }
    if(company->departments == NULL){
        return -1;
    }
    Department* department = &company->departments->departments[company->departments->size];
    department->name = malloc(strlen(name) + 1);
    if(errno == ENOMEM){
        company->departments->departments = realloc(company->departments->departments, sizeof(Department) * company->departments->size);
        return -1;
    }
    memcpy(department->name, name, strlen(name) + 1);
    department->outcome = outcome;
    company->departments->size++;
    return company->departments->size - 1;
};


// \brief Удаляет подразделение из компании
// \param company Компания
// \param index Индекс подразделения
// \returns 0 в случае успеха, -1 в случае неудачи
// 
int remove_department(Company* company, int index) {
    if (company == NULL || index < 0 || index >= company->departments->size) {
        return -1;
    }

    DepartmentsContainer* container = company->departments;
    Department* departmentToRemove = &container->departments[index];

    // Освобождаем память названия подразделения
    free(departmentToRemove->name);
    // Если это - не последний элемент массива
    if(index != container->size - 1){
        memmove(&container->departments[index], &container->departments[index + 1], container->size - (index + 1));
    }

    container->size--;

    // Перевыделяем память
    if (container->size == 0) {
        // Удаляем первый элемент, то есть сам массив
        free(container->departments);
        return 0;
    } else {
        container->departments = realloc(container->departments, sizeof(Department) * container->size);
        if (container->departments == NULL) {
            free(container);
            return -1;
        }
    }

    return 0;
}


// \brief Получить размер массива подразделений
// \param array Массив подразделений
// \returns Размер массива подразделений, -1 в случае ошибки
int get_departments_count(DepartmentsContainer* array){
    if(array == NULL){
        return -1;
    }
    return array->size;
}

// \brief Получает подразделение по индексу
// \param departments Ссылка на подразделения
// \param index Индекс подразделения
// \returns Подразделение, NULL в случае, если она не найдена.
Department* get_department(DepartmentsContainer* departments, int index) {
    if(departments == NULL){
        return NULL;
    }
    if(departments->size < index){
        return NULL;
    }
    return &departments->departments[index];
}

// \brief Получает имя подразделение
// \param department Подразделение
// \returns Имя подразделения
// 
char* get_department_name(Department* department){
    if(department == NULL || department->name == NULL){
        return NULL;
    }
    return department->name;
}

// \brief Получает прибыльность подразделения
// \param department Подразделение
// \returns - или +, 0 в случае ошибки
// 
int get_department_outcome(Department* department){
    if(department == NULL){
        return 0;
    }
    return department->outcome;
}


// \brief Переименовывает подразделение
// \param department Подразделение
// \param newName Новое название подразделения
// \returns 0 в случае успеха, -1 в случае неудачи
// 
int set_department_name(Department* department, char* newName){
    if(department == NULL){
        return -1;
    }
    if(newName == NULL || strlen(newName) < 1){
        return -1;
    }
    department->name = realloc(department->name, strlen(newName) + 1);
    if(errno == ENOMEM){
        return -1;
    }
    memcpy(department->name, newName, strlen(newName) + 1);
    if(errno == ENOMEM){
        return -1;
    }
    return 0;
}

// \brief Меняет прибыльность подразделения
// \param department Подразделение
// \param outcome Новое значение прибыльности (+ или -)
// \returns 0 в случае успеха, -1 в случае неудачи
// 
int set_department_outcome(Department* department, int outcome){
    if(department == NULL){
        return -1;
    }
    department->outcome = outcome;
    return 0;
}


// PRINT


// \brief Печатает информацию о компаниях
// \param array Многоэлементная структура
// 
void print_all_companies(CompanyContainer* array) {
    printf("#\t\tName\t\tMargin\t\tDeps\n");
    if(array == NULL){
        return;
    }
    int count = array->size;
    if(count < 1){
        return;
    }
    for(int i = 0; i < count; i++){
        printf("%d\t\t%s\t\t%d\t\t%d\n", i, get_company_name(&array->companies[i]), get_company_outcome(&array->companies[i]), get_departments_count(get_company_departments(&array->companies[i])));
    }
}

// \brief Печатает информацию о всех подразделениях
// \param array Подразделения
// 
void print_all_departments(DepartmentsContainer* array) {
    printf("#\t\tНаименование\t\tПрибыльность\n");
    if(array == NULL){
        return;
    }
    if(array->size < 1){
        return;
    }
    for(int i = 0; i < array->size; i++){
        printf("%d\t\t%s\t\t%d\n", i, get_department_name(&array->departments[i]), get_department_outcome(&array->departments[i]));
    }
}

// \brief Печатает информацию о компании
// \param company Компания
// 
void print_company(Company* company) {
    if(company == NULL){
        return;
    }
    printf("%s\t\t%d\t\t%d\n", get_company_name(company), get_company_outcome(company), get_company_departments(company)->size);
}

// \brief Печатает информацию о подразделении
// \param department Ссылка на подразделение
// 
void print_department(Department* department){
    if(department == NULL){
        return;
    }
    printf("%s\t\t%d\n", get_department_name(department), get_department_outcome(department));
};
