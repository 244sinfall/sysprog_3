#ifndef COMPANIES_FILE_FILIN_H
#define COMPANIES_FILE_FILIN_H 
#include <stdio.h>

#include "company_struct.h"

typedef struct CompaniesMeta {
    int max_company_name;
    int max_department_name;
    int max_company_departments;
    int file;
} CompaniesMeta;

CompaniesMeta* create_file(int max_company_name, int max_department_name, int max_company_departments, char *filename);

CompaniesMeta* open_file(char *path);

void close_file(CompaniesMeta* file);

// \brief Добавляет компанию в многоэлементую структуру
// \param file Метаданные файла
// \param name Имя компании
void fadd_company(CompaniesMeta* file, char* name);

// \brief Удаляет компанию из многоэлементной структуры
// \param file Метаданные файла
// \param index Индекс компании, которую нужно удалить
// \returns Ссылка на многоэлементную структуру компаний
void fremove_company(CompaniesMeta* file, int index);
// \brief Получить количество компаний в файле
// \param file Файл
// \returns Размер массива компаний
int fget_companies_count(CompaniesMeta* file);

// \brief Получить размер массива подразделений
// \param file Файл
// \returns Размер массива подразделений
int fget_departments_count(CompaniesMeta* file, int companyIndex);

// \brief Получает компанию по индексу
// \param file Файл
// \param index Индекс компании
// \returns Компанию, NULL в случае, если она не найдена.
Company* fget_company(CompaniesMeta* file, int index);

// \brief Переименовывает компанию
// \param file Файл
// \param index Индекс компании
// \param newName Новое название компании
// \returns 0 в случае успеха, -1 в случае неудачи
int fset_company_name(CompaniesMeta* file, int index, char* newName);

// DEPARTMENTS

// \brief Добавляет подразделение в компанию
// \param file Файл
// \param index Индекс компании
// \param name Имя подразделения
// \param outcome Прибыль или убыток подразделения
// \returns Индекс добавленного подразделения, -1 в случае неудачи
int fadd_department(CompaniesMeta* file, int index, char* name, int outcome);


// \brief Удаляет подразделение из компании
// \param file Файл
// \param index Индекс компании
// \param depIndex Индекс подразделения
// \returns 0 в случае успеха, -1 в случае неудачи
// 
int fremove_department(CompaniesMeta* file, int index, int depIndex);


// \brief Переименовывает подразделение
// \param file Файл
// \param index Индекс компании
// \param depIndex Индекс подразделения
// \param newName Новое название подразделения
// \returns 0 в случае успеха, -1 в случае неудачи
// 
int fset_department_name(CompaniesMeta* file, int index, int depIndex, char* newName);

// \brief Меняет прибыльность подразделения
// \param file Файл
// \param index Индекс компании
// \param depIndex Индекс подразделения
// \param outcome Новое значение прибыльности (+ или - значение)
// \returns 0 в случае успеха, -1 в случае неудачи
// 
int fset_department_outcome(CompaniesMeta* file, int index, int depIndex, int outcome);

// \brief Печатает информацию о компаниях
// \param file Файл
// 
void fprint_all_companies(CompaniesMeta* file);

// \brief Печатает информацию о компаниях
// \param file Файл
// \param Index Индекс компании
// 
void fprint_all_departments(CompaniesMeta* file, int index);

#endif