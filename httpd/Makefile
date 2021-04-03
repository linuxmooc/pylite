run:
	#pyvm server.py -verbose -serial
	pyvm server.py

kill:
	pkill -9 python

static:
	ab -n 2048 -c 10 http://127.0.0.1:8080/index.html

dynamic:
	ab -n 2048 -c 10 'http://127.0.0.1:8080/compute.py?number1=11&number2=22'

clean:
	rm -f *.pyc *.xml *.stackdump core
