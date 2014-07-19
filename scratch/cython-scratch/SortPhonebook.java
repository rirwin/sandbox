import java.io.*;
import java.nio.file.*;
import java.util.Scanner;



class SortPhonebook {

    public static void main(String []args) {

        // 10M / 32 supports up to ph:999-9999
        final int bv_size = 321500; 
        final int int_size = 32;
        int comp_int = 0; // 32-bit int in java

        int[] bit_vector;
        bit_vector = new int[bv_size];
        int bit, idx;
        int ph_num = 0; 

        Scanner scanner = null;
        Writer writer = null;

        try{
            scanner = new Scanner(new File("unsorted_phonebook.txt"));
            while(scanner.hasNextInt()){
                ph_num = scanner.nextInt();
                bit = ph_num % int_size;
                idx = ph_num / int_size;
                comp_int = 1;
                comp_int <<= bit;
                bit_vector[idx] |= comp_int;
            }
        } catch (IOException x) {
            System.err.println(x);
        } finally {
            if(scanner!=null)
                scanner.close();
        }
        
        try {
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream("sorted_phonebook_java.txt"), "utf-8"));
            for (idx = 0; idx < bv_size; idx++) {
                comp_int = 1;
                if (bit_vector[idx] != 0) {
                    for (bit = 0; bit < int_size; bit++) {
                        if ((comp_int & bit_vector[idx]) != 0) {
                            writer.write(Integer.toString(idx*int_size + bit) + "\n");
                        }
                        comp_int <<= 1;
                    }
                }
            }
        } catch (IOException x) {
            System.err.println(x);
        } finally {
            try {writer.close();} catch (Exception ex) {}
        }

    }
}
