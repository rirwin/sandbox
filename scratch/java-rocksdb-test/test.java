import org.rocksdb.RocksDB;
import org.rocksdb.Options;
import org.rocksdb.RocksDBException;


public class test {
    public static void main(String[] args) {

        // a static method that loads the RocksDB C++ library.
        System.out.println(System.getProperty("java.library.path"));
        
        String k1 = "k1";
        String v1 = "v1";
        /*
        byte[] key1 = new byte[k1.getBytes().length()+1];
        byte[] key2 = new byte[k2.getBytes().length()+1];

        key1[0] = (byte) (0xFF & k1.getBytes().length());
        key2[0] = (byte) (0xFF & k2.getBytes().length());

        System.arraycopy(k1.getBytes(), 0, key1, 1, k1.length());
        System.arraycopy(k2.getBytes(), 0, key2, 1, k2.length());
        */

        byte[] key1 = k1.getBytes();
        byte[] val1 = v1.getBytes();

        RocksDB.loadLibrary();
        // the Options class contains a set of configurable DB options
        // that determines the behavior of a database.
        Options options = new Options().setCreateIfMissing(true);
        RocksDB db = null;
        try {
            // a factory method that returns a RocksDB instance
            db = RocksDB.open(options, "testdb");
            // do something
            db.put(key1, val1);
            byte[] buff = db.get(key1);
            System.out.println(new String(buff));
            
        } catch (RocksDBException e) {
            // do some error handling
            
        }
        finally {
            if (db != null) db.close();
            options.dispose();
        }
        
    }
}
