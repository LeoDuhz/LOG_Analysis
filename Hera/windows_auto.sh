#!/bin/bash
OPTIONS="All Lua Protobuf WinPack DLL Quit"
select opt in $OPTIONS; do
	if [ "$opt" = "All" ]; then
		if [ -d "windows_package" ]; then
			rm -rf windows_package
		fi
		scp zjunlict@10.12.225.55:./windows_package.zip ./
		unzip windows_package.zip
		cp -f -r windows_package/* ./
		rm -rf windows_package*
		cp -f 3rdParty/lua/bin/x64/lua5.1.dll ./
		cp -f 3rdParty/lua/bin/x64/lua5.1.dll 3rdParty/tolua++/bin
		cp -f 3rdParty/tolua++/bin/x64/tolua++.dll 3rdParty/tolua++/bin
		cd ../Medusa/src/LuaModule
		../../../ZBin/3rdParty/tolua++/bin/tolua++.exe -n zeus -o lua_zeus.cpp zeus.pkg
		cd ../../share/proto
		if [ ! -d "cpp" ]; then
			mkdir cpp
		fi
		../../../ZBin/3rdParty/protobuf/bin/protoc.exe --cpp_out=cpp *.proto
		exit

	elif [ "$opt" = "Lua" ]; then
		cp -f 3rdParty/lua/bin/x64/lua5.1.dll 3rdParty/tolua++/bin
		cp -f 3rdParty/tolua++/bin/x64/tolua++.dll 3rdParty/tolua++/bin
		cd ../Medusa/src/LuaModule
		../../../ZBin/3rdParty/tolua++/bin/tolua++.exe -n zeus -o lua_zeus.cpp zeus.pkg
		cd ../../../ZBin

	elif [ "$opt" = "Protobuf" ]; then
		cd ../Medusa/share/proto
		if [ ! -d "cpp" ]; then
			mkdir cpp
		fi
		../../../ZBin/3rdParty/protobuf/bin/protoc.exe --cpp_out=cpp *.proto
		cd ../../../ZBin

	elif [ "$opt" = "WinPack" ]; then
		if [ -d "windows_package" ]; then
			rm -rf windows_package
		fi
		scp zjunlict@10.12.225.55:./windows_package.zip ./
		unzip windows_package.zip
		cp -f -r windows_package/* ./
		rm -rf windows_package*

	elif [ "$opt" = "DLL" ]; then
		echo Choose 32 bit or 64bit
		OPTIONS2="32bit 64bit Quit"
		select opt2 in $OPTIONS2; do
			if [ "$opt2" = "32bit" ]; then
				cp -f 3rdParty/lua/bin/x86/lua5.1.dll ./
				exit
			elif [ "$opt2" = "64bit" ]; then
				cp -f 3rdParty/lua/bin/x64/lua5.1.dll ./
				exit
			elif [ "$opt2" = "Quit" ]; then
				exit
			else
				clear
				echo -e "\033[31mBad Option!!! Please enter 1-3\033[0m" 
			fi
		done

	elif [ "$opt" = "Quit" ]; then
		exit

	else
		clear
		echo -e "\033[31mBad Option!!! Please enter 1-6\033[0m" 
	fi
done