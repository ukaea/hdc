
# coding: utf-8

# # Calling Fortran via HDC
# 
# *Important*: ctypes must be able to find `libhdc_fortran_module.so`. The easiest way is to include the path in `LD_LIBRARY_PATH`. If running from the `examples` dir and if HDC is installed into `../install`, this can be used:
# 
# ```bash
# LD_LIBRARY_PATH=$PWD/../install/lib/ jupyter notebook
# ```
# 

# In[1]:


import numpy as np


# In[2]:


# import the base HDC class
from pyhdc import HDC


# In[3]:


# import fortran function wrapper
from hdc_fortran_module import test_cpos


# First, we'll construct the input "equilibrium" container.

# In[4]:


equilibrium = HDC()
equilibrium['profiles_1d/psi'] = np.linspace(0, 1, 5)
equilibrium['time'] = np.array(2.34)


# Display the equilibrium using the dump method.

# In[5]:


print(equilibrium.dumps())


# Print nodes using `numpy.asarray`

# In[6]:


print("equilibrium['time'] -> {}".format(np.asarray(equilibrium['time'])))
print("equilibrium['profiles_1d/psi'] -> {}".format(np.asarray(equilibrium['profiles_1d/psi'])))


# Call a Fortran function that inputs and outputs a container. The interface function is as simple as this:
# 
# ```python
# def test_cpos(equilibriumin):
#     
#     # load the Fortran library
#     fm = ctypes.cdll.LoadLibrary('libhdc_fortran_module.so')
# 
#     # in the Python / C wrapper, the output tree must be constructed
#     tree = HDC()
#     # HDC is converted implicetely to C/Fortran void pointer in a ctypes call
#     fm.test_cpos_f2c(equilibriumin, tree)
# 
#     return tree
# ```
# 
# The Fortran subroutine interface looks like this:
# 
# ```Fortran
# subroutine test_cpos_f2c(equilibriumin, tree_out) bind(c, name="test_cpos_f2c")
#     use hdc_fortran
#     use iso_c_binding
# 
#     implicit none
# 
#     type(hdc_t), value :: equilibriumin, tree_out
#     type(hdc_t) :: distsourceout
#     
#     ! process the data and put outputs into tree_out
# 
# end subroutine
# ```

# In[7]:


tree_out = test_cpos(equilibrium)


# Visually check the results

# In[8]:


print(tree_out.to_python())


# Check the (side) effect of the fortran function:

# In[9]:


assert np.all(np.asarray(tree_out['distsourceout/source/profiles_1d/psi'] == 
              -3 * np.asarray(equilibrium['profiles_1d/psi'])))

