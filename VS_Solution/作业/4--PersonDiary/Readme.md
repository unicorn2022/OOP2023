**作者：董佳昕**

**PTA上的压缩包没有exe，学在浙大上的压缩包有exe**

**可以直接点击code/sample.bat运行测试样例**

# 一、问题描述

**Personal Diary**是一个命令行程序，包含以下四个程序

```tex
pdadd
pdlist []
pdshow
pdremove
```

## 1.1	pdadd程序

向日记文件**dairy.txt**中添加一个实体,包含日记的时间、内容
时间：如果日记的时间与**dairy.txt**中的已有日记重复,则会将原来的日记替换掉
内容：从**stdin**中逐行读取日记内容，直到单独的一个**'.'/EOF**截止

## 1.2	pdlist程序

按照日期的顺序，列出**diary.txt**中的日记实体
输入：如果通过命令行参数，输入了开始日期和结束日期，则只输出在这两个日期之间的日记
输出：**stdout**

## 1.3	pdshow程序

按照输入的日期，列出**diary.txt**中的对应日记实体的内容
输入：**stdin**
输出：**stdout**

## 1.4	pdremove程序

按照输入的日期，删除**diary.txt**中的对应日记
输入：**stdin**
输出：**stdout**
返回值：**0**表示删除成功，**1**表示删除失败

# 二、实现思路

## 2.1	定义的类

### 2.1.1	Date类

#### 成员变量

```c++
int year;  //日记的年份
int month; //日记的月份
int day;   //日记的日子
```

#### 成员函数

```c++
bool input(); 
//从键盘中输入日期,并且给出输入提示
//当输入的日期为-1/-1/-1时,返回false
//否则返回true
bool input(ifstream& fin); 
//从文件流中输入日期
//当输入的日期为-1/-1/-1时,表示整个文件的日记读取完成,返回false
//否则返回true

void output();
//向屏幕中输出日期
void output(ofstream& fout);
//向文件流中输出日期
```

####  重载符号

(1)以年、月、日为第一、二、三关键字进行比较

(2)注意，**-1/-1/-1**默认为最大值

### 2.1.2	Diary类

#### 成员变量

```c++
Date date;				//记录日记的日期
vector<string>content;  //记录日记的内容
```

#### 成员函数

```c++
bool input(); 
//从键盘中输入日记的日期及内容,并且给出输入提示
//当输入的日期为-1/-1/-1时,返回false
//否则返回true
bool input(ifstream& fin); 
//从文件中输入日记的日期及内容
//当输入的日期为-1/-1/-1时,表示整个文件的日记读取完成,返回false
//否则返回true

void output();
//向屏幕中输出日记的日期及内容
void output(ofstream& fout);
//向文件流中输出日记的日期及内容
void outputDate();
//向屏幕中输出日记的日期
Date GetDate();
//获取当前日记的日期
```

####  重载符号

(1)以**date**为唯一关键字进行比较

## 2.2	存储

(1)日记文件为**./code/diary.txt**

(2)按照日期，升序排列所有日记文件

(3)以**-1/-1/-1**为文件结尾

## 2.3	pdadd

(1)用一个**vector< Diary >diary**保存已有的所有日记

(1)首先，从键盘中输入新增日记的信息，放到**vector**数组里面，记为**diary[0]**

(2)然后，从**diary.txt**文件读取所有日记，放到**vector**数组里面

(3)从**diary[1]**开始输出存下的所有日记，分为三种情况

​	(a)**diary[i] = diary[0]**：根据题意，舍弃**diary[i]**，将**diary[0]**输入到文件中

​	(b)**diary[i] > diary[0] && diary[i] < diary[0]**：由于根据日期升序排列，此时要先输出**diary[0]**，后输出**diary[i]**

​	(c)其它情况：直接输出**diary[i]** 

## 2.4	pdlist

(1)与**pdadd**类似，从**diary.txt**中读取所有日记，保存到**diary**数组中

(2)从**diary[0]**开始输出存下的所有日记，由于只需要表示日记实体，因此我们只输出日记的日期

(3)至于输出一定范围内的日记，我们只需要在输出的时候，判断一下**diary[i]**的日期是否在指定的范围内即可

(4)当没有日记输出时，会输出提示信息

## 2.5	pdshow

(1)与**pdlist**类似，从**diary.txt**中读取所有日记，保存到**diary**数组中

(2)从**diary[0]**开始遍历存下的所有日记，如果日期与输入的相同，则输出当前日记的所有内容

(3)当没有日记输出时，会输出提示信息

## 2.6	pdremove

(1)与**pdadd**类似，从**diary.txt**中读取所有日记，保存到**diary**数组中

(2)从**diary[0]**开始输出存下的所有日记，分为两种情况

​	(a)**diary[i]**的日期与给定的日期相同：跳过该条日记

​	(b)否则将**diary[i]**输入到文件中

(3)当待删除的日期不存在时，会输出提示信息

# 三、测试样例

## 3.1	pdadd

### 3.1.1	正常插入

<img src="H:\Temp\Typora\image-20220412202918526.png" alt="image-20220412202918526" style="zoom:80%;" />

插入前：**diary.txt**

<img src="H:\Temp\Typora\image-20220412203004186.png" alt="image-20220412203004186" style="zoom:80%;" />

插入后：**diary.txt**

<img src="H:\Temp\Typora\image-20220412203028707.png" alt="image-20220412203028707" style="zoom:80%;" />

### 3.1.2	插入的日期存在

<img src="H:\Temp\Typora\image-20220412203124564.png" alt="image-20220412203124564" style="zoom:80%;" />

插入前：见**3.1.1插入后**

插入后：

<img src="H:\Temp\Typora\image-20220412203201614.png" alt="image-20220412203201614" style="zoom:80%;" />

## 3.2	pdlist

**diary.txt**见**3.1.2**插入后

### 3.2.1	无参数

<img src="H:\Temp\Typora\image-20220412203243925.png" alt="image-20220412203243925" style="zoom:80%;" />

### 3.2.2	日期范围内有日记

<img src="H:\Temp\Typora\image-20220412203353435.png" alt="image-20220412203353435" style="zoom:80%;" />

### 3.2.3	日期范围内没有日记

<img src="H:\Temp\Typora\image-20220412203419401.png" alt="image-20220412203419401" style="zoom:80%;" />

## 3.3	pdshow

**diary.txt**见**3.1.2**插入后

### 3.2.1	有对应的日记

<img src="H:\Temp\Typora\image-20220412203512975.png" alt="image-20220412203512975" style="zoom:80%;" />

### 3.2.2	没有对应的日记

<img src="H:\Temp\Typora\image-20220412203536473.png" alt="image-20220412203536473" style="zoom:80%;" />

## 3.4	pdremove

### 3.4.1	正常删除

<img src="H:\Temp\Typora\image-20220412203614795.png" alt="image-20220412203614795" style="zoom:80%;" />

删除前：

<img src="H:\Temp\Typora\image-20220412203201614.png" alt="image-20220412203201614" style="zoom:80%;" />

删除后：

<img src="H:\Temp\Typora\image-20220412203637679.png" alt="image-20220412203637679" style="zoom:80%;" />

### 3.4.2	目标日记不存在

<img src="H:\Temp\Typora\image-20220412203710204.png" alt="image-20220412203710204" style="zoom:80%;" />

删除前：

<img src="H:\Temp\Typora\image-20220412203637679.png" alt="image-20220412203637679" style="zoom:80%;" />

删除后：

<img src="H:\Temp\Typora\image-20220412203637679.png" alt="image-20220412203637679" style="zoom:80%;" />
