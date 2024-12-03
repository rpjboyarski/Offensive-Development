CSC=mono-csc
INCLUDES=Includes
SRC_DIR=src
SRC=${SRC_DIR}/Program.cs ${SRC_DIR}/DInvoke.cs ${SRC_DIR}/Win32.cs
CSC_FLAGS=/platform:x64 -unsafe

i-am-really-stupid:
	${CSC} ${SRC} ${CSC_FLAGS} -out:bin/Program.exe
