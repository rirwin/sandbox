package structures;
import java.util.*;

public class IntHashTable {

    // internal array
    private IntNode arr[];

    private int size;

    public IntHashTable(int size) {
        this.size = size;
        this.arr = new IntNode[size];
    }

    public void insert(int key, int value) {
        int insPos = hashFunction(key);
        IntNode intNode = new IntNode(key, value);
        intNode.next = arr[insPos];
        arr[insPos] = intNode;
    }

    public void printTable() {
        System.out.println("Table:");
        for (int i = 0; i < size; i++) 
            if (arr[i] != null)
                System.out.println(arr[i].getKey() + " " + arr[i].getValue());
    }

    private int hashFunction(int key) {
        return key % size;
    }

    // make another file and generic, try inner class for now
    private class IntNode{
        private int key, value;
        public IntNode next;
        public IntNode(int key, int value) {
            this.key = key;
            this.value = value;
            this.next = null;
        }
        public int getKey(){
            return this.key;
        }
        public int getValue(){
            return this.value;
        }
    }
}
