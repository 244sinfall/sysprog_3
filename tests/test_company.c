#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "company.h"

void test_company() {
    CompanyContainer* array = NULL;
    array = add_company(array, "Company 1");
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_STRING_EQUAL(get_company_name(get_company(array, 0)), "Company 1");
    array = add_company(array, "Company 2");
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_STRING_EQUAL(get_company_name(get_company(array, 1)), "Company 2");
    set_company_name(get_company(array, 0), "Company 3");
    CU_ASSERT_STRING_EQUAL(get_company_name(get_company(array, 0)), "Company 3");
    CU_ASSERT_EQUAL(get_companies_count(array), 2)
    remove_company(array, 0);
    CU_ASSERT_EQUAL(get_companies_count(array), 1)
    CU_ASSERT_STRING_EQUAL(get_company_name(get_company(array, 0)), "Company 2");
    free(array);
}

void test_department() {
    CompanyContainer* array = NULL;
    array = add_company(array, "Company 1");
    CU_ASSERT_EQUAL(get_company_departments(get_company(array, 0))->size, 0);
    add_department(get_company(array, 0), "Department 1", 500);
    add_department(get_company(array, 0), "Department 2", 500);
    add_department(get_company(array, 0), "Department 3", -500);
    CU_ASSERT_EQUAL(get_company_departments(get_company(array, 0))->size, 3);
    CU_ASSERT_EQUAL(get_company_outcome(get_company(array, 0)), 500);
    add_department(get_company(array, 0), "Department 4", -500);
    CU_ASSERT_EQUAL(get_company_outcome(get_company(array, 0)), 0);
    CU_ASSERT_STRING_EQUAL(get_department_name(get_department(get_company_departments(get_company(array, 0)), 0)), "Department 1");
    CU_ASSERT_EQUAL(set_department_name(get_department(get_company_departments(get_company(array, 0)), 0), "low money department"), 0);
    CU_ASSERT_STRING_EQUAL(get_department_name(get_department(get_company_departments(get_company(array, 0)), 0)), "low money department");
    CU_ASSERT_EQUAL(get_company_outcome(get_company(array, 0)), 0);
    remove_department(get_company(array, 0), 0);
    remove_department(get_company(array, 0), 1);
    remove_department(get_company(array, 0), 1);
    CU_ASSERT_EQUAL(get_company_outcome(get_company(array, 0)), 500);
    CU_ASSERT_EQUAL(get_company_departments(get_company(array, 0))->size, 1);
    free(array);
}


int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Company Tests", NULL, NULL);
    CU_add_test(suite, "test_company", test_company);
    CU_add_test(suite, "test_department_company", test_department);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}