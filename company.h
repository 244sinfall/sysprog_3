#ifndef COMPANIES_FILIN_H
#define COMPANIES_FILIN_H

typedef struct Department Department;

typedef struct Company Company;

typedef struct CompanyContainer{
    int size;
    Company* companies;
} CompanyContainer;

typedef struct DepartmentsContainer{
    int size;
    Department* departments;
} DepartmentsContainer;


struct Company {
    char* name;
    DepartmentsContainer* departments;
};

struct Department {
    char* name;
    int outcome;
};

//
// CONTAINERS
//

// \brief Создает многоэлементную структуру компаний
// \returns Ссылка на область памяти, где будет располагаться структура
CompanyContainer* init_companies();
// \brief Создает многоэлементную структуру подразделений
// \returns Ссылка на область памяти, где будет располагаться структура
DepartmentsContainer* init_departments();

// COMPANY

// \brief Добавляет компанию в многоэлементую структуру
// \param array Ссылка на многоэлементую структуру компаний (NULL, чтобы создать)
// \param name Имя компании
// \returns Ссылка на многоэлементую структуру компаний
CompanyContainer* add_company(CompanyContainer* array, char* name);

// \brief Удаляет компанию из многоэлементной структуры
// \param array Ссылка на многоэлементную структуру компаний
// \param index Индекс компании, которую нужно удалить
// \returns Ссылка на многоэлементную структуру компаний
CompanyContainer* remove_company(CompanyContainer* array, int index);
// \brief Получить размер массива компаний
// \param array Массив компаний
// \returns Размер массива компаний, -1 в случае ошибки
int get_companies_count(CompanyContainer* array);

// \brief Получает компанию по индексу
// \param array Ссылка на многоэлементную структуру компаний
// \param index Индекс компании
// \returns Компанию, NULL в случае, если она не найдена.
Company* get_company(CompanyContainer* array, int index);

// \brief Получает название компании
// \param company Ссылка на компанию
// \returns Имя, NULL в случае, если она не найдена.
char* get_company_name(Company* company);

// \brief Переименовывает компанию
// \param company Ссылка на компанию
// \param newName Новое название компании
// \returns 0 в случае успеха, -1 в случае неудачи
int set_company_name(Company* company, char* newName);

// \brief Получает подразделения компании
// \param company Ссылка на компанию
// \param index Индекс компании
// \returns Многоэлементная структура департаментов, NULL в случае ошибки, или если ее нет.
DepartmentsContainer* get_company_departments(Company* company);

// \brief Получить общий результат компании (+ или -)
// \param company Компания
// \returns Положительное или отрицательное число дохода
int get_company_outcome(Company* company);


// DEPARTMENTS

// \brief Добавляет подразделение в компанию
// \param company Компания
// \param name Имя подразделения
// \param outcome Прибыль или убыток подразделения
// \returns Индекс добавленного подразделения, -1 в случае неудачи
int add_department(Company* company, char* name, int outcome);


// \brief Удаляет подразделение из компании
// \param company Компания
// \param index Индекс подразделения
// \returns 0 в случае успеха, -1 в случае неудачи
// 
int remove_department(Company* company, int index);
// \brief Получить размер массива подразделений
// \param array Массив подразделений
// \returns Размер массива подразделений, -1 в случае ошибки
int get_departments_count(DepartmentsContainer* array);

// \brief Получает подразделение по индексу
// \param departments Ссылка на подразделения
// \param index Индекс подразделения
// \returns Подразделение, NULL в случае, если она не найдена.
Department* get_department(DepartmentsContainer* departments, int index);

// \brief Получает имя подразделение
// \param department Подразделение
// \returns Имя подразделения
// 
char* get_department_name(Department* department);

// \brief Получает прибыльность подразделения
// \param department Подразделение
// \returns прибыль или убыток
// 
int get_department_outcome(Department* department);


// \brief Переименовывает подразделение
// \param department Подразделение
// \param newName Новое название подразделения
// \returns 0 в случае успеха, -1 в случае неудачи
// 
int set_department_name(Department* department, char* newName);

// \brief Меняет прибыльность подразделения
// \param department Подразделение
// \param outcome Новое значение прибыльности (+ или - значение)
// \returns 0 в случае успеха, -1 в случае неудачи
// 
int set_department_outcome(Department* department, int outcome);


// PRINT


// \brief Печатает информацию о компаниях
// \param array Многоэлементная структура
// 
void print_all_companies(CompanyContainer* array);

// \brief Печатает информацию о всех подразделениях
// \param array Подразделения
// 
void print_all_departments(DepartmentsContainer* array);
// \brief Печатает информацию о компании
// \param company Компания
// 
void print_company(Company* company);

// \brief Печатает информацию о подразделении
// \param department Ссылка на подразделение
// 
void print_department(Department* department);


#endif