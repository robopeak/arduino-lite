This is a template sketch that you may want to make a copy from it.

How to create your own sketch?
--------------------------------
Very Simple!
1) Just copy the template folder and paste it in the same folder with the name you would like it to be.
E.g. copy sketch/template to sketch/mySketch

2) Put any sourcecode inside your sketch's src folder. We support following source code format:
 * .c    //c
 * .cpp  //c plus plus 
 * .S    //assembly code

3) Click build.cmd to compile!
   You will find the compiled result (*.hex) in the /build/output/avr folder

How to upload the hex to AVR?
--------------------------------
just click launchsh.cmd and type : make upload
NOTICE: you should modify the Makefile to set correct AVR type, CPU Frequency, Serial Port, etc in advance.

