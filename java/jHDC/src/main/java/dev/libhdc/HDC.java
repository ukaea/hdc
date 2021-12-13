package dev.libhdc;

import java.util.ArrayList;
import java.util.Arrays;
import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.nio.IntBuffer;

import org.nd4j.linalg.api.ndarray.INDArray;
//import org.nd4j.linalg.api.buffer.factory.DefaultDataBufferFactory;
import org.nd4j.linalg.api.buffer.DataBuffer;
import org.nd4j.linalg.api.buffer.DataType;
import java.util.LinkedHashMap;
// import org.nd4j.serde.binary.BinarySerde;
import org.nd4j.linalg.factory.Nd4j;
import org.apache.commons.lang3.ArrayUtils;

public class HDC {
    public class HDCException extends RuntimeException {
        public HDCException(String msg) { super(msg); }
    }

    //private String nativeUUID;
    private byte[] nativeUUID;
    private long nativeStorageID;

    private static final int HDC_EMPTY  =  0;
    private static final int HDC_STRUCT =  1;
    private static final int HDC_LIST   =  2;
    private static final int HDC_INT8   =  3;
    private static final int HDC_INT16  =  4;
    private static final int HDC_INT32  =  5;
    private static final int HDC_INT64  =  6;
    private static final int HDC_UINT8  =  7;
    private static final int HDC_UINT16 =  8;
    private static final int HDC_UINT32 =  9;
    private static final int HDC_UINT64 = 10;
    private static final int HDC_FLOAT  = 11;
    private static final int HDC_DOUBLE = 12;
    private static final int HDC_STRING = 13;
    private static final int HDC_BOOL   = 14;
    private static final int HDC_ERROR  = 15;

    protected HDC(byte[] uuid, long storageID) {
        nativeUUID = new byte[16];
        for (int i=0; i<16; i++) nativeUUID[i] = uuid[i];
        nativeStorageID = storageID;
    }

    public HDC(ArrayList<Integer> shape, double[] data) {
        create_double_array(shape,data);
    }
    public HDC(ArrayList<Integer> shape, boolean[] data) {
        create_boolean_array(shape,data);
    }
    public HDC(ArrayList<Integer> shape, byte[] data) {
        create_byte_array(shape,data);
    }
    public HDC(ArrayList<Integer> shape, short[] data) {
        create_short_array(shape,data);
    }
    public HDC(ArrayList<Integer> shape, int[] data) {
        create_int_array(shape,data);
    }
    public HDC(ArrayList<Integer> shape, long[] data) {
        create_long_array(shape,data);
    }
    public HDC(ArrayList<Integer> shape, float[] data) {
        create_float_array(shape,data);
    }

    public HDC(double data) {
        create_double(data);
    }
    public HDC(boolean data) {
        create_boolean(data);
    }
    public HDC(byte data) {
        create_byte(data);
    }
    public HDC(short data) {
        create_short(data);
    }
    public HDC(int data) {
        create_int(data);
    }
    public HDC(long data) {
        create_long(data);
    }
    public HDC(float data) {
        create_float(data);
    }

    public HDC() {
        create(null);
    }

    public HDC(String str) {
        create(str);
    }

    protected native void create(String str);
    protected native void create_double_array(ArrayList<Integer> shape, double[] data);
    protected native void create_float_array(ArrayList<Integer> shape, float[] data);
    protected native void create_long_array(ArrayList<Integer> shape, long[] data);
    protected native void create_int_array(ArrayList<Integer> shape, int[] data);
    protected native void create_short_array(ArrayList<Integer> shape, short[] data);
    protected native void create_byte_array(ArrayList<Integer> shape, byte[] data);
    protected native void create_boolean_array(ArrayList<Integer> shape, boolean[] data);

    protected native void create_double(double data);
    protected native void create_float(float data);
    protected native void create_long(long data);
    protected native void create_int(int data);
    protected native void create_short(short data);
    protected native void create_byte(byte data);
    protected native void create_boolean(boolean data);

    protected native byte as_byte();
    protected native short as_short();
    protected native int as_int();
    protected native long as_long();
    protected native float as_float();
    protected native double as_double();
    public native boolean as_bool();

    public native void add_child(String path, HDC child);
    public native void set_child(String path, HDC child);
    public native void delete_child(String path);
    public native void delete_slice(long index);
    public native void append(HDC hdc);
    public native void insert(long index, HDC hdc);
    public native HDC get(String name);
    public native HDC get(long index);
    public native ArrayList<String> keys();
    public native boolean exists(String path);
    public native boolean exists(long index);

    public native void set_data(ArrayList<Integer> shape, boolean[] data);
    public native void set_data(ArrayList<Integer> shape, byte[] data);
    public native void set_data(ArrayList<Integer> shape, short[] data);
    public native void set_data(ArrayList<Integer> shape, int[] data);
    public native void set_data(ArrayList<Integer> shape, long[] data);
    public native void set_data(ArrayList<Integer> shape, float[] data);
    public native void set_data(ArrayList<Integer> shape, double[] data);

    protected ArrayList<Integer> array2arrayList(long[] arr) {
        ArrayList<Integer> al = new ArrayList<>(arr.length);
        for (long i : arr) {
            al.add((int)i);
        }
        return al;
    }

    public void set_data(long[] shape, boolean[] data) {
        set_data(array2arrayList(shape),data);
    }

    public void set_data(long[] shape, byte[] data) {
        set_data(array2arrayList(shape),data);
    }

    public void set_data(long[] shape, short[] data) {
        set_data(array2arrayList(shape),data);
    }
    
    public void set_data(long[] shape, int[] data) {
        set_data(array2arrayList(shape),data);
    }

    public void set_data(long[] shape, long[] data) {
        set_data(array2arrayList(shape),data);
    }

    public void set_data(long[] shape, float[] data) {
        set_data(array2arrayList(shape),data);
    }

    public void set_data(long[] shape, double[] data) {
        set_data(array2arrayList(shape),data);
    }

    public native void set_data(boolean data);
    public native void set_data(byte data);
    public native void set_data(short data);
    public native void set_data(int data);
    public native void set_data(long data);
    public native void set_data(float data);
    public native void set_data(double data);

    public native void set_string(String str);

    public native ByteBuffer get_data();

    public INDArray data() {
        ByteBuffer buffer = get_data();
        long rank = get_rank();
        long length = 1;
        ArrayList<Integer> shape = get_shape();
        long[] shape_ = new long[(int)rank];
        for(int i=0;i<rank;i++) {
            shape_[i] = shape.get(i);
            length *= shape_[i];
        }
        DataBuffer data = null;
        switch ((int)get_type()) {
            case HDC_INT8:
                data = Nd4j.createBuffer(buffer, DataType.INT8, (int)length);
                break;
            case HDC_INT16:
                data = Nd4j.createBuffer(buffer, DataType.INT16, (int)length);
                break;
            case HDC_INT32:
                data = Nd4j.createBuffer(buffer, DataType.INT32, (int)length);
                break;
            case HDC_INT64:
                data = Nd4j.createBuffer(buffer, DataType.INT64, (int)length);
                break;
            case HDC_UINT8:
                data = Nd4j.createBuffer(buffer, DataType.UINT8, (int)length);
                break;
            case HDC_UINT16:
                data = Nd4j.createBuffer(buffer, DataType.UINT16, (int)length);
                break;
            case HDC_UINT32:
                data = Nd4j.createBuffer(buffer, DataType.UINT32, (int)length);
                break;
            case HDC_UINT64:
                data = Nd4j.createBuffer(buffer, DataType.UINT64, (int)length);
                break;
            case HDC_FLOAT:
                data = Nd4j.createBuffer(buffer, DataType.FLOAT, (int)length);
                break;
            case HDC_DOUBLE:
                data = Nd4j.createBuffer(buffer, DataType.DOUBLE, (int)length);
                break;
            case HDC_BOOL:
                data = Nd4j.createBuffer(buffer, DataType.BOOL, (int)length);
                break;
            default:
                throw new HDCException("Cannot return data of type " + get_type_str() + " as NDArray");
        }
        return Nd4j.create(data,shape_);
    }

    public int[] as_int_1d() {
        return data().toIntVector();
    }

    public long[] as_long_1d() {
        return data().toLongVector();
    }

    public float[] as_float_1d() {
        return data().toFloatVector();
    }

    public double[] as_double_1d() {
        return data().toDoubleVector();
    }

    public int[][] as_int_2d() {
        return data().toIntMatrix();
    }
    
    public long[][] as_long_2d() {
        return data().toLongMatrix();
    }

    public float[][] as_float_2d() {
        return data().toFloatMatrix();
    }
    
    public double[][] as_double_2d() {
        return data().toDoubleMatrix();
    }

    public void set_data(INDArray data) {
        long[] shape_ = data.shape();
        ArrayList<Integer> shape = new ArrayList<>(shape_.length);
        for (long i : shape_) {
            shape.add((int)i);
        }
        switch (data.dataType()) {
            case BYTE:
                set_data(shape, data.data().asBytes());
                break;
            case INT:
                set_data(shape, data.data().asInt());
                break;
            case LONG:
                set_data(shape, data.data().asLong());
                break;
            case FLOAT:
                set_data(shape, data.data().asFloat());
                break;
            case DOUBLE:
                set_data(shape, data.data().asDouble());
                break;
            default:
                throw new HDCException("Unsupported type: " + data.dataType() );
        }
    }

    public Number scalar() {
        Number n;
        switch ((int)get_type()) {
            case HDC_INT8:
                n = as_byte();
                break;
            case HDC_INT16:
                n = as_short();
                break;
            case HDC_INT32:
                n = as_int();
                break;
            case HDC_INT64:
                n = as_long();
                break;
            case HDC_UINT8:
                n = as_byte();
                break;
            case HDC_UINT16:
                n = as_short();
                break;
            case HDC_UINT32:
                n = as_int();
                break;
            case HDC_UINT64:
                n = as_long();
                break;
            case HDC_FLOAT:
                n = as_float();
                break;
            case HDC_DOUBLE:
                n = as_double();
                break;
            default:
                throw new HDCException("Cannot return data of type " + get_type_str() + " as Number");
        }
        return n;
    }

    public String get_uuid() {
        return String.valueOf(nativeUUID);
    }

    public void print_uuid() {
        System.out.println(nativeUUID);
    }

    public native String get_string();

    public native long get_rank();
    public native ArrayList<Integer> get_shape();
    public native String serialize(String protocol);
    public native void save(String uri, String datapath);
    public final static native HDC load(String uri, String datapath);
    public native void print_info();
    public native long get_type();
    public native String get_type_str();
    public native void dump();

    public final static native void init();
    public final static native HDC load(String uri);

    public String dumps() {
        return serialize("json");
    }

    public LinkedHashMap<String,HDC> children() {
        LinkedHashMap<String,HDC> children_ = new LinkedHashMap<String,HDC>();
        for (String key : keys()) {
            children_.put(key,get(key));
        }
        return children_;
    }

    public ArrayList<HDC> slices() {
        ArrayList<HDC> slices_ = new ArrayList<HDC>();
        for (long i=0; i<get_shape().get(0); i++) slices_.add(get(i));
        return slices_;
    }

    static {
        System.loadLibrary("hdc-jni");
    }
}
