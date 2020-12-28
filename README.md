# TFTP.Win

## LICENSE
[GPL 3.0协议](LICENSE)。本项目允许任何人对源码进行任何修改，
但是任何不可以将源代码会修改后的版本用于**盈利**
## 一点小建议
如果想用这个代码过实验，请把命令替换一下，
代码函数改一下名。否则这东西特征有点明显。  
你只需要把`main.cpp`里的parser函数中的字符串改了，
调用命令就会变。
## 功能
完成了socket实验的所有要求，包括但不限于：
- 文件log
- 报错显示
- 前端ui
- 文件传输
- 可靠udp
- octet和netascii自动切换行尾

这个项目里实现的可靠udp算法已经得到实验证实。**所有**
文件传输ap都i可以**同时抵抗**陈凯发的那个软件制造的四种攻击：  
- 延时
- 丢包
- 重复包
- 乱序
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
cd "c:\path\to\xxx\TFTP.Win.exe"
```
然后  
```bat
.\xxx\TFTP.Win.exe tftp -s port "(69)"
```
这样文件目录就是`xxx`文件夹了。  

类似地，你可以用下方命令启动客户端：  
```bat
.\TFTP.Win.exe tftp -c host "(10.12.186.173)" port "(69)" -proto "(udp)" -write "(./file.txt) -encode octet"
```

> 注意，客户端默认保存位置也是你执行TFTP.Win.exe的目录。
> 所以请不要在相同目录下同时运行服务器和客户端


当然，你可以直接启动winForm程序，使用ui来进行客户端的操作。

## bug
如果连接目标服务器不存在，客户端会一直尝试连接而卡死。
我没有设置超时，因为你发现有问题了可以直接
ctrl + c退出。给老师检查的时候注意
不要搞错服务器ip和端口，老师一般不会发现这个问题的。
我懒得改这个了。
