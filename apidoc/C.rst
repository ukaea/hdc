.. C API

HDC C API
============

.. default-domain:: C

.. type:: hdc_t

	HDC (opaque) object type

.. type:: hdc_data_t

	HDC data object structure

	.. member:: hdc_type_d dtype
	.. member:: unsigned short rank
	.. member:: size_t* shape
	.. member:: void* data

.. function:: hdc_t* hdc_new_empty()

   Create a new HDC object.
   
.. function:: bool hdc_exists(struct hdc_t* tree, char* path)

	Return true if an hdc with the path object exists in the tree.
