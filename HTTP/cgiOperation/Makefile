EduManSys:EducationManagementSys.cpp mysql_op.cpp 
	g++ -o $@ $^ -I /home/xuhuan/study/Project/HTTP/cgiOperation/mysql/include -L /home/xuhuan/study/Project/HTTP/cgiOperation/mysql/lib -lmysqlclient
.PHONY:clean
clean:
	rm -f EduManSys

.PHONY:output
output:
	cp EduManSys ..


#生成可执行文件成功但是不能执行，用ldd查看可执行文件依赖的共享库文件
#设置程序共享库位置，加环境变量
#export LD_LIBRARY_PATH=动态库所在文件夹
