package structures;
import java.util.*;

public class IntQueue {

    // array container
    private int[] a;

    // i is head of queue, j is tail
    private int i, j;

    // size of queue
    private int s;

    // number of items in queue
    private int numEnqueued;

    public IntQueue(int s) {
        this.i = 0;
        this.j = 0;
        this.a = new int[s];
        this.s = s;
        this.numEnqueued = 0;
    }

    public int getNumEnqueued() {
        return this.numEnqueued;
    }

    public void push(int x) {
        if ( j == i && numEnqueued > 0 ){
            //System.out.println("Queue is full");
        }
        else {
            // place at tail, inc
            a[j] = x;
            j = (j + 1) % s;
            numEnqueued++;
        }
    }

    public void printContents() {
        System.out.print("Contents: ");
        for (int ii = 0; ii < this.s; ii++)
            System.out.print(this.a[ii] + " ");
        System.out.println();
    }

    public void printQueue() {
        System.out.print("Queue: ");
        for (int offset = 0; offset < this.numEnqueued; offset++)
            System.out.print(this.a[(this.i + offset) % this.s] + " ");
        System.out.println();
    }

    public int front() {
        if (numEnqueued == 0) {
            throw new NoSuchElementException();
        }
        return a[i];
    }

    public void pop() {

        if (numEnqueued == 0) {
            System.out.println("pop() Empty Queue");
        }
        else {
            int x = a[i];
            i = (i + 1) % s;
            numEnqueued--;
        }
    }

    public int get(int i) {
        return a[i];
    }

}
