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
