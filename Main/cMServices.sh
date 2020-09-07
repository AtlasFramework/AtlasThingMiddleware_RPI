echo "Creating directory to install CppMicroServices .."
mkdir temp
mkdir temp/CMircoServices
cd $PWD/temp/

echo "Getting the CppMicroServices github repository .."
git clone https://github.com/CppMicroServices/CppMicroServices CMircoServices
sed -i 's/-Werror/ /' CMircoServices/CMakeLists.txt       #otherwise the warnings will stop the cmake

echo "building the repository ..."
cmake CMircoServices/
echo "make ..."
sudo make
echo "make install ..."
sudo make install

LD_LIBRARY_PATH=/usr/local/include

export LD_LIBRARY_PATH

sudo ldconfig

# rm -r $PWD
