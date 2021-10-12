#!/usr/bin/env sh

set -e

GCC_VERSION=11.2.0
BINUTILS_VERSION=2.37

TMPDIR=`pwd`/toolchain/tmp

PREFIX=`pwd`/toolchain
TARGET=i686-elf
PATH="$PREFIX/bin:$PATH"

type ${TARGET}-gcc && exit 0

sudo apt install -y \
	build-essential \
	bison \
	flex \
	libgmp3-dev \
	libmpc-dev \
	libmpfr-dev \
	texinfo \
	curl


[ ! -d $TMPDIR ] && mkdir -p $TMPDIR
cd $TMPDIR

curl -LO https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.xz

tar -xf binutils-${BINUTILS_VERSION}.tar.xz

[ ! -d ./build-binutils ] && mkdir build-binutils
cd build-binutils

../binutils-${BINUTILS_VERSION}/configure \
	--target=$TARGET \
	--prefix="$PREFIX" \
	--with-sysroot \
	--disable-nls \
	--disable-werror
make
make install


cd $TMPDIR
curl -LO https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz

tar -xf gcc-${GCC_VERSION}.tar.xz

type $TARGET-as || (echo "$TARGET-as is not in PATH" && exit 1)

[ ! -d ./build-gcc ] && mkdir build-gcc
cd build-gcc

../gcc-${GCC_VERSION}/configure \
	--target=$TARGET \
	--prefix="$PREFIX" \
	--disable-nls \
	--enable-languages=c \
	--without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

cd ../../
rm -rf ${TMPDIR}
