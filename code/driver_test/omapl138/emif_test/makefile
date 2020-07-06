libxml=/home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/board-support/libxml2/include 
#libxml=/usr/include/libxml2 
#CPP=g++
target=emif_test xml_test
all:$(target)
objs1=emif_test.o
objs2=xml_test.o
%.o:%.cpp
	$(CPP) -c -I$(libxml) $< -o $@ 
emif_test:$(objs1)
	$(CPP) $^ -o $@  -lstdc++ -lxml2
xml_test:$(objs2)
	$(CPP)  $^ -o $@ -lstdc++ -lxml2


clean:
	rm -fr $(objs1) $(objs2) $(target) 

