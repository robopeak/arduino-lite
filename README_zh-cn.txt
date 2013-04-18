欢迎使用Arduino-Lite, 这是一个用于AVR设备的轻量级且高效的运行库。
Arduino-Lite是基于Arduino项目的工作，他的网站是www.arduino.cc

Arduino-Lite是RoboPeak机器人团队(www.RoboPeak.com)内部开发使用的运行库，
我们很荣幸将这个运行库分享给您使用。

本项目网站:       http://code.google.com/p/arduino-lite
RoboPeak团队网站: http://www.robopeak.com

-------------------------
 首次使用
-------------------------
当首次在计算机上使用该运行库时(刚从SVN上check-out或是刚从下载的软件包中解
压缩)，请完成以下步骤

 a. 确保当前的操作系统是被Arduino-Lite支持的 (详见 支持的平台)
 b. 在软件包的根目录下执行 buildenv.cmd 批注处理脚本
 c. 当该脚本完成执行后，Arduino-Lite就可以使用了

Arduino-Lite是自我包含的，这意味着您无需在系统中安装任何额外的软件包和库。
(是的，WINAVR也已经包含在Arduino-Lite中了)。

-------------------------
 支持的平台
-------------------------
Arduino-Lite库源代码是可以在所有平台上使用的(Linux, MacOS, Win32)，但是它的
编译系统目前仅仅支持Win32平台。

目前以下Windows平台已经通过验证：
<> Windows XP 32位版本
<> Windows Vista 32位 和 64位 版本
<> Windows 7 32位 和 64位 版本

-------------------------
 创建一个新工程
-------------------------
基于Arduino-Lite的工程可以直接从位于<Arduino-lite home>/sketch/template.zip
的模板创建。

请按照如下步骤创建您的工程:
1) 将template.zip解压缩到目录<Arduino-lite home>/sketch
2) 请确保解压缩后的目录结构是这样的：
   <Arduino-lite home>/sketch/template/build.cmd
3) 将template文件夹重命名成你所希望的工程名字，例如: helloAVR
4) 到这里，一个新的工程已经创建

-------------------------
 编译一个工程
-------------------------
1) 进入工程文件夹(例如:helloAVR)，执行目录中的批处理脚本launchsh.cmd
2) 在命令行终端下键入: make <回车>
3) 此时将开始对当前工程进行编译，如果一切顺利，您可以在
<project_folder>/build/目录下找到hex/elf文件。

-------------------------
 哪里有文档和手册?
-------------------------
相关的使用手册和文档请访问下面的链接:

  Arduino-Lite特点和原理介绍:
         http://www.robopeak.net/blog/?p=42
  Arduino-Lite的获取和使用:
         http://www.robopeak.net/blog/?p=70
  Arduino-Lite函数手册和开发参考:
         http://www.robopeak.net/blog/?p=107
  项目主页: 
         http://code.google.com/p/arduino-lite

另外，您可以在RoboPeak团队网站(http://www.robopeak.com)中找到额外的有用信息。

Arduino-Lite和官方的Arduino库很相似，您会发现很多函数调用方式都是相近的。
因此，Arduino官方网站中的文档也很有用。

---------------------------
 可用的make命令
---------------------------
编译相关:
<> make/make all
   编译当前工程
<> make clean
   清除上次编译的结果

芯片下载/编程
<> make upload
   将hex经过串口上传到目标AVR芯片内(通过STK500v2协议的bootloader)
<> make usbupload
   将hex经过USB上传到目标AVR芯片内(通过HidBootloader)
<> make burn
   将hex通过RoboPeak USB Connector编程器(或者Avr-doper兼容设备)烧写到目标AVR芯片
<> make erase
   通过RoboPeak USB Connector编程器(或者Avr-doper兼容设备)擦除目标AVR芯片的数据
<> make fuse
   通过RoboPeak USB Connector编程器(或者Avr-doper兼容设备)给目标芯片配置熔丝位
<> make lock
   通过RoboPeak USB Connector编程器(或者Avr-doper兼容设备)给目标芯片配置Lock bit

代码分析和调试
<> make dump
   将编译产生的elf(*.elf)文件反汇编至AVR汇编代码
<> make dumpobj
   将编译产生的目标文件(*.o)反汇编至AVR汇编代码


---------------------------
 联系作者
---------------------------
本项目的初始开发人员是Shikai Chen (www.csksoft.net)，如有疑问，请E-mail:
 csk@live.com