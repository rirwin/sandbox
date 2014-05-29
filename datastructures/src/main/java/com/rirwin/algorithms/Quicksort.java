package com.rirwin.algorithms;

public class Quicksort {

    public static void sort(int a[]){
        quickSort(a, 0, a.length-1);
    }

    private static void quickSort(int a[], int l, int r){
        if (l < r) {
            int piv = l;
            int newPiv = partition(a, l, r, piv);
            quickSort(a, l, newPiv-1);
            quickSort(a, newPiv+1, r);
        }
    }

    private static int partition(int a[], int l, int r, int p) {

        int pval = a[p];

        swap(a, p, r);
        int idx = l;
        for (int i = l; i < r; i++) {
            if (a[i] <= pval) {
                swap(a, i, idx);
                idx++;
            }
        }
        swap(a, idx, r);

        return idx;
    }

    private static void printArray(int a[]){
        System.out.print("Array: ");
        for (int i = 0; i < a.length; i++) {
            System.out.print(a[i] + " ");
        }
        System.out.println();
    }

    private static void swap(int a[], int i, int j) {
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}
