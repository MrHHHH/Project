.PHONY:all
all:httpd cgi

httpd:/home/xuhuan/study/Project/HTTP/Http_Multithread/httpd.o /home/xuhuan/study/Project/HTTP/Http_Multithread/main.o 
	g++ -o $@ $^ -lpthread
%.o:%.cpp
	g++ -c $<

.PHONY:cgi
cgi:
	cd cgiOperation; make; make output; cd -

.PHONY:clean
clean:
	rm -rf *.o httpd EduManSys output; cd cgiOperation; make clean; cd -

.PHONY:output
output:all
	mkdir output
	cp httpd output/
	cp -rf log output/
	cp -rf conf output/
	cp -rf wwwRoot output/
	mkdir -p output/wwwRoot/cgiBin
	cp -f EduManSys output/wwwRoot/cgiBin/
	cp -rf /home/xuhuan/study/Project/HTTP/Http_Multithread/cgiOperation/mysql/lib output/wwwRoot/cgiBin
	cp -f httpCtl.sh output/ 
