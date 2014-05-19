package com.rirwin.structures;

public class TestIntQueue {

    public static void main(String[] args) {
        
        int queueSize = 100;
        IntQueue q = new IntQueue(queueSize);

        assert q.getNumEnqueued() == 0;

        int first = 13;
        q.push(first);

        assert q.front() == first;

        for (int i = 0; i < queueSize*2; i++) {
            q.push(i);
        }

        assert q.front() == first;

        for (int i = 0; i < queueSize-1; i++){
            q.pop();
            assert q.front() == i : "Test enqueued elements failed";
        }

    }
}
