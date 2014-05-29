package com.rirwin.structures;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class GenericQueueTest {

    @Test
    public void testGenericQueue() {

        final int queueSize = 100;
        final GenericQueue<Integer> iq = new GenericQueue<>(queueSize);

        assertEquals("Wrong number enqueued", 0, iq.getNumEnqueued());

        final int first = 13;
        iq.push(first);

        assertEquals("First element not accepted", (int)first, (int)iq.front());

        for (int i = 0; i < queueSize*2; i++) {
            iq.push(i);
        }

        assertEquals("First element disappeared", (int)first, (int)iq.front());

        for (int i = 0; i < queueSize-1; i++){
            iq.pop();
            assertEquals((int)i, (int)iq.front());
        }

                
        final GenericQueue<Double> dq = new GenericQueue<>(queueSize);

                
    }
}
