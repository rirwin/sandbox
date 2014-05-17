import structures.IntHashTable;

public class TestIntHashTable {

    public static void main(String[] args) {
        
        int tableSize = 4093;
        IntHashTable iht = new IntHashTable(tableSize);
        iht.printTable();
        iht.insert(3,12);
        iht.insert(13,90);
        iht.printTable();
    }
}
