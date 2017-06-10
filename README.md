### What is pylite?
pylite is a hobby python interpreter, which can tell you how python compiler and virtual machine works. It serves for educational purposes, so the code is kept as simple as possible. 

pylite consists of two executables: pycom and pyvm. pycom is the compiler, which has 5000 LOC. pyvm is the virtual machine, which has 6000 LOC.

### How to build? 
1. build pycom and pyvm 
   + cd pylite && make DEBUG=0
2. add pycom and pyvm to PATH vaiable 
   + PATH=\`pwd\`/com:\`pwd\`/vm:$PATH
3. run tests
   + cd tests && ./run 
4. run a simple HttpServer
   + cd httpd && pyvm server.py
   + open the browser and access http://localhost:8080

### Implemented features:
1. builtin objects: list/dict/tuple/slice/range
2. if/while/for
3. function (closure is not supported)
4. class (only support single inheritance)
5. module
6. exception handling
7. a simple GC

### Known bugs:
1. pylite only use 4 spaces or TAB for indentation 
2. list/dict/tuple literals must be in single line

### pylite介绍
pylite是一个用于编译原理教学的python解释器，通过学习它的源代码可以了解python编译器和虚拟机是如何工作的。

pylite包含两个可执行程序：pycom和pyvm。pycom是编译器，由5000行代码构成。pyvm是虚拟机，由6000行代码构成。

### 如何编译？
1. 编译pycom和pyvm 
   + cd pylite && make DEBUG=0
2. 将pycom和pyvm所在的目录加入PATH环境变量
   + PATH=\`pwd\`/com:\`pwd\`/vm:$PATH
3. 运行测试
   + cd tests && ./run 
4. 运行一个简单的http服务器(综合了大部分语言特性)
   + cd httpd && pyvm server.py
   + 打开浏览器，访问http://localhost:8080

#### 已经实现的特征：
1. 常见的内置对象list/dict/tuple/slice/range
2. 控制流语句if/while/for 
3. 函数(不支持闭包)
4. 类(只支持单继承)
5. 模块
6. 异常处理
7. 垃圾回收

### Known bugs:
1. 在缩进中，只能使用4空格或则TAB
2. 必须在一行完成list/dict/tuple的字面表示，不能跨行
