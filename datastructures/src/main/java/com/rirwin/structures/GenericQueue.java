package com.rirwin.structures;

import java.util.NoSuchElementException;

public class GenericQueue<T> {

    // head node
    private Node<T> head;

    // tail node
    private Node<T> tail;

    // size of queue
    private final int size;

    // number of items in queue
    private int numEnqueued;

    public GenericQueue(final int s) {
        head = null;
        tail = null;
        numEnqueued = 0;
        this.size = s;
    }

    public int getNumEnqueued() {
        return numEnqueued;
    }

    public void push(final T x) {

        if (numEnqueued == size) {
            return; // Queue full
        }

        Node<T> node = new Node<T>(x);

        if (tail == null) { // empty queue
            head = node;
            tail = node;
        }
        else {
            tail.next = node;
            tail = tail.next;
        }
        numEnqueued++;
    }

    public void printContents() {

        Node<T> node = head;
        System.out.print("Contents: ");

        while(node != null) {
            System.out.print(node.value + " ");
            node = node.next;
        }

        System.out.println();
    }

    public void printQueue() {
        System.out.print("Queue: ");
        printContents();
    }

    public T front() {
        if (head == null) {
            throw new NoSuchElementException();
        }
        return head.value;
    }

    public void pop() {

        if (head == null) {
            System.out.println("pop() Empty Queue");
        }
        else {
            Node<T> node = head;
            head = head.next;
            node = null;
            numEnqueued--;
        }
    }

    private class Node<T> {
        public T value;
        public Node<T> next;

        public Node() {
            value = null;
            next = null;
        }

        public Node(T v) {
            value = v;
            next = null;
        }
    }
    
}
