package com.rirwin.structures;

public class TestIntHashTable {

    public static void main(String[] args) {
        
        int tableSize = 4093; // large prime number
        IntHashTable iht = new IntHashTable(tableSize);

        int key1 = 3,              value1 = 12;
        int key2 = 12,             value2 = 143;
        int key3 = tableSize+12,   value3 = 99;
        int key4 = 12,             value4 = 1;
        int key5 = tableSize+12,   value5 = 2;
        int key6 = 2*tableSize+12, value6 = 21;
            
        iht.put(key1,value1); 
        iht.put(key2,value2); 

        assert iht.hasKey(key1) == true;
        assert iht.hasKey(key2) == true;

        assert iht.get(key1) == value1;
        assert iht.get(key2) == value2;

        assert iht.getNumCollisions() == 0;

        iht.put(key3,value3); 
        iht.put(key4,value4);

        assert iht.get(key3) == value3;
        assert iht.get(key4) == value4;

        assert iht.getNumCollisions() == 1;

        iht.remove(key3);
        assert iht.hasKey(key3) == false;
        assert iht.get(key4) == value4;
        assert iht.getNumCollisions() == 0;

        iht.put(key5,value5); 
        iht.put(key6,value6);
        assert iht.getNumCollisions() == 2;

        assert iht.get(key4) == value4;
        assert iht.get(key5) == value5;
        assert iht.get(key6) == value6;

        iht.remove(key5);
        assert iht.get(key4) == value4;
        assert iht.hasKey(key5) == false;
        assert iht.get(key6) == value6;
        assert iht.getNumCollisions() == 1;

        iht.printTable();
    }
}
