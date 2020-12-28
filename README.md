# TFTP.Win
狗屎计网socket实验代码  
一堆傻逼要求，是你只会cpp所以要求学生只能用cpp？  
不瞒你说，我的另一个[项目](https://github.com/Chronostasys/Chronos.P2P)传文件速度是这个cpp版本的300倍。
可靠性、内存占用率都在这个cpp版本之上。
cpp肯定也可以做到这个速度，但是麻烦死。
用cpp写socket真的是想不开折磨自己  
## 项目结构
- TFTP.Win cpp库源码
- TFTP.WinFormApp winform前端，里边用PInvoke引入了cpp的dll，但是为了显示错误信息，最终是直接命令行执行exe的
## 命令例子
首先，cpp项目是个命令行程序。你需要打开cmd或者powershell，
然后cd到它的目录
```bat
cd "c:/path/to/TFTP.Win.exe"
```
然后，在这个目录下启动TFTP服务器
```bat
./TFTP.Win.exe tftp -s port "(69)"
```
这个命令你可以自己改参数，修改服务器的端口一类。  
> 注意：**服务器的文件目录是你执行TFTP.Win.exe的目录**。  

也就是说你可以先  
```bat
cd "c:/path/to/xxx/TFTP.Win.exe"
```
然后  
```bat
./xxx/TFTP.Win.exe tftp -s port "(69)"
```
这样文件目录就是`xxx`文件夹了。  

类似地，你可以用下方命令启动客户端：  
```bat
.\TFTP.Win.exe tftp -c host "(10.12.186.173)" port "(69)" -proto "(udp)" -write "(./file.txt) -encode octet"
```

> 注意，客户端默认保存位置也是你执行TFTP.Win.exe的目录。
> 所以请不要在相同目录下同时运行服务器和客户端


当然，你可以直接启动winForm程序，使用ui来进行客户端的操作。
