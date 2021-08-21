cd ffmpeg-4.1.42
export PREFIX=../fflib/pc/

function build_pc
{
   echo -e "\033[32m build start \033[0m"

   ./configure \
       --prefix=$PREFIX \
       --disable-static \
       --enable-shared \
       --disable-doc \
       --disable-yasm \
       --disable-ffmpeg \
       --disable-ffplay \
       --disable-ffprobe \
       --disable-avdevice \
       --disable-doc \
       --disable-symver 

   make clean

   make -j16
   make install

   echo -e "\033[32m build successful \033[0m"
}

build_pc