# ndk环境
cd ffmpeg-4.1.42
export NDK=../android-ndk-r17c
export SYSROOT=$NDK/platforms/android-21/arch-arm
export TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64
CPU=armv7-a

ISYSROOT=$NDK/sysroot
ASM=$ISYSROOT/usr/include/arm-linux-androideabi

# 要保存动态库的目录，这里保存在源码根目录下的android/armv7-a
export PREFIX=../fflib/android/$CPU
ADDI_CFLAGS="-marm"

# 编译配置详解答
# 设置编译针对的系统，网上查到很多资料说编译前需要修改configure文件，设置这项后就不用修改了，系统会自动修改，效果一样。
#--target-os=linux \
# 编译后文件的输出目录
#--prefix=$PREFIX \
#--enable-cross-compile \
# 编译动态库，shared和static的开关是相对的。
#--enable-static \
#--disable-shared \
# 生成文档，以及相关的功能模块
#--disable-doc \
# 禁止编译可执行文件
#--disable-programs
#--disable-ffmpeg \
#--disable-ffplay \
#--disable-ffprobe \
#--disable-avdevice \
#--disable-doc \
#--disable-symver \
# 设置交叉编译器，按照实际路径修改就可以了。
#--cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
# 设置编译器，不然会报错，系统默认会使用arm-linux-androideabi-clang，但是此编译器在NDK不存在，导致编译失败
#--cc=$TOOLCHAIN/bin/arm-linux-androideabi-gcc \
#--nm=$TOOLCHAIN/bin/arm-linux-androideabi-nm \
# 设置编译so库的架构，当前设置为arm，可以根据实际需求修改
#--arch=arm \
# 设置编译针对的平台，可以根据实际需求进行设置，当前设置为最低支持android-21版本，arm架构。
#--sysroot=$SYSROOT \
#--extra-cflags="-I$ASM -isysroot $ISYSROOT -D__ANDROID_API__=21 -U_FILE_OFFSET_BITS -Os -fPIC -DANDROID -Wno-deprecated -mfloat-abi=softfp -marm" \
#--extra-ldflags="$ADDI_LDFLAGS" \

$ADDITIONAL_CONFIGURE_FLAG

function build_android
{
   echo -e "\033[32m build start \033[0m"

   ./configure \
       --target-os=android \
       --prefix=$PREFIX \
       --enable-cross-compile \
       --enable-static \
       --disable-shared \
       --disable-doc \
       --disable-ffmpeg \
       --disable-ffplay \
       --disable-ffprobe \
       --disable-avdevice \
       --disable-doc \
       --disable-symver \
       --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
       --cc=$TOOLCHAIN/bin/arm-linux-androideabi-gcc \
       --nm=$TOOLCHAIN/bin/arm-linux-androideabi-nm \
       --arch=arm \
       --sysroot=$SYSROOT \
       --extra-cflags="-I$ASM -isysroot $ISYSROOT -D__ANDROID_API__=21 -U_FILE_OFFSET_BITS -Os -fPIC -DANDROID -Wno-deprecated -mfloat-abi=softfp -marm" \
       --extra-ldflags="$ADDI_LDFLAGS" \
       $ADDITIONAL_CONFIGURE_FLAG

   make clean

   make -j16
   make install

   echo -e "\033[32m build successful \033[0m"
}

function merge
{
   echo -e "\033[32m package start \033[0m"

   # 打包
   $TOOLCHAIN/bin/arm-linux-androideabi-ld \
       -rpath-link=$SYSROOT/usr/lib \
       -L$SYSROOT/usr/lib \
       -L$PREFIX/lib \
       -soname libffmpeg.so -shared -nostdlib -Bsymbolic --whole-archive --no-undefined -o \
       $PREFIX/libffmpeg.so \
       libavcodec/libavcodec.a \
       libavfilter/libavfilter.a \
       libavformat/libavformat.a \
       libavutil/libavutil.a \
       libswresample/libswresample.a \
       libswscale/libswscale.a \
       -lc -lm -lz -ldl -llog --dynamic-linker=/system/bin/linker \
       $TOOLCHAIN/lib/gcc/arm-linux-androideabi/4.9.x/libgcc.a

   # strip 精简文件
   $TOOLCHAIN/bin/arm-linux-androideabi-strip  $PREFIX/libffmpeg.so

   echo -e "\033[32m package successful \033[0m"
}


build_android
merge