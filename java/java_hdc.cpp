#include "dev_libhdc_HDC.h"

#include <hdc.hpp>

namespace
{

jfieldID getUUIDField(JNIEnv* jEnv, jobject jObj)
{
    jclass hdc_class = jEnv->GetObjectClass(jObj);
    return jEnv->GetFieldID(hdc_class, "nativeUUID", "Ljava/lang/String;");
}

jfieldID getStorageField(JNIEnv* jEnv, jobject jObj)
{
    jclass hdc_class = jEnv->GetObjectClass(jObj);
    return jEnv->GetFieldID(hdc_class, "nativeStorageID", "J");
}

void initHDC(JNIEnv* jEnv, jobject jObj, const hdc_t& hdc)
{
    jEnv->SetLongField(jObj, getStorageField(jEnv, jObj), hdc.storage_id);

    jstring jUUID = jEnv->NewStringUTF(hdc.uuid);
    jEnv->SetObjectField(jObj, getUUIDField(jEnv, jObj), jUUID);
}

HDC getHDC(JNIEnv* jEnv, jobject jObj)
{
    auto jUUID = reinterpret_cast<jstring>(jEnv->GetObjectField(jObj, getUUIDField(jEnv, jObj)));
    jlong jStorage = jEnv->GetLongField(jObj, getStorageField(jEnv, jObj));

    const char* uuid = jEnv->GetStringUTFChars(jUUID, nullptr);

    hdc_t hdc = {};
    strncpy(hdc.uuid, uuid, HDC_UUID_LENGTH);
    hdc.storage_id = jStorage;

    return HDC(hdc);
}

jobject newHDC(JNIEnv* jEnv, hdc_t hdc)
{
    jstring jUUID = jEnv->NewStringUTF(hdc.uuid);
    long jStorageID = hdc.storage_id;

    jclass hdc_class = jEnv->FindClass("dev/libhdc/HDC");
    jmethodID ctor = jEnv->GetMethodID(hdc_class, "<init>", "(Ljava/lang/String;J)V");
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

jboolean Java_dev_libhdc_HDC_exists(JNIEnv* jEnv, jobject jObj, jstring jPath)
{
    auto hdc = getHDC(jEnv, jObj);
    std::string path = jEnv->GetStringUTFChars(jPath, nullptr);
    return hdc.exists(path);
}

jobject Java_dev_libhdc_HDC_get_1data(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    return jEnv->NewDirectByteBuffer(hdc.as_void_ptr(), hdc.get_datasize());
}

jstring Java_dev_libhdc_HDC_serialize__(JNIEnv* jEnv, jobject jObj)
{
    auto hdc = getHDC(jEnv, jObj);
    auto str = hdc.serialize();
    return jEnv->NewStringUTF(str.c_str());
}

jstring Java_dev_libhdc_HDC_to_1json_1string(JNIEnv* jEnv, jobject jObj, jint jMode)
{
    auto hdc = getHDC(jEnv, jObj);
    auto str = hdc.to_json_string(jMode);
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

void Java_dev_libhdc_HDC_to_1json(JNIEnv* jEnv, jobject jObj, jstring jFile, jint jMode)
{
    auto hdc = getHDC(jEnv, jObj);
    std::string file = jEnv->GetStringUTFChars(jFile, nullptr);
    hdc.to_json(file, jMode);
}

void Java_dev_libhdc_HDC_serialize__Ljava_lang_String_2(JNIEnv* jEnv, jobject jObj, jstring jFile)
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