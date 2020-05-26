package dev.libhdc;

import java.util.ArrayList;
import java.nio.ByteBuffer;

import org.nd4j.linalg.api.ndarray.INDArray;
//import org.nd4j.linalg.api.buffer.factory.DefaultDataBufferFactory;
import org.nd4j.linalg.api.buffer.DataBuffer;
import org.nd4j.linalg.api.buffer.DataType;

// import org.nd4j.serde.binary.BinarySerde;
import org.nd4j.linalg.factory.Nd4j;

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

    public HDC() {
        create(null);
    }

    public HDC(String path) {
        create(path);
    }

    protected native void create(String path);

    public native void add_child(String path, HDC child);
    public native void set_child(String path, HDC child);
    public native void delete_child(String path);
    public native void append(HDC hdc);
    public native HDC get(String name);
    public native HDC get(long index);
    public native ArrayList<String> keys();
    public native boolean exists(String path);

    public native void set_data(ArrayList<Integer> shape, boolean[] data);
    public native void set_data(ArrayList<Integer> shape, byte[] data);
    public native void set_data(ArrayList<Integer> shape, short[] data);
    public native void set_data(ArrayList<Integer> shape, int[] data);
    public native void set_data(ArrayList<Integer> shape, long[] data);
    public native void set_data(ArrayList<Integer> shape, float[] data);
    public native void set_data(ArrayList<Integer> shape, double[] data);

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

    public String get_uuid() {
        return String.valueOf(nativeUUID);
    }

    public void print_uuid() {
        System.out.println(nativeUUID);
    }

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

    static {
        System.loadLibrary("hdc-jni");
    }
}
