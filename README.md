# System Programming Lab

This repo contains 6 system programming projects. These projects were main assignments of *'System Programming Laboratory'* class whose original name was *'Software Experiment 2'* in 2017. I revised/refined the codes of 2017 for better efficiency and readability. I worked in Ubuntu 16.04 LTS so I also recommend you to work in similar environment. I hope this repo will help you to study basic unix system programming as it worked for me!

## PA0: Make own "string_sw.h"
### Description
The objectives of this project are understanding inner processes of string manipulation functions in `string.h` and making them by myself. `string_sw.h` contains 19 most commonly used functions for string manipulation. (e.g. `strlen`, `strcpy`, `strcat`) 18 functions except `int2str()` are very similar to same-named functions in `string.h` so you can check information of them in linux man page. And information of `int2str()` is below:  

`char *int2str(char *dest, int num);`  
Get integer `num` and convert it to string. Then save in `dest` and return it. But if `dest` is NULL, dynamically allocate the memory and save string to there. Then return the address of it. If it is not available to dynamically allocate some memory space, return NULL. 

### Practice
There are 4 files in PA0. Escpecially, you can verify all the functions in `string_sw.h` with `test.c`. Just enter `make` and `./test` in shell then you can check the result in stdout.

## PA1: Convert text file formats
### Description
The objectives of this project are doing file I/O in unix and normalizing the text file formats. It firstly checks newline characters in input file to check if it has unix format (LF) or dos format (CRLF). Then if input file has unix format, convert it to dos format. And if input file has dos format, convert it to unix format. Buf if input file has mixed format (LF+CRLF), convert it to closer format. If the number of LF and CRLF are same, convert it to unix format. Converting rules are written in table below:  

||to unix format|to dos format|
|--|--|--|
|Newline character|LF|CRLF|
|TAB character|8 spaces to 1 tab|1 tab to 4 spaces|
|Whitespaces before newline character|Trim|Trim|

*Attention: Converting rule about TAB character is applied only to source file (.c, .h, .java) when converting to unix format. The rule is applied to all files when converting to dos format.*

### Difficulties
Since there are 3 steps to convert, I dynamically allocated spaces several times. It was better to declare char array but it was impossible because the length of input file is not given. So I used `malloc()` several times and I also had to use `free()` several times. The difficult point was that I had to use `free()` differently for each cases (e.g. source file to unix format, normal file to unix format, any file to dos format, no convert). If I free memory that I did not dynamically allocate before, segmentation fault can occur. 

### Practice
There are 6 files and 1 directory. There are 6 example input files in directory named `data`. You can check the result of this project by just entering `make` and `./convert` in shell. Then, result message will be printed and output file whose name is (input file name) + '.out' will be generated. If you want to see characters and backslash escapes in input and output file, use command `od -c <file_name>` in shell. And if you want to experiment with other example input files or your own input files, revise `main.c`. The convert cases and result messages of example input files are below:  

|Input|num(CR)|num(LF)|Convert|
|--|:--:|:--:|--|
|hello1.txt|0|4|to dos format|
|hello2.txt|4|4|to unix format (normal file)|
|hello3.java|2|4|to unix format (source file)|
|hello4.java|2|5|to unix format (source file)|
|hello5.txt|3|5|to dos format|
|hello6.txt|0|0|NO convert|

```
Input:hello1.txt Ouput:hello1.txt.out Size:56
Input:hello2.txt Ouput:hello2.txt.out Size:55
Input:hello3.java Ouput:hello3.java.out Size:39
Input:hello4.java Ouput:hello4.java.out Size:47
Input:hello5.txt Ouput:hello5.txt.out Size:56
Input:hello6.txt Ouput:hello6.txt.out Size:8
```

Here is an example of result of `od -c` command:  
<p align="center">
<img src="/img/pa1.png" alt="PA1" height="40%" width="40%" align="middle">
</p>
