#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int DEBUG = 0;

void read_file_create_bit_vector(uint32_t* bvp, FILE* ifp);

int main() {

  FILE *ifp; // in file pointer
  FILE *ofp; // out file pointer
  char infile_name[] = "unsorted_phonebook.txt";
  char outfile_name[] = "sorted_phonebook.txt";
  
  int bv_size = 312500; // 10M / 32 1-000-0000 (big enough for 7 digits
  int *ph_num;
  int bit = 0;
  int idx = 0;
  uint32_t comp_int = 0; // int used for computation / setting bit

  // bit vector
  uint32_t *bvp = (uint32_t*)malloc(bv_size*sizeof(uint32_t));

  // clear bit vector
  for (idx = 0; idx < bv_size; idx++) {
    bvp[idx] = 0;
  }

  ifp = fopen(infile_name, "r");
  if (ifp == NULL) {
    fprintf(stderr, "Can't open input file in.list!\n");
    exit(1);
  }

  ofp = fopen(outfile_name, "w");
  if (ofp == NULL) {
    fprintf(stderr, "Can't open output file %s!\n",
            outfile_name);
    exit(1);
  }

  // ---- reads file and sets bits in bit vector ----

  while (fscanf(ifp, "%d", ph_num) != EOF) {

    idx = *ph_num/32;    // calc which int in bv to set
    bit = *ph_num % 32;    // calc which bit in int to set

    comp_int = 1;
    comp_int<<=bit;
    *(bvp + idx) = (*(bvp + idx) | comp_int); // set bit
  }

  // ---- outputs sorted numbers in file ----

  for (idx = 0; idx < bv_size; idx++) {

    comp_int = 1;
    for (bit = 0; bit < 32; bit++) {
      if (comp_int & *(bvp + idx))
        fprintf(ofp,"%d\n",idx*32+bit);
      comp_int<<=1;
    }
  }

  fclose(ifp);
  fclose(ofp);

  return 0;
}
