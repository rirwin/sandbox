import sort_phonebook_cython
import sort_phonebook_python
import os
import time

lt = time.time()
os.system("./sort_phonebook_in_c")
ct = time.time()
print ct - lt, " seconds to sort in c"
lt = ct

sort_phonebook_cython.sort_phonebook()
ct = time.time()
print ct - lt, " seconds to sort in cython"
lt = ct

sort_phonebook_python.sort_phonebook()
ct = time.time()
print ct - lt, " seconds to sort in python"
lt = ct
