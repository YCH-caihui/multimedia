rm -rf opencv-3.4.15/build
rm -rf cvlib
mkdir opencv-3.4.15/build
cd opencv-3.4.15/build
cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=../../cvlib/pc ..
make 
make install
