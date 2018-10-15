# Ubuntu

apt update
apt upgrade
apt install -y build-essential git cmake gfortran libboost-all-dev gfortran libhdf5-dev python3-numpy python3-pytest python3-pip python3-h5py libssl-dev libreadline-dev libcppunit-dev nano
pip3 install cython
cd /tmp/
git clone https://github.com/yahoo/mdbm
cd mdbm/
apt install nano
nano Makefile.base #change PREFIX
make
make install

# Centos

yum update
yum -y upgrade
yum -y install epel-release.noarch
yum update
yum -y install centos-release-scl
yum update
yum -y  groupinstall "Development Tools"
yum -y install boost boost-devel cppunit-devel openssl-devel git nano hdf5-devel python36-numpy readline-devel centos-release-scl devtoolset-7-gcc*
curl "https://bootstrap.pypa.io/get-pip.py" -o "get-pip.py"
python3.6 get-pip.py
pip install pytest h5py cython
cd /tmp/
git clone https://github.com/yahoo/mdbm
cd mdbm/
apt install nano
nano Makefile.base #change PREFIX
make
make install
