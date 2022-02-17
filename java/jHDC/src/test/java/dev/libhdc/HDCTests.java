package dev.libhdc;

import dev.libhdc.HDC;
import java.util.ArrayList;
import java.nio.ByteBuffer;
import java.util.Arrays;
import org.nd4j.linalg.api.ndarray.INDArray;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.api.buffer.DataType;
import java.nio.ByteBuffer;
import java.util.Map;

public class HDCTests {

    @Test
    void NodeManipulationPath() {
        HDC.init();

        // Create new HDC tree
        HDC tree = new HDC();

        // test node manipulation
        HDC dummy1 = new HDC();
        tree.add_child("aaa/bbb/int", dummy1);
        assert(tree.exists("aaa"));
        assert(tree.exists("aaa/bbb"));
        assert(tree.exists("aaa/bbb/int"));

        HDC dummy2 = new HDC();
        tree.add_child("aaa/bbb/eee", dummy2);
        assert(tree.exists("aaa/bbb/eee"));
        HDC dummy3 = new HDC();
        tree.add_child("aaa/bbb/bbb", dummy3);
        assert(tree.exists("aaa/bbb/bbb"));

        ArrayList<String> keys = tree.keys();
        ArrayList<String> expected_keys = new ArrayList<String>();
        expected_keys.add("aaa");
        assertEquals(keys,expected_keys);

        HDC subtree = tree.get("aaa/bbb");
        assert(subtree.exists("int"));

        //test delete
        subtree.delete_child("int");
        assert(!subtree.exists("int"));

        //test set_child
        tree.add_child("set/aaa", new HDC());
        tree.set_child("set", new HDC());
        assert(!tree.exists("set/aaa"));

        // test children() method
        for (Map.Entry<String, HDC> entry : tree.children().entrySet())  {
            assert(tree.exists(entry.getKey()));
        }

    }

    @Test
    void NodeManipulationIndex() {
        HDC.init();
        //Test tree indexes manipulations
        HDC list = new HDC();
        for (int i=0; i<5; i++) {
            list.append(new HDC(String.valueOf(i)));
        }
        for (int i=0; i<5; i++) {
            assert(list.exists(i));
            assertEquals(list.get(i).get_string(),String.valueOf(i));
        }
        list.insert(0L,new HDC("7"));
        assertEquals(list.get(0L).get_string(),"7");
        assertEquals(list.get(1L).get_string(),"0");
        list.delete_slice(3L);
        assertEquals(list.get(3L).get_string(),"3");

        // Test slices() method
        long i=0;
        for (HDC slice : list.slices()) {
            assertEquals(slice.get_string(),list.get(i).get_string());
            i++;
        }
    }

    @Test
    void LongManipulation() {
        HDC.init();
        long[] array = new long[]{ 7, 2, 3, 4 };
        ArrayList<Integer> shape = new ArrayList<>();
        shape.add(array.length);
        long[] shape_ = new long[]{array.length};
        // test set/get data
        HDC node = new HDC();
        node.set_data(shape, array);
        assertEquals(shape,node.get_shape());
        long[] out_data = node.as_long_1d();
        assertArrayEquals(array,out_data);
        INDArray in_data = Nd4j.create(array,shape_,DataType.INT64);
        assertEquals(in_data,node.data());
        // test constructor
        HDC node2 = new HDC(shape, array);
        assertEquals(in_data,node2.data());
        // test set_data(INDArray)
        HDC node3 = new HDC();
        node3.set_data(in_data);
        assertEquals(in_data,node3.data());
        // test set_data(long[],...)
        HDC node4 = new HDC();
        node4.set_data(shape_, array);
        assertEquals(in_data,node4.data());
        // test set_external
        HDC external = new HDC();
        external.set_data(shape, array);
        assertEquals(in_data,external.data());
        HDC external2 = new HDC();
        external2.set_data(shape_, array);
        assertEquals(in_data,external2.data());
        HDC external3 = new HDC();
        external3.set_external(in_data);
        assertEquals(in_data,external3.data());
    }

    @Test
    void IntManipulation() {
        HDC.init();
        int[] array = new int[]{ 7, 2, 3, 4 };
        ArrayList<Integer> shape = new ArrayList<>();
        shape.add(array.length);
        long[] shape_ = new long[]{array.length};
        // test set/get data
        HDC node = new HDC();
        node.set_data(shape, array);
        int[] out_data = node.as_int_1d();
        assertArrayEquals(array,out_data);
        INDArray in_data = Nd4j.create(array,shape_,DataType.INT32);
        assertEquals(in_data,node.data());
        // test constructor
        HDC node2 = new HDC(shape, array);
        assertEquals(in_data,node2.data());
        // test set_data(INDArray)
        HDC node3 = new HDC();
        node3.set_data(in_data);
        assertEquals(in_data,node3.data());
        // test set_data(long[],...)
        HDC node4 = new HDC();
        node4.set_data(shape_, array);
        assertEquals(in_data,node4.data());
        // test set_external
        HDC external = new HDC();
        external.set_data(shape, array);
        assertEquals(in_data,external.data());
        HDC external2 = new HDC();
        external2.set_data(shape_, array);
        assertEquals(in_data,external2.data());
        HDC external3 = new HDC();
        external3.set_external(in_data);
        assertEquals(in_data,external3.data());
    }

    @Test
    void ShortManipulation() {
        HDC.init();
        short[] array = new short[]{ 7, 2, 3, 4 };
        ArrayList<Integer> shape = new ArrayList<>();
        shape.add(array.length);
        long[] shape_ = new long[]{array.length};
        // test set/get data
        HDC node = new HDC();
        node.set_data(shape, array);
        INDArray in_data = Nd4j.create(array,shape_,DataType.INT16);
        assertEquals(in_data,node.data());
        // test constructor
        HDC node2 = new HDC(shape, array);
        assertEquals(in_data,node2.data());
        // test set_data(long[],...)
        HDC node4 = new HDC();
        node4.set_data(shape_, array);
        assertEquals(in_data,node4.data());
        // test set_external
        HDC external = new HDC();
        external.set_data(shape, array);
        assertEquals(in_data,external.data());
        HDC external2 = new HDC();
        external2.set_data(shape_, array);
        assertEquals(in_data,external2.data());
    }

    @Test
    void ByteManipulation() {
        HDC.init();
        byte[] array = new byte[]{ 7, 2, 3, 4 };
        ArrayList<Integer> shape = new ArrayList<>();
        shape.add(array.length);
        long[] shape_ = new long[]{array.length};
        // test set/get data
        HDC node = new HDC();
        node.set_data(shape, array);
        INDArray in_data = Nd4j.create(array,shape_,DataType.INT8);
        assertEquals(in_data,node.data());
        // test constructor
        HDC node2 = new HDC(shape, array);
        assertEquals(in_data,node2.data());
        // test set_data(INDArray)
        HDC node3 = new HDC();
        node3.set_data(in_data);
        assertEquals(in_data,node3.data());
        // test set_data(long[],...)
        HDC node4 = new HDC();
        node4.set_data(shape_, array);
        assertEquals(in_data,node4.data());
        // test set_external
        HDC external = new HDC();
        external.set_data(shape, array);
        assertEquals(in_data,external.data());
        HDC external2 = new HDC();
        external2.set_data(shape_, array);
        assertEquals(in_data,external2.data());
        HDC external3 = new HDC();
        external3.set_external(in_data);
        assertEquals(in_data,external3.data());
    }

    @Test
    void FloatManipulation() {
        HDC.init();
        float[] array = new float[]{ 7.7F, 2.2F, 3e20F, 4e-16F };
        ArrayList<Integer> shape = new ArrayList<>();
        shape.add(array.length);
        long[] shape_ = new long[]{array.length};
        // test set/get data
        HDC node = new HDC();
        node.set_data(shape, array);
        float[] out_data = node.as_float_1d();
        assertArrayEquals(array,out_data);
        INDArray in_data = Nd4j.create(array,shape_,DataType.FLOAT);
        assertEquals(in_data,node.data());
        // test constructor
        HDC node2 = new HDC(shape, array);
        assertEquals(in_data,node2.data());
        // test set_data(INDArray)
        HDC node3 = new HDC();
        node3.set_data(in_data);
        assertEquals(in_data,node3.data());
        // test set_data(long[],...)
        HDC node4 = new HDC();
        node4.set_data(shape_, array);
        assertEquals(in_data,node4.data());
        // test set_external
        HDC external = new HDC();
        external.set_data(shape, array);
        assertEquals(in_data,external.data());
        HDC external2 = new HDC();
        external2.set_data(shape_, array);
        assertEquals(in_data,external2.data());
        HDC external3 = new HDC();
        external3.set_external(in_data);
        assertEquals(in_data,external3.data());
    }

    @Test
    void DoubleManipulation() {
        HDC.init();
        double[] array = new double[]{ 7.7, 2.2, 3e20, 4e-16 };
        ArrayList<Integer> shape = new ArrayList<>();
        shape.add(array.length);
        long[] shape_ = new long[]{array.length};
        // test set/get data
        HDC node = new HDC();
        node.set_data(shape, array);
        double[] out_data = node.as_double_1d();
        assertArrayEquals(array,out_data);
        INDArray in_data = Nd4j.create(array,shape_,DataType.DOUBLE);
        assertEquals(in_data,node.data());
        // test constructor
        HDC node2 = new HDC(shape, array);
        assertEquals(in_data,node2.data());
        // test set_data(INDArray)
        HDC node3 = new HDC();
        node3.set_data(in_data);
        assertEquals(in_data,node3.data());
        // test set_data(long[],...)
        HDC node4 = new HDC();
        node4.set_data(shape_, array);
        assertEquals(in_data,node4.data());
        // test set_external
        HDC external = new HDC();
        external.set_data(shape, array);
        assertEquals(in_data,external.data());
        HDC external2 = new HDC();
        external2.set_data(shape_, array);
        assertEquals(in_data,external2.data());
        HDC external3 = new HDC();
        external3.set_external(in_data);
        assertEquals(in_data,external3.data());
    }

//     @Test
//     void BooleanManipulation() {
//         HDC.init();
//         boolean[] array = new boolean[]{ true, true, false };
//         ArrayList<Integer> shape = new ArrayList<>();
//         shape.add(array.length);
//         long[] shape_ = new long[]{array.length};
//         // test set/get data
//         HDC node = new HDC();
//         node.set_data(shape, array);
//         INDArray in_data = Nd4j.create(array,shape_,DataType.BOOL);
//         assertEquals(in_data,node.data());
//         // test constructor
//         HDC node2 = new HDC(shape, array);
//         assertEquals(in_data,node2.data());
//     }

    @Test
    void LongScalarManipulation() {
        long n = 42;
        HDC node = new HDC();
        node.set_data(n);
        assertEquals(node.scalar(),n);

        HDC node2 = new HDC(n);
        assertEquals(node2.scalar(),n);
    }

    @Test
    void IntScalarManipulation() {
        int n = 42;
        HDC node = new HDC();
        node.set_data(n);
        assertEquals(node.scalar(),n);

        HDC node2 = new HDC(n);
        assertEquals(node2.scalar(),n);
    }

    @Test
    void ShortScalarManipulation() {
        short n = 42;
        HDC node = new HDC();
        node.set_data(n);
        assertEquals(node.scalar(),n);

        HDC node2 = new HDC(n);
        assertEquals(node2.scalar(),n);
    }

    @Test
    void ByteScalarManipulation() {
        byte n = 42;
        HDC node = new HDC();
        node.set_data(n);
        assertEquals(node.scalar(),n);

        HDC node2 = new HDC(n);
        assertEquals(node2.scalar(),n);
    }

    @Test
    void FloatScalarManipulation() {
        float n = 42.42F;
        HDC node = new HDC();
        node.set_data(n);
        assertEquals(node.scalar(),n);

        HDC node2 = new HDC(n);
        assertEquals(node2.scalar(),n);
    }

    @Test
    void DoubleScalarManipulation() {
        double n = 42.42;
        HDC node = new HDC();
        node.set_data(n);
        assertEquals(node.scalar(),n);

        HDC node2 = new HDC(n);
        assertEquals(node2.scalar(),n);
    }

    @Test
    void StringManipulation() {
        String str_in = "test string";
        HDC node = new HDC();
        node.set_string(str_in);
        String str = node.get_string();
        assertEquals(str,str_in);

        HDC node2 = new HDC(str_in);
        String str2 = node2.get_string();
        assertEquals(str2,str_in);
    }

}
