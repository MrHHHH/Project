.PHONY:all
all:httpd cgi

httpd:/home/xuhuan/git/Project/Http/Http_Multithread/httpd.o /home/xuhuan/git/Project/Http/Http_Multithread/main.o 
	gcc -o $@ $^ -lpthread
%.o:%.c
	gcc -c $<

.PHONY:cgi
cgi:
	cd cgiOperation; make; make output; cd -

.PHONY:clean
clean:
	rm -rf *.o httpd EduManSys output ; cd cgiOperation; make clean; cd -

.PHONY:output
output:all
	mkdir output
	cp httpd output/
	cp -rf log output/
	cp -rf conf output/
	cp -rf wwwRoot output/
	mkdir -p output/wwwRoot/cgiBin
	cp -f EduManSys output/wwwRoot/cgiBin/
	cp -rf /home/xuhuan/git/Project/Http/Http_Multithread/cgiOperation/mysql/lib output/wwwRoot/cgiBin
	cp -f httpCtl.sh output/ 
