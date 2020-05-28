#include "dev_libhdc_HDC.h"

#include <hdc.hpp>

namespace
{

jfieldID getUUIDField(JNIEnv* jEnv, jobject jObj)
{
    jclass hdc_class = jEnv->GetObjectClass(jObj);
    return jEnv->GetFieldID(hdc_class, "nativeUUID", "[B");
}

jfieldID getStorageField(JNIEnv* jEnv, jobject jObj)
{
    jclass hdc_class = jEnv->GetObjectClass(jObj);
    return jEnv->GetFieldID(hdc_class, "nativeStorageID", "J");
}

void initHDC(JNIEnv* jEnv, jobject jObj, const hdc_t& hdc)
{
    jEnv->SetLongField(jObj, getStorageField(jEnv, jObj), hdc.storage_id);

    jbyteArray jUUID = jEnv->NewByteArray(HDC_UUID_LENGTH);

    jbyte *carr = jEnv->GetByteArrayElements(jUUID, NULL);
    memcpy(carr,hdc.uuid,HDC_UUID_LENGTH);
    jEnv->ReleaseByteArrayElements(jUUID, carr, 0);

    jEnv->SetObjectField(jObj, getUUIDField(jEnv, jObj), jUUID);
}

HDC getHDC(JNIEnv* jEnv, jobject jObj)
{
    auto jUUID = reinterpret_cast<jbyteArray>(jEnv->GetObjectField(jObj, getUUIDField(jEnv, jObj)));
    jlong jStorage = jEnv->GetLongField(jObj, getStorageField(jEnv, jObj));
    hdc_t hdc = {};
    jbyte *carr = jEnv->GetByteArrayElements(jUUID, NULL);
    memcpy(hdc.uuid, carr, HDC_UUID_LENGTH);
    jEnv->ReleaseByteArrayElements(jUUID, carr, 0);
    hdc.storage_id = jStorage;
    return HDC(hdc);
}

jobject newHDC(JNIEnv* jEnv, hdc_t hdc)
{
    jbyteArray jUUID = jEnv->NewByteArray(HDC_UUID_LENGTH);
    jbyte *carr = jEnv->GetByteArrayElements(jUUID, NULL);
    memcpy(carr,hdc.uuid,HDC_UUID_LENGTH);
    long jStorageID = hdc.storage_id;
    jclass hdc_class = jEnv->FindClass("dev/libhdc/HDC");
    jmethodID ctor = jEnv->GetMethodID(hdc_class, "<init>", "([BJ)V");
    assert(ctor);
    jEnv->ReleaseByteArrayElements(jUUID, carr, 0);
    jobject jHDC = jEnv->NewObject(hdc_class, ctor, jUUID, jStorageID);
    return jHDC;
}

template<typename T>
void set_data(JNIEnv* jEnv, jobject jObj, jobject jShape, T* data)
{
    auto hdc = getHDC(jEnv, jObj);

    jclass list_class = jEnv->FindClass("java/util/ArrayList");
    jint jSize = jEnv->CallIntMethod(jShape, jEnv->GetMethodID(list_class, "size", "()I"));

    std::vector<size_t> shape = {};
    shape.reserve(jSize);

    jclass integer_class = jEnv->FindClass("java/lang/Integer");

    for (jint i = 0; i < jSize; ++i) {
        jobject
            element = jEnv->CallObjectMethod(jShape, jEnv->GetMethodID(list_class, "get", "(I)Ljava/lang/Object;"), i);
        jint value = jEnv->CallIntMethod(element, jEnv->GetMethodID(integer_class, "intValue", "()I"));
        shape.push_back(value);
    }

    hdc.set_data(shape, data);
}

} // anon namespace

void Java_dev_libhdc_HDC_add_1child(JNIEnv* jEnv, jobject obj, jstring jPath, jobject child)
{
    auto hdc = getHDC(jEnv, obj);
    std::string path = jEnv->GetStringUTFChars(jPath, nullptr);
    auto child_hdc = getHDC(jEnv, child);
    hdc.add_child(path, child_hdc);
}

jobject Java_dev_libhdc_HDC_get(JNIEnv* jEnv, jobject obj, jstring jPath)
{
    auto hdc = getHDC(jEnv, obj);
    std::string path = jEnv->GetStringUTFChars(jPath, nullptr);
    auto child = hdc.get(path);
    hdc_t child_hdc = child.as_obj();
    return newHDC(jEnv, child_hdc);
}

jobject Java_dev_libhdc_HDC_keys(JNIEnv* jEnv, jobject obj)
{
    auto hdc = getHDC(jEnv, obj);

    auto keys = hdc.keys();

    jclass list_class = jEnv->FindClass("java/util/ArrayList");
    jobject jKeys = jEnv->NewObject(list_class, jEnv->GetMethodID(list_class, "<init>", "()V"));

    for (const auto& key : keys) {
        jstring jKey = jEnv->NewStringUTF(key.c_str());
        jEnv->CallVoidMethod(jKeys, jEnv->GetMethodID(list_class, "add", "(Ljava/lang/Object;)Z"), jKey);
    }

    return jKeys;
}

void Java_dev_libhdc_HDC_delete_1child(JNIEnv* jEnv, jobject obj, jstring jPath)
{
    auto hdc = getHDC(jEnv, obj);
    std::string path = jEnv->GetStringUTFChars(jPath, nullptr);
    hdc.delete_child(path);
}

void Java_dev_libhdc_HDC_delete_1slice(JNIEnv* jEnv, jobject obj, jlong jIndex)
{
    auto hdc = getHDC(jEnv, obj);
    hdc.delete_slice((size_t)jIndex);
}

void Java_dev_libhdc_HDC_set_1data__Ljava_util_ArrayList_2_3I(JNIEnv* jEnv,
                                                              jobject obj,
                                                              jobject jShape,
                                                              jintArray jData)
{
    jint* data = jEnv->GetIntArrayElements(jData, nullptr);
    set_data(jEnv, obj, jShape, data);
}

void Java_dev_libhdc_HDC_set_1data__Ljava_util_ArrayList_2_3J(JNIEnv* jEnv,
                                                              jobject obj,
                                                              jobject jShape,
                                                              jlongArray jData)
{
    jlong* data = jEnv->GetLongArrayElements(jData, nullptr);
    set_data(jEnv, obj, jShape, data);
}

void Java_dev_libhdc_HDC_set_1data__Ljava_util_ArrayList_2_3F(JNIEnv* jEnv,
                                                              jobject obj,
                                                              jobject jShape,
                                                              jfloatArray jData)
{
    jfloat* data = jEnv->GetFloatArrayElements(jData, nullptr);
    set_data(jEnv, obj, jShape, data);
}

void Java_dev_libhdc_HDC_set_1data__Ljava_util_ArrayList_2_3D(JNIEnv* jEnv,
                                                              jobject obj,
                                                              jobject jShape,
                                                              jdoubleArray jData)
{
    jdouble* data = jEnv->GetDoubleArrayElements(jData, nullptr);
    set_data(jEnv, obj, jShape, data);
}

void Java_dev_libhdc_HDC_set_1data__Ljava_util_ArrayList_2_3B(JNIEnv* jEnv,
                                                              jobject obj,
                                                              jobject jShape,
                                                              jbyteArray jData)
{
    jbyte* data = jEnv->GetByteArrayElements(jData, nullptr);
    set_data(jEnv, obj, jShape, data);
}

void Java_dev_libhdc_HDC_set_1data__Ljava_util_ArrayList_2_3S(JNIEnv* jEnv,
                                                              jobject obj,
                                                              jobject jShape,
                                                              jshortArray jData)
{
    jshort* data = jEnv->GetShortArrayElements(jData, nullptr);
    set_data(jEnv, obj, jShape, data);
}

void Java_dev_libhdc_HDC_set_1data__Ljava_util_ArrayList_2_3Z(JNIEnv* jEnv,
                                                              jobject obj,
                                                              jobject jShape,
                                                              jbooleanArray jData)
{
    jboolean* data = jEnv->GetBooleanArrayElements(jData, nullptr);
    set_data(jEnv, obj, jShape, data);
}

void Java_dev_libhdc_HDC_set_1data__D(JNIEnv* jEnv, jobject obj, jdouble data)
{
    auto hdc = getHDC(jEnv, obj);
    hdc.set_data(data);
}

void Java_dev_libhdc_HDC_set_1data__Z(JNIEnv* jEnv, jobject obj, jboolean data)
{
    auto hdc = getHDC(jEnv, obj);
    hdc.set_data(data);
}

void Java_dev_libhdc_HDC_set_1data__B(JNIEnv* jEnv, jobject obj, jbyte data)
{
    auto hdc = getHDC(jEnv, obj);
    hdc.set_data(data);
}

void Java_dev_libhdc_HDC_set_1data__S(JNIEnv* jEnv, jobject obj, jshort data)
{
    auto hdc = getHDC(jEnv, obj);
    hdc.set_data(data);
}

void Java_dev_libhdc_HDC_set_1data__I(JNIEnv* jEnv, jobject obj, jint data)
{
    auto hdc = getHDC(jEnv, obj);
    hdc.set_data(data);
}

void Java_dev_libhdc_HDC_set_1data__J(JNIEnv* jEnv, jobject obj, jlong data)
{
    auto hdc = getHDC(jEnv, obj);
    hdc.set_data(data);
}

void Java_dev_libhdc_HDC_set_1data__F(JNIEnv* jEnv, jobject obj, jfloat data)
{
    auto hdc = getHDC(jEnv, obj);
    hdc.set_data(data);
}

void Java_dev_libhdc_HDC_set_1string(JNIEnv* jEnv, jobject jObj, jstring jStr)
{
    auto hdc = getHDC(jEnv, jObj);
    std::string str = jEnv->GetStringUTFChars(jStr, nullptr);
    hdc.set_string(str);
}

jstring Java_dev_libhdc_HDC_get_1string(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    std::string str = hdc.as_string();
    return jEnv->NewStringUTF(str.c_str());
}


jlong Java_dev_libhdc_HDC_get_1rank(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return hdc.get_rank();
}

jobject Java_dev_libhdc_HDC_get_1shape(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);

    std::vector<size_t> shape = hdc.get_shape();

    jclass list_class = jEnv->FindClass("java/util/ArrayList");
    jobject jShape = jEnv->NewObject(list_class, jEnv->GetMethodID(list_class, "<init>", "()V"));

    jclass integer_class = jEnv->FindClass("java/lang/Integer");

    for (const auto& i : shape) {
        jobject jI = jEnv->NewObject(integer_class, jEnv->GetMethodID(integer_class, "<init>", "(I)V"), i);
        jEnv->CallVoidMethod(jShape, jEnv->GetMethodID(list_class, "add", "(Ljava/lang/Object;)Z"), jI);
    }

    return jShape;
}

void Java_dev_libhdc_HDC_set_1child(JNIEnv* jEnv, jobject jObj, jstring jPath, jobject jChild)
{
    auto hdc = getHDC(jEnv, jObj);
    std::string path = jEnv->GetStringUTFChars(jPath, nullptr);
    auto child = getHDC(jEnv, jChild);

    hdc.set_child(path, child);
}

void Java_dev_libhdc_HDC_append(JNIEnv* jEnv, jobject jObj, jobject jChild)
{
    auto hdc = getHDC(jEnv, jObj);
    auto child = getHDC(jEnv, jChild);

    hdc.append(child);
}

void Java_dev_libhdc_HDC_insert(JNIEnv* jEnv, jobject jObj, jlong jIndex, jobject jChild)
{
    auto hdc = getHDC(jEnv, jObj);
    auto child = getHDC(jEnv, jChild);

    hdc.insert(jIndex,child);
}

jobject Java_dev_libhdc_HDC_get__Ljava_lang_String_2(JNIEnv* jEnv, jobject jObj, jstring jPath)
{
    auto hdc = getHDC(jEnv, jObj);
    std::string path = jEnv->GetStringUTFChars(jPath, nullptr);

    auto child = hdc.get(path);

    return newHDC(jEnv, child.as_obj());
}

jobject Java_dev_libhdc_HDC_get__J(JNIEnv* jEnv, jobject jObj, jlong jIndex)
{
    auto hdc = getHDC(jEnv, jObj);
    auto child = hdc.get(jIndex);
    return newHDC(jEnv, child.as_obj());
}

jboolean Java_dev_libhdc_HDC_exists__Ljava_lang_String_2(JNIEnv* jEnv, jobject jObj, jstring jPath)
{
    auto hdc = getHDC(jEnv, jObj);
    std::string path = jEnv->GetStringUTFChars(jPath, nullptr);
    return hdc.exists(path);
}

jboolean Java_dev_libhdc_HDC_exists__J(JNIEnv* jEnv, jobject jObj, jlong index)
{
    auto hdc = getHDC(jEnv, jObj);
    return hdc.exists((size_t)index);
}

jobject Java_dev_libhdc_HDC_get_1data(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return jEnv->NewDirectByteBuffer(hdc.as_void_ptr(), hdc.get_datasize());
}

jstring Java_dev_libhdc_HDC_serialize(JNIEnv* jEnv, jobject jObj, jstring jProtocol)
{
    auto hdc = getHDC(jEnv, jObj);
    std::string protocol = jEnv->GetStringUTFChars(jProtocol, nullptr);
    auto str = hdc.serialize(protocol);
    return jEnv->NewStringUTF(str.c_str());
}

void Java_dev_libhdc_HDC_print_1info(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    hdc.print_info();
}

jlong Java_dev_libhdc_HDC_get_1type(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return hdc.get_type();
}

jstring Java_dev_libhdc_HDC_get_1type_1str(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    auto str = hdc.get_type_str();
    return jEnv->NewStringUTF(str);
}

jobject Java_dev_libhdc_HDC_load(JNIEnv* jEnv, jclass, jstring jURI)
{
    std::string uri = jEnv->GetStringUTFChars(jURI, nullptr);
    auto hdc = HDC::load(uri);
    return newHDC(jEnv, hdc.as_obj());
}

void Java_dev_libhdc_HDC_create(JNIEnv* jEnv, jobject jObj, jstring jPath)
{
    HDC hdc;
    if (jPath == nullptr) {
        hdc = HDC();
    } else {
        std::string path = jEnv->GetStringUTFChars(jPath, nullptr);
        hdc = HDC(path);
    }
    initHDC(jEnv, jObj, hdc.as_obj());
}

template<typename T>
void create(JNIEnv* jEnv, jobject jObj, jobject jShape, T* data)
{
    HDC hdc;

    jclass list_class = jEnv->FindClass("java/util/ArrayList");
    jint jSize = jEnv->CallIntMethod(jShape, jEnv->GetMethodID(list_class, "size", "()I"));

    std::vector<size_t> shape = {};
    shape.reserve(jSize);

    jclass integer_class = jEnv->FindClass("java/lang/Integer");

    for (jint i = 0; i < jSize; ++i) {
        jobject
            element = jEnv->CallObjectMethod(jShape, jEnv->GetMethodID(list_class, "get", "(I)Ljava/lang/Object;"), i);
        jint value = jEnv->CallIntMethod(element, jEnv->GetMethodID(integer_class, "intValue", "()I"));
        shape.push_back(value);
    }

    hdc.set_data(shape, data);
    initHDC(jEnv, jObj, hdc.as_obj());
}


void Java_dev_libhdc_HDC_create_1double_1array(JNIEnv* jEnv, jobject jObj, jobject jShape, jdoubleArray jData)
{
    jdouble* data = jEnv->GetDoubleArrayElements(jData, nullptr);
    create(jEnv, jObj, jShape, data);
}
void Java_dev_libhdc_HDC_create_1float_1array(JNIEnv* jEnv, jobject jObj, jobject jShape, jfloatArray jData)
{
    jfloat* data = jEnv->GetFloatArrayElements(jData, nullptr);
    create(jEnv, jObj, jShape, data);
}
void Java_dev_libhdc_HDC_create_1long_1array(JNIEnv* jEnv, jobject jObj, jobject jShape, jlongArray jData)
{
    jlong* data = jEnv->GetLongArrayElements(jData, nullptr);
    create(jEnv, jObj, jShape, data);
}
void Java_dev_libhdc_HDC_create_1int_1array(JNIEnv* jEnv, jobject jObj, jobject jShape, jintArray jData)
{
    jint* data = jEnv->GetIntArrayElements(jData, nullptr);
    create(jEnv, jObj, jShape, data);
}
void Java_dev_libhdc_HDC_create_1short_1array(JNIEnv* jEnv, jobject jObj, jobject jShape, jshortArray jData)
{
    jshort* data = jEnv->GetShortArrayElements(jData, nullptr);
    create(jEnv, jObj, jShape, data);
}
void Java_dev_libhdc_HDC_create_1byte_1array(JNIEnv* jEnv, jobject jObj, jobject jShape, jbyteArray jData)
{
    jbyte* data = jEnv->GetByteArrayElements(jData, nullptr);
    create(jEnv, jObj, jShape, data);
}
void Java_dev_libhdc_HDC_create_1boolean_1array(JNIEnv* jEnv, jobject jObj, jobject jShape, jbooleanArray jData)
{
    jboolean* data = jEnv->GetBooleanArrayElements(jData, nullptr);
    create(jEnv, jObj, jShape, data);
}

template<typename T>
void create(JNIEnv* jEnv, jobject jObj, T data)
{
    HDC hdc;
    hdc.set_data(data);
    initHDC(jEnv, jObj, hdc.as_obj());
}
void Java_dev_libhdc_HDC_create_1double(JNIEnv* jEnv, jobject jObj, jdouble jData)
{
    create( jEnv, jObj, jData);
}
void Java_dev_libhdc_HDC_create_1float(JNIEnv* jEnv, jobject jObj, jfloat jData)
{
    create( jEnv, jObj, jData);
}
void Java_dev_libhdc_HDC_create_1long(JNIEnv* jEnv, jobject jObj, jlong jData)
{
    create( jEnv, jObj, jData);
}
void Java_dev_libhdc_HDC_create_1int(JNIEnv* jEnv, jobject jObj, jint jData)
{
    create( jEnv, jObj, jData);
}
void Java_dev_libhdc_HDC_create_1short(JNIEnv* jEnv, jobject jObj, jshort jData)
{
    create( jEnv, jObj, jData);
}
void Java_dev_libhdc_HDC_create_1byte(JNIEnv* jEnv, jobject jObj, jbyte jData)
{
    create( jEnv, jObj, jData);
}
void Java_dev_libhdc_HDC_create_1boolean(JNIEnv* jEnv, jobject jObj, jboolean jData)
{
    create( jEnv, jObj, jData);
}

jboolean Java_dev_libhdc_HDC_as_1bool(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return hdc.as_scalar<bool>();
}
jbyte Java_dev_libhdc_HDC_as_1byte(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return hdc.as_scalar<byte>();
}
jshort Java_dev_libhdc_HDC_as_1short(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return hdc.as_scalar<short>();
}
jlong Java_dev_libhdc_HDC_as_1long(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return hdc.as_scalar<long>();
}
jfloat Java_dev_libhdc_HDC_as_1float(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return hdc.as_scalar<float>();
}
jdouble Java_dev_libhdc_HDC_as_1double(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return hdc.as_scalar<double>();
}
jint Java_dev_libhdc_HDC_as_1int(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return hdc.as_scalar<int32_t>();
}

jobject Java_dev_libhdc_HDC_load__Ljava_lang_String_2Ljava_lang_String_2(JNIEnv* jEnv, jclass, jstring jUri, jstring jDataPath)
{
    std::string uri = jEnv->GetStringUTFChars(jUri, nullptr);
    std::string datapath = jEnv->GetStringUTFChars(jDataPath, nullptr);
    HDC h = HDC::load(uri,datapath);
    return newHDC(jEnv, h.as_obj());
}


void Java_dev_libhdc_HDC_save(JNIEnv* jEnv, jobject jObj, jstring jUri, jstring jDataPath)
{
    auto hdc = getHDC(jEnv, jObj);
    std::string uri = jEnv->GetStringUTFChars(jUri, nullptr);
    std::string datapath = jEnv->GetStringUTFChars(jDataPath, nullptr);
    hdc.save(uri,datapath);
}

void Java_dev_libhdc_HDC_serialize__Ljava_lang_String_2(JNIEnv* jEnv, jclass jObj, jstring jFile)
{
    auto hdc = getHDC(jEnv, jObj);
    std::string file = jEnv->GetStringUTFChars(jFile, nullptr);
    hdc.serialize(file);
}

void Java_dev_libhdc_HDC_init(JNIEnv*, jclass)
{
    HDC::init();
}

void Java_dev_libhdc_HDC_dump(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    hdc.dump();
}
