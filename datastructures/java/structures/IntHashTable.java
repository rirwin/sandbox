package structures;
import java.util.*;

public class IntHashTable {

    // internal array
    private IntNode arr[];

    private int size;

    private int collisionCnt;

    public IntHashTable(int size) {
        this.size = size;
        this.arr = new IntNode[size];
        this.collisionCnt = 0;
    }

    public void insert(int key, int value) {
        int insPos = hashFunction(key);
        IntNode intNode = new IntNode(key, value);
        if (arr[insPos] != null) {
            intNode.next = arr[insPos];
            this.collisionCnt++;
        }
        arr[insPos] = intNode;
    }

    public void printTable() {
        System.out.println("Table:");
        for (int i = 0; i < size; i++) {
            IntNode node = arr[i];
            while (node != null) {
                System.out.println(node.getKey() + " " + node.getValue());
                node = node.next;
            }
        }
        System.out.println("Number of collisions: " + this.collisionCnt);
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
