export DEBUG

all:
	make -C com && make -C vm

clean:
	make clean -C com && make clean -C vm

back:
	cd .. && tar zcvf pylite-`date +%Y-%m%d`.tgz pylite
