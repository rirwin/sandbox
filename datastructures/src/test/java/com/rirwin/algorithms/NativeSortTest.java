package com.rirwin.algorithms;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

public class NativeSortTest{

    @Test
    public void testNativeSort() {
        int arr[] = {4,3,2,5,7};
        NativeSort.arraySort(arr);

        for (int i = 0; i < arr.length-1; i++) {
            assertTrue("Elements not in sorted order", arr[i] <= arr[i+1]);
        }
    }
}
