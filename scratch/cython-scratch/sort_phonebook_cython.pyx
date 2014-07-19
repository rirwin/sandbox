def sort_phonebook():

    # 10M / 32 supports 0 - 9999999 (or ph:999-9999)
    DEF bv_size = 321500 
    DEF int_size = 32

    cdef int bit
    cdef int idx
    cdef int ph_num
    cdef unsigned int comp_int
    cdef unsigned int bit_vector_i

    cdef unsigned int bit_vector[bv_size]

    for idx in xrange(bv_size):
        bit_vector[idx] = 0
    
    # reading from file
    with open('unsorted_phonebook.txt','r') as ifh:
         for line in ifh:
             ph_num = int(line)
             bit = ph_num % int_size
             idx = ph_num / int_size
             comp_int = 1
             comp_int <<= bit
             bit_vector[idx] |= comp_int

    with open('sorted_phonebook_cython.txt','w') as ofh:
         for idx in xrange(bv_size):
             comp_int = 1
             if bit_vector[idx] > 0: # small optimization
                 for bit in xrange(int_size):
                     if comp_int & bit_vector[idx]:
                         ofh.write(str(idx*int_size + bit) + '\n')
                     comp_int <<= 1    
