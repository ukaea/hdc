This shows how to build HDC with Anaconda3 based Python, MDBM and UDA support on ITM gateway.

# Source my env
Sourcing this script replaces loading of modules and setting UDA
```bash
. ~g2dfrid/hdc.env
```

# load modules, set paths
```bash
module load gnu/6.1.0 cmake/3.5.2
module load profile/advanced
module load boost/1.61.0--gnu--6.1.0
module load zlib/1.2.8--gnu--6.1.0 szip/2.1--gnu--6.1.0 hdf5/1.8.17--gnu--6.1.0
module load netcdf
export BOOST_ROOT=$BOOST_HOME
export HDF5_ROOT=$HDF5_HOME

source /marconi_work/eufus_gw/work/g2jurban/miniconda3/etc/profile.d/conda.sh
conda activate base
```
# set UDA stuff
```bash
source ~g2jhollo/setup.env
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:~g2jhollo/uda/etc/

export UDA_LOG=$HOME
export UDA_HOME=/afs/eufus.eu/user/g/g2jhollo/uda
export UDA_LOG_MODE=a
export UDA_PLUGIN_CONFIG=$UDA_HOME/etc/plugins/udaPlugins.conf
export UDA_SARRAY_CONFIG=$UDA_HOME/etc/udagenstruct.conf
export UDA_LOG_LEVEL=DEBUG
export UDA_PLUGIN_DIR=$UDA_HOME/lib/plugins
export UDA_EXP2IMAS_MAPPING_FILE_DIRECTORY=$UDA_HOME/source/plugins/exp2imas/mappings
export UDA_PLUGIN_DEBUG_SINGLEFILE=1

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:~g2jhollo/uda/etc/
```
# obtain the sources
```bash
git clone https://d_fridrich@bitbucket.org/compass-tokamak/hdc.git
cd hdc
git checkout develop
```
# configure and build 

HDC will be installed into hdc/install
```bash
mkdir build
cd build
cmake -DPYTHON_INCLUDE_DIR=/marconi_work/eufus_gw/work/g2jurban/miniconda3/include/python3.6m/ \
-DPYTHON_LIBRARY=/marconi_work/eufus_gw/work/g2jurban/miniconda3/lib/libpython3.6m.so.1.0 \
-DCYTHON_EXECUTABLE=/marconi_work/eufus_gw/work/g2jurban/miniconda3/bin/cython \
-DCMAKE_INSTALL_PREFIX=$PWD/../install -DMDBM_INCLUDE_DIR=/afs/eufus.eu/user/g/g2dfrid/projects/mdbm/include \
-DMDBM_LIBRARY=/afs/eufus.eu/user/g/g2dfrid/projects/mdbm/lib64/libmdbm.so -DENABLE_UDA=ON ..

make -j
make install
```

