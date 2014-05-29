package com.rirwin.structures;

import java.util.Vector;
import java.util.NoSuchElementException;

public class GenericQueue<T> {

    private final Vector<T> v;

    private final int s;

    private int numEnqueued;

    public GenericQueue(int queueSize) {
        v = new Vector<T>();
        s = queueSize;
        numEnqueued = 0;
    }

    public int getNumEnqueued() {
        return numEnqueued;
    }

    public void push(T elem) {

        if (v.size() == s) {
            //System.out.println("Queue is full");
        }
        else {
            v.add(elem);
            numEnqueued++;
        }
    }    

    public void printContents() {
        System.out.print("Contents: ");
        for (int ii = 0; ii < numEnqueued; ii++) {
            System.out.print(v.get(ii) + " ");
        }
        System.out.println();
    }

    public void printQueue() {
        printContents();
    }

    public T front() {
        if (numEnqueued == 0) {
            throw new NoSuchElementException();
        }
        else {
            return v.get(0);
        }
    }

    public void pop() {
        if (numEnqueued == 0) {
            System.out.println("pop() Empty Queue");
        }
        else {;
            v.remove(0);
            numEnqueued--;
        }
    }
    
}
