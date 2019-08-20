# Ubuntu

apt update
apt upgrade
apt install -y build-essential git cmake gfortran libboost-all-dev gfortran libhdf5-dev python3-numpy python3-pytest python3-pip python3-h5py libssl-dev libreadline-dev libcppunit-dev
pip3 install cython
cd /tmp/
git clone https://github.com/yahoo/mdbm
cd mdbm/
sed -i.bak 's|/tmp/install|/usr/local|g' Makefile.base
make
make install

# Centos

yum -y update
yum -y install epel-release.noarch
yum update
yum -y install centos-release-scl
yum -y update
yum -y groupinstall "Development Tools"
yum -y install boost boost-devel cppunit-devel openssl-devel git hdf5-devel readline-devel centos-release-scl devtoolset-7-gcc*
curl "https://bootstrap.pypa.io/get-pip.py" -o "get-pip.py"
python3.6 get-pip.py
pip install pytest h5py cython numpy
cd /tmp/
git clone https://github.com/yahoo/mdbm
cd mdbm/
sed -i.bak 's|/tmp/install|/usr/local|g' Makefile.base
make
make install

# Fedora

yum -y update
yum -y groupinstall "Development Tools"
yum -y install boost boost-devel cppunit-devel openssl-devel git hdf5-devel readline-devel gcc-c++ cmake python3-devel python3-pip python3-numpy python3-h5py python3-pytest python3-Cython
cd /tmp/
git clone https://github.com/yahoo/mdbm
cd mdbm/
sed -i.bak 's|/tmp/install|/usr/local|g;/^PEDANTIC += -Werror/ s/PEDANTIC/#PEDANTIC/g' Makefile.base
make
make install
