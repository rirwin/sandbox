import numpy
def sort_phonebook():
    
    bv_size = 321500 # 10M / 32 supports 0 - 9999999 (or ph:999-9999)
    int_size = 32

    bit_vector = numpy.zeros(bv_size, dtype=numpy.uint32)

    for idx in xrange(bv_size):
        bit_vector[idx] = 0
    
    # reading from file
    with open('unsorted_phonebook.txt','r') as ifh:
         for line in ifh:
             ph_num = int(line)
             bit = ph_num % int_size
             idx = ph_num / int_size
             comp_int = numpy.uint32(1)
             comp_int <<= bit
             bit_vector[idx] |= comp_int

    with open('sorted_phonebook_python.txt','w') as ofh:
         for idx in xrange(bv_size):
             comp_int = numpy.uint32(1)
             if bit_vector[idx] > 0: # small optimization
                 for bit in xrange(int_size):
                     if comp_int & bit_vector[idx]:
                         ofh.write(str(idx*32 + bit) + '\n')
                     comp_int <<= 1

