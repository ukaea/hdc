import dev.libhdc.HDC;
import java.util.ArrayList;
import java.nio.ByteBuffer;
import java.util.Arrays;
import org.nd4j.linalg.api.ndarray.INDArray;

public class Example {
    public static void main(String[] args) {
        HDC.init();

        // Create new HDC tree
        HDC tree = new HDC();

        // Add some children
        HDC dummy1 = new HDC();
        tree.add_child("aaa/bbb/int", dummy1);
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
        HDC node = subtree.get("int");

        // Ask whether child exists
        System.out.println("exists: " + tree.exists("aaa/bbb/eee"));

        // Delete subtree
        tree.delete_child("aaa/bbb/eee");

        // Ask whether child exists
        System.out.println("exists: " + tree.exists("aaa/bbb/eee"));

        // Test set_child
        HDC old_ = new HDC();
        old_.set_string("old");
        HDC new_ = new HDC();
        new_.set_string("new");
        tree.add_child("aaa/node",old_);
        tree.set_child("aaa/node",new_);

        // Prepare some data
        int[] array = new int[]{ 7, 2, 3, 4 };
        ArrayList<Integer> shape = new ArrayList<>();
        shape.add(array.length);

        // Add data to a single node
        HDC data = new HDC();
        data.set_data(shape, array);

        // Add data to a subtree
        tree.get("aaa/bbb/int").set_data(shape, array);

        tree.add_child("aaa/bbb/bbb", dummy3);

        //Test tree indexes manipulations
        HDC list = new HDC();
        for (int i=0; i<5; i++) {
            HDC n = new HDC();
            n.set_string(String.valueOf(i));
            list.append(n);
        }
        HDC n = new HDC();
        n.set_string("7");
        list.insert(0,n);
        list.delete_slice(3L);
        tree.add_child("aaa/list",list);

        // Add longs
        long[] arrayOfLongs = new long[]{ 7, 2, 3, 4 };
        ArrayList<Integer> shapeOfLongs = new ArrayList<>();
        shapeOfLongs.add(arrayOfLongs.length);
        HDC nodeLongs = new HDC();
        nodeLongs.set_data(shapeOfLongs, arrayOfLongs);
        tree.add_child("aaa/bbb/long", nodeLongs);

        //add ints
        int[] arrayOfInts = new int[]{ 7, 2, 3, 4 };
        ArrayList<Integer> shapeOfInts = new ArrayList<>();
        shapeOfInts.add(arrayOfInts.length);
        HDC nodeInts = new HDC();
        nodeInts.set_data(shapeOfInts, arrayOfInts);
        tree.add_child("aaa/bbb/int", nodeInts);
        // Add doubles
        double[] arrayOfDouble = new double[]{ 7e-11, 2.2, 3e2, 4e10 };
        ArrayList<Integer> shapeOfDouble = new ArrayList<>();
        shapeOfDouble.add(arrayOfDouble.length);
        HDC nodeDouble = new HDC();
        nodeDouble.set_data(shapeOfDouble, arrayOfDouble);
        tree.add_child("aaa/bbb/double", nodeDouble);

        // Add string
        HDC nodeStr = new HDC();
        nodeStr.set_string("test string");
        tree.add_child("aaa/bbb/string",nodeStr);
        String str = tree.get("aaa/bbb/string").get_string();
        System.out.println("String: " + str);

        //test constructors
        HDC nodeDoubleC = new HDC(shapeOfDouble, arrayOfDouble);
        tree.add_child("aaa/constructors/double", nodeDoubleC);
        HDC nodeIntsC = new HDC(shapeOfInts, arrayOfInts);
        tree.add_child("aaa/constructors/int", nodeIntsC);

        //test constructors_scalar
        HDC nodeDoubleScalarC = new HDC(3.14);
        tree.add_child("aaa/constructors_scalar/double", nodeDoubleScalarC);
        HDC nodeIntsScalarC = new HDC(1111111);
        tree.add_child("aaa/constructors_scalar/int", nodeIntsScalarC);

        System.out.println("Scalar int   : " + nodeIntsScalarC.scalar());
        System.out.println("Scalar double: " + nodeDoubleScalarC.scalar());

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

        // On screen
        tree.dump();

        // Serialize data to JSON
        tree.save("json://tree.json", "");

        //load from JSON
        HDC tree2 = HDC.load("json://tree.json", "");
        HDC node2 = tree2.get("aaa/bbb/int");
        INDArray out_data2 = node2.data();
        System.out.println("Loaded data: " + out_data2);
    }

}
