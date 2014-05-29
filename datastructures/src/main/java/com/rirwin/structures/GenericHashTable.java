package com.rirwin.structures;

import java.util.*;

/**
 * Class for creating a custom hashtable with integer keys and values.
 * The current hash function uses mod.
 * Collisions are handled through chaining.
 */
public class GenericHashTable<T> {

    /**
     * Internal array for hash table
     */
    private Node<T> arr[];

    private int size;

    private int collisionCnt;

    GenericHashTable(int size) {
        this.size = size;
        this.arr = new Node[size];
        this.collisionCnt = 0;
    }

    public void put(int key, T value) {
        int insPos = hashFunction(key);

        // check if already in array
        Node<T> node = arr[insPos];

        while (node != null) {

            // if in array update value, return
            if (node.key == key) {
                node.value = value;
                return;
            }
            node = node.next;
        }

        // create a new Node
        node = new Node<T>(key, value);
        if (arr[insPos] != null) {
            node.next = arr[insPos];
            this.collisionCnt++;
        }

        // put at normal position, no collision
        arr[insPos] = node;
    }


    public void remove(int key) {
        int pos = hashFunction(key);
        Node<T> node = arr[pos];

        if (node == null) throw new NoSuchElementException();

        // Handle head of list being equal to key
        if (node.key == key) {
            arr[pos] = node.next;
            if (node.next != null)
                this.collisionCnt--;
            node = null;
            return;
        }

        Node<T> nextNode = node.next;

        // Check key of nextNode in loop
        while (nextNode != null) {
            if (nextNode.key == key) {
                this.collisionCnt--;
                node.next = nextNode.next;
                nextNode = null;
                return;
            }
            node = nextNode;
            nextNode = nextNode.next;
        }

        // If no element matches key, throw exception
        throw new NoSuchElementException();
    }

    public boolean hasKey(int key) {
        int pos = hashFunction(key);
        Node<T> node = arr[pos];
        while(node != null) {
            if (node.key == key)
                return true;
            node = node.next;
        }
        return false;
    }

    /**
     * Returns value if exists
     * Return null if not exists
     */
    public T get(int key) {
        int pos = hashFunction(key);
        Node<T> node = arr[pos];
        while(node != null) {
            if (node.key == key)
                return node.value;
            node = node.next;
        }
        return null;
    }

    public void printTable() {
        System.out.println("Table:");
        for (int i = 0; i < size; i++) {
            Node<T> node = arr[i];
            while (node != null) {
                System.out.println(node.key + " " + node.value);
                node = node.next;
            }
        }
        System.out.println("Number of collisions: " + this.collisionCnt);
    }

    public int getNumCollisions() {
        return this.collisionCnt;
    }

    private int hashFunction(int key) {
        return key % size;
    }

    private class Node<T> {
        public T value;
        public int key;
        public Node next;
        public Node(int k, T v) {
            key = k;
            value = v;
            next = null;
        }
    }
    
}
