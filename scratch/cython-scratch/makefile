
all: init_phonebook c_test_compile java_test_compile cython_test_compile run_tests

init_phonebook: create_phonebook_file.py
	python create_phonebook_file.py

c_test_compile: sort_phonebook.c
	gcc $< -o sort_phonebook_in_c

java_test_compile: SortPhonebook.java
	javac SortPhonebook.java

cython_test_compile: sort_phonebook_cython.pyx
	python setup_cython_phonebook_sort.py build_ext --inplace

run_tests:
	python run_all_tests.py
	diff sorted_phonebook_c.txt sorted_phonebook_cython.txt
	diff sorted_phonebook_c.txt sorted_phonebook_python.txt
	diff sorted_phonebook_c.txt sorted_phonebook_java.txt
