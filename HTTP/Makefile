httpd:/home/xuhuan/git/Project/HTTP/httpd.o /home/xuhuan/git/Project/HTTP/main.o 
	g++ -o $@ $^ -lpthread
%.o:%.cpp
	g++ -c $< -o $@

.PHONY:clean
clean:
	rm -f *.o httpd
