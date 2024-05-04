AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = main
main_SOURCES = main.c
main_LDADD = libcompany.a

check_PROGRAMS = runtests
runtests_SOURCES = tests/test_company.c
runtests_LDADD = libcompany.a

noinst_LIBRARIES = libcompany.a
libcompany_a_SOURCES = company.c company_file.c
