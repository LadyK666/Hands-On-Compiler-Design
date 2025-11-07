# README
这是知乎文章-编译原理实战：基于 Flex+Bison 的解释器构建与运行原理剖析的配套代码
## how to run

### 下载 MSYS2
https://www.msys2.org/

打开页面，下载 msys2-x86_64-xxxx.exe 安装程序。

安装后打开 MSYS2 MSYS 终端

--- 

### 更新包管理器

输入：

pacman -Syu


（执行后会提示关闭终端，再重新打开）

安装工具链（含 gcc, make, flex, bison）

pacman -S make gcc flex bison


测试是否成功

flex --version
bison --version
gcc --version
make --version


如果都能显示版本号，说明安装成功

---

### 编译运行
在 MSYS2 终端中进入你的项目文件夹（一共三个文件夹，任意哪一个操作都相同），执行：

make

这会使得出现parser可执行程序

你可以使用以下格式来使用它：

parser < test1.txt


祝你学的开心

