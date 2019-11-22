function has
{
	if which $1 > /dev/null ; then
		return 0
	else
		return 1
	fi
}

#readonly dir_xlnx_install=/proj/xbuilds/2018.2_daily_latest/installs/lin64/Vivado/HEAD
#readonly vivado_settings="$dir_xlnx_install/settings64.sh"

readonly dir_xlnx_install=/media/workspace1/SDSoC_2019.1/Vivado/2019.1
readonly vivado_settings="$dir_xlnx_install/settings64.sh"

source "$vivado_settings"

# linaro
cross_compile_32=arm-linux-gnueabihf-

# codesourcery
if ! has "${cross_compile_32}gcc" ; then
	cross_compile_32=arm-none-linux-gnueabi-
fi

# debian package
if ! has "${cross_compile_32}gcc" ; then
	cross_compile_32=arm-linux-gnueabi-
fi

# panic ?

readonly cross_compile_64=aarch64-linux-gnu-
readonly MAKE="make -j`nproc`"
