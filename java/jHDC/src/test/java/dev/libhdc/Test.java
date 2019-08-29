import dev.libhdc.HDC;
import java.util.ArrayList;
import java.nio.ByteBuffer;
import java.util.Arrays;

import org.nd4j.linalg.api.ndarray.INDArray;

public class Test {

    public static void main(String[] args) {
        HDC.init();

        // Create new HDC tree
        HDC tree = new HDC();

        // Add some children
        HDC dummy1 = new HDC();
        tree.add_child("aaa/bbb/ccc", dummy1);
        HDC dummy2 = new HDC();
        tree.add_child("aaa/bbb/eee", dummy2);
        HDC dummy3 = new HDC();
        tree.add_child("aaa/bbb/bbb", dummy3);

        System.out.println("root keys");
        for (String key : tree.keys()) {
            System.out.println("key: " + key);
        }

        System.out.println("aaa/bbb keys");
        for (String key : tree.get("aaa/bbb").keys()) {
            System.out.println("key: " + key);
        }

        // Get subtree
        HDC subtree = tree.get("aaa/bbb");
        // Get node
        HDC node = subtree.get("ccc");

        // Ask whether child exists
        System.out.println("exists: " + tree.exists("aaa/bbb/eee"));

        // Delete subtree
        tree.delete_child("aaa/bbb/eee");

        // Ask whether child exists
        System.out.println("exists: " + tree.exists("aaa/bbb/eee"));

        // Prepare some data
        int[] array = new int[]{ 7, 2, 3, 4 };
        ArrayList<Integer> shape = new ArrayList<>();
        shape.add(4);

        // Add data to a single node
        HDC data = new HDC();
        data.set_data(shape, array);

        // Add data to a subtree
        tree.get("aaa/bbb/ccc").set_data(shape, array);

        // Ask for some data details, use subtree to shorten the path
        long rank = node.get_rank();
        ArrayList<Integer> shape2 = node.get_shape();

        System.out.println("Dimension: " + rank);
        System.out.println("Shape: " + shape2);
        System.out.println("dtype: " + node.get_type_str());

        ByteBuffer buffer = node.get_data();
        System.out.println("Buffer: " + buffer);

        INDArray out_data = node.data();
        System.out.println("Data: " + out_data);

        // Serialize data to JSON
        tree.to_json("tree.json", 0);

        // On screen
        tree.dump();
        tree.serialize("pokus.json");
    }

}