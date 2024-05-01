#ifndef COMPANIES_STRUCT_FILIN_H
#define COMPANIES_STRUCT_FILIN_H

typedef struct Department Department;

typedef struct Company Company;

typedef struct CompanyContainer{
    int size;
    Company *companies;
} CompanyContainer;

typedef struct DepartmentsContainer{
    int size;
    Department *departments;
} DepartmentsContainer;

struct Company {
    char *name;
    DepartmentsContainer *departments;
};

struct Department {
    char *name;
    int outcome;
};

#endif