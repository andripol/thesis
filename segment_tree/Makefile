CPPFLAGS = -g -O3 -Wall -Wextra -Werror

TARGET1 = segment_tree
SOURCE1 = segment_tree.cpp
OBJECT_CODE1 = segment_tree.o

$(TARGET1): $(OBJECT_CODE1)
	g++ -std=c++11 $(CPPFLAGS) -o $(TARGET1) $(OBJECT_CODE1)

$(OBJECT_CODE1): $(SOURCE1)
	g++ -std=c++11 $(CPPFLAGS) -c $(SOURCE1) 

TARGET2 = dummy_segment_sums
SOURCE2 = dummy_segment_sums.cpp
OBJECT_CODE2 = dummy_segment_sums.o

$(TARGET2): $(OBJECT_CODE2)
	g++ -std=c++11 $(CPPFLAGS) -o $(TARGET2) $(OBJECT_CODE2)

$(OBJECT_CODE2): $(SOURCE2)
	g++ -std=c++11 $(CPPFLAGS) -c $(SOURCE2) 

all: $(TARGET1) $(TARGET2)

clean:
	rm $(TARGET1) *.o 

clean_all:
	rm $(TARGET1) $(TARGET2) *.o 
