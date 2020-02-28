#make file for Go_CPP
#author: Ryan Strotman


compiler = g++
target = go

all:$(target)

$(target): $(target)
	$(compiler) -o $(target) $(target).cpp

clean:
	rm $(target)
