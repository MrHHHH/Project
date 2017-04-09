#!/bin/bash

#here文档，自动生成Makefile文件

#httpd
LOCAL_PATH=$(pwd)
SRC=$(ls ${LOCAL_PATH}/*.cpp | tr '\n' ' ')
OBJ=$(ls ${LOCAL_PATH}/*.cpp | sed 's/\.cpp/\.o/g' | tr '\n' ' ')
SERVER_BIN=httpd
CC=g++
LIB="-lpthread"

#cgi
CGI_PATH=${LOCAL_PATH}/cgiOperation/
CGI_SRC=$(ls $CGI_PATH | grep -E '.cpp$')
CGI_BIN=cgiOp

#cgi的Makefile
cat << EOF > ${CGI_PATH}/Makefile
${CGI_BIN}:${CGI_SRC}
	${CC} -o \$@ \$^
.PHONY:clean
clean:
	rm -f ${CGI_BIN}

.PHONY:output
output:
	cp $CGI_BIN ..
EOF


#主Makefile生成httpd
cat <<EOF > Makefile
.PHONY:all
all:${SERVER_BIN} cgi

${SERVER_BIN}:${OBJ}
	${CC} -o \$@ \$^ ${LIB}
%.o:%.cpp
	${CC} -c \$<

.PHONY:cgi
cgi:
	cd cgiOperation; make; make output; cd -

.PHONY:clean
clean:
	rm -rf *.o ${SERVER_BIN} ${CGI_BIN} output; cd cgiOperation; make clean; cd -

.PHONY:output
output:all
	mkdir output
	cp ${SERVER_BIN} output/
	cp -rf log output/
	cp -rf conf output/
	cp -rf wwwRoot output/
	mkdir -p output/wwwRoot/cgiBin
	cp -f $CGI_BIN output/wwwRoot/cgiBin/
	cp -f httpCtl.sh output/ 
EOF

