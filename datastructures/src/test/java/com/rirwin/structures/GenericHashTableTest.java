package com.rirwin.structures;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

public class GenericHashTableTest {
    
    @Test 
    public void testGenericHashTableAsDouble() {
        final int tableSize = 4093;
        final GenericHashTable<Double> dht = new GenericHashTable<>(tableSize);


        final Integer key1 = 3;
        final Integer key2 = 12;
        final Integer key3 = tableSize+12;
        final Integer key4 = 12;
        final Integer key5 = tableSize+12;
        final Integer key6 = 2*tableSize+12;
        
        final Double value1 = 12.;
        final Double value2 = 143.;
        final Double value3 = 99.;
        final Double value4 = 1.;
        final Double value5 = 2.;
        final Double value6 = 21.;

        dht.put(key1,value1);
        dht.put(key2,value2);

        assertTrue("Missing key", dht.hasKey(key1));
        assertTrue("Missing key", dht.hasKey(key2));

        assertEquals("Wrong number enqueued", value1, dht.get(key1));
        assertEquals("Wrong number enqueued", value2, dht.get(key2));

        assertEquals("No collisions expected", 0, dht.getNumCollisions());

        dht.put(key3,value3);
        dht.put(key4,value4);

        assertEquals("Wrong entry", value3, dht.get(key3));
        assertEquals("Wrong entry", value4, dht.get(key4));

        assertEquals("Collisions expected", 1, dht.getNumCollisions());

        dht.remove(key3);

        assertFalse("No key expected", dht.hasKey(key3));
        assertEquals("Wrong entry", value4, dht.get(key4));
        assertEquals("No collisions expected", 0, dht.getNumCollisions());

        dht.put(key5,value5);
        dht.put(key6,value6);

        assertEquals("Collisions expected", 2, dht.getNumCollisions());

        assertEquals("Wrong entry", value4, dht.get(key4));
        assertEquals("Wrong entry", value5, dht.get(key5));
        assertEquals("Wrong entry", value6, dht.get(key6));

        dht.remove(key5);
        assertEquals("Wrong entry", value4, dht.get(key4));
        assertFalse("Key not expected", dht.hasKey(key5));

        assertEquals("Wrong entry", value6, dht.get(key6));
        assertEquals("Collisions expected", 1, dht.getNumCollisions());

        dht.printTable();
    }
    
    @Test 
    public void testGenericHashTableAsString() {
        final int tableSize = 4093;
        final GenericHashTable<String> sht = new GenericHashTable<>(tableSize);


        final Integer key1 = 3;
        final Integer key2 = 12;
        final Integer key3 = tableSize+12;
        final Integer key4 = 12;
        final Integer key5 = tableSize+12;
        final Integer key6 = 2*tableSize+12;
        
        final String value1 = "12.";
        final String value2 = "143.";
        final String value3 = "99.";
        final String value4 = "1.";
        final String value5 = "2.";
        final String value6 = "21.";

        sht.put(key1,value1);
        sht.put(key2,value2);

        assertTrue("Missing key", sht.hasKey(key1));
        assertTrue("Missing key", sht.hasKey(key2));

        assertEquals("Wrong number enqueued", value1, sht.get(key1));
        assertEquals("Wrong number enqueued", value2, sht.get(key2));

        assertEquals("No collisions expected", 0, sht.getNumCollisions());

        sht.put(key3,value3);
        sht.put(key4,value4);

        assertEquals("Wrong entry", value3, sht.get(key3));
        assertEquals("Wrong entry", value4, sht.get(key4));

        assertEquals("Collisions expected", 1, sht.getNumCollisions());

        sht.remove(key3);

        assertFalse("No key expected", sht.hasKey(key3));
        assertEquals("Wrong entry", value4, sht.get(key4));
        assertEquals("No collisions expected", 0, sht.getNumCollisions());

        sht.put(key5,value5);
        sht.put(key6,value6);

        assertEquals("Collisions expected", 2, sht.getNumCollisions());

        assertEquals("Wrong entry", value4, sht.get(key4));
        assertEquals("Wrong entry", value5, sht.get(key5));
        assertEquals("Wrong entry", value6, sht.get(key6));

        sht.remove(key5);
        assertEquals("Wrong entry", value4, sht.get(key4));
        assertFalse("Key not expected", sht.hasKey(key5));

        assertEquals("Wrong entry", value6, sht.get(key6));
        assertEquals("Collisions expected", 1, sht.getNumCollisions());

        sht.printTable();
    }
    

    @Test
    public void testGenericHashTableAsInteger() {

        final int tableSize = 4093; // large prime number
        final GenericHashTable<Integer> iht = new GenericHashTable<>(tableSize);

        final Integer key1 = 3,              value1 = 12;
        final Integer key2 = 12,             value2 = 143;
        final Integer key3 = tableSize+12,   value3 = 99;
        final Integer key4 = 12,             value4 = 1;
        final Integer key5 = tableSize+12,   value5 = 2;
        final Integer key6 = 2*tableSize+12, value6 = 21;

        iht.put(key1,value1);
        iht.put(key2,value2);

        assertTrue("Missing key", iht.hasKey(key1));
        assertTrue("Missing key", iht.hasKey(key2));

        assertEquals("Wrong number enqueued", value1, iht.get(key1));
        assertEquals("Wrong number enqueued", value2, iht.get(key2));

        assertEquals("No collisions expected", 0, iht.getNumCollisions());

        iht.put(key3,value3);
        iht.put(key4,value4);

        assertEquals("Wrong entry", value3, iht.get(key3));
        assertEquals("Wrong entry", value4, iht.get(key4));

        assertEquals("Collisions expected", 1, iht.getNumCollisions());

        iht.remove(key3);

        assertFalse("No key expected", iht.hasKey(key3));
        assertEquals("Wrong entry", value4, iht.get(key4));
        assertEquals("No collisions expected", 0, iht.getNumCollisions());

        iht.put(key5,value5);
        iht.put(key6,value6);

        assertEquals("Collisions expected", 2, iht.getNumCollisions());

        assertEquals("Wrong entry", value4, iht.get(key4));
        assertEquals("Wrong entry", value5, iht.get(key5));
        assertEquals("Wrong entry", value6, iht.get(key6));

        iht.remove(key5);
        assertEquals("Wrong entry", value4, iht.get(key4));
        assertFalse("Key not expected", iht.hasKey(key5));

        assertEquals("Wrong entry", value6, iht.get(key6));
        assertEquals("Collisions expected", 1, iht.getNumCollisions());

        iht.printTable();
    }
}
