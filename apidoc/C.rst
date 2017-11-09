.. C API

HDC C API proposal
==================

.. default-domain:: C

.. type:: hdc_t

	HDC (opaque) object type

.. type:: hdc_rank_t

	Rank data type (uint8_t)

Type macros:
	.. macro:: HDC_EMPTY
	.. macro:: HDC_STRUCT
	.. macro:: HDC_LIST
	.. macro:: HDC_INT8
	.. macro:: HDC_INT16
	.. macro:: HDC_INT32
	.. macro:: HDC_INT64
	.. macro:: HDC_UINT8
	.. macro:: HDC_UINT16
	.. macro:: HDC_UINT32
	.. macro:: HDC_UINT64
	.. macro:: HDC_FLOAT
	.. macro:: HDC_DOUBLE
	.. macro:: HDC_STRING
	.. macro:: HDC_BOOL
	.. macro:: HDC_ERROR

.. type:: enum hdc_type_t

	HDC type enum type

    .. member:: EMPTY_ID    = HDC_EMPTY
    .. member:: STRUCT_ID   = HDC_STRUCT
    .. member:: LIST_ID     = HDC_LIST
    .. member:: INT8_ID     = HDC_INT8
    .. member:: INT16_ID    = HDC_INT16
    .. member:: INT32_ID    = HDC_INT32
    .. member:: INT64_ID    = HDC_INT64
    .. member:: UINT8_ID    = HDC_UINT8
    .. member:: UINT16_ID   = HDC_UINT16
    .. member:: UINT32_ID   = HDC_UINT32
    .. member:: UINT64_ID   = HDC_UINT64
    .. member:: FLOAT_ID    = HDC_FLOAT
    .. member:: DOUBLE_ID   = HDC_DOUBLE
    .. member:: STRING_ID   = HDC_STRING
    .. member:: BOOL_ID     = HDC_BOOL
    .. member:: ERROR_ID    = HDC_ERROR

.. type:: struct hdc_data_t

	HDC data object structure

	.. member:: hdc_type_t dtype
	.. member:: hdc_rank_t rank
	.. member:: size_t* shape
	.. member:: void* data

.. function:: void hdc_init(char* plugin_name, char* settings)

	Init HDC library with a given backend plugin.
	settings are plugin-specific JSON.

.. function:: hdc_t* hdc_new_empty()

   Create a new HDC object
   
.. function:: hdc_t* hdc_get(hdc_t* obj, char* path)

   Return HDC object with a given path.
   
.. function:: hdc_data_t hdc_get_data(hdc_t* obj, char* path)

   Return HDC data struct from the HDC object with a given path.
   
.. function:: bool hdc_exists(hdc_t* obj, char* path)

	Return true if an hdc with the path object exists in the obj

.. function:: <type> hdc_as<type>(hdc_t* obj)

	Return true if an hdc with the path object exists in the obj

.. function:: hdc_rank_t hdc_get_rank(hdc_t* obj)

	Return the rank of an HDC array-type container

.. function:: size_t* hdc_get_size(hdc_t* obj)

	Return the size of an HDC array-type container

.. function:: hdc_set(hdc_t* obj, char* path, hdc_t* data)
	
	Set the target container in obj in the path to the data object.
	Contructs the tree if necessary.
	
.. function:: hdc_set_data(hdc_t* obj, char* path, hdc_data_t* data)
	
	Set the target container in obj in the path to the data object.
	Contructs the tree if necessary.
