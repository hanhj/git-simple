#####Python程序设计

# 1 Python语法
python的帮助:进入python后help(),进入help提示,此时输入想要的模块名.  
输入modules可以列出所有模块名.  
或者:import xx模块,然后help(xx)

## 1.1 python安装:
https://python123.io/download下载windows,linux,macos版本  
pip是python软件包管理器   
可利用的pip源有  
阿里云:https://mirrors.aliyun.com/pypi/simple  
中国科大:https://pypi.mirrors.ustc.edu.cn/simple  
豆瓣:https://pypi.douban.com/simple  
清华:https://pypi.tuna.tsinghua.edu.cn/simple   
<br\>

pip install -i https://pypi.mirrors.ustc.edu.cn/simple xxx

## 1.2[一个温度转换的简单例子](temperature_trans.py)

	#一个简单的温度转换例子程序
	#华氏温度和摄氏温度转换
	#华氏温度将水冰点温度设为32度,沸点温度为212度
	#摄氏温度将水冰点温度设为0度,沸点温度设为100度
	#1摄氏度=1.8华氏温度,1华氏温度=5/9摄氏度 
	#c=(f-32)/1.8,f=c*1.8+32
	tmp=input("请输入温度,xxF表示华氏温度 xxC表示摄氏温度:")
	if tmp[-1] in [ 'f' ,'F']:
	    f=eval(tmp[0:-1])
	    c=(f-32)/1.8
	    print("转换为摄氏温度{:.2f}C".format(c))
	elif tmp[-1] in ['c','C']:
	    c=eval(tmp[0:-1])
	    f=c*1.8+32
	    print("转换为华氏温度{:.2f}F".format(f))
	else:
	    print("input is error")

## 1.3 计算机的IPO(input process output) 
为了在计算机上解决一个现实问题通常需要经过(IPO)以下几个步骤.

1. 问题是什么?  
	即对问题的描述,分析其要求,解决过程是什么?问题实质是什么?关键算法是什么?   
	对于本例,要求是要将华氏温度与摄氏温度相互转换.为了解决这个问题,我们就需要了解华氏温度与摄氏温度的定义,相互之间的关系,从而建立一个数学表达式来完成这种转换.在现实中,有时问题并不能这么直接的看出解决方法,有时是个数学问题,物理问题,化学问题等等需要专业领域方面的知识,而且还需要将这方面的专业知识通过数学方法表达出来,才能够在计算机上实现.
2.	输入是什么?    
	任何问题都需要一个输入才能驱动,需要对输入的数据进行定义,格式描述.在本例中在数字尾部添加一个F或C的字符来表示不同的温度.
3. 输出是什么?  
	在通过过程解决问题之后,还往往需要将结果输出出来,此时需要对输出数据进行定义,输出形式进行定义.比如在本例中,就将数据格式化为字符,然后在终端上显示出来.如果需要网络发布数据,就需要通过网页的形式展示出来.

## 1.4 Python基本语法
- 缩进  
	Python默认用四个空格表示一个缩进,用缩进表示程序的层次关系
- 变量命名  
	Python可以用字母,数字,下划线构成变量命名,用"="对变量赋值  
	变量首字符不能为数字,变量名是大小写敏感的.
- 保留字  

	|a|b|c|d|e|
	|----|----|----|----|----|
	|and|elif|import|raise|global|
	|as|elif|in|return|noolocal|
	|assert|except|is|try|True|
	|break|finally|lambda|while|False|
	|class|for|not|with|None|
	|continue|from|or|yield||
	|def|if|pass|del||

在1.2的例子程序中,我们用到了eval函数,print函数,input函数  

	eval(str)
		eval函数用来执行一个字符串,参变量是一个字符串,eval将执行该字符串
	input(prompt)
		input函数用来从控制台输入一个字串,参变量是一个字符串,用来提示
	print(str)
		print函数用来向控制台输出一个字串,可以用来格式化输出.
		{:.3f}是一个槽,用来对应后面的变量,表示将其格式化的形式.

# 2 Python数据类型
Python有三种数据类型:  

1.	数字 如90,-1,2.2
2.	字符 "xxx",或'xxx'
	用[index]来索引其中字符,第一个字符为[0],最后一个[-1],其中一段[m,n],[0,-1]表示除了最后一个字符的字串
3.	列表 [xx,xx,...]
	用[index]来索引其中内容,用in [xx,xx]表示判断是否有列表中所列出的内容

	

# 3 Python程序控制
## 3.1 Python语言在计算机发展历史中的位置:
- 计算机发展的历程可以分成  

|年代|时代特点|诞生的语言|
|---------|-----------------------|-----------------------|
|40年代-80年代|计算机结构时代,应对的是解决计算能力的问题|c语言,关注计算机本身问题|
|80年代-2008|视窗和网络时代,解决的是人机交互,机器之间交互的问题|java,解决跨平台,计算机之间的交互问题;c++,理解主客体关系,面向对象,适合大型程序;VB适合桌面程序,解决人机交互问题.Python理解问题本身求解,适合多种问题|
|2008-2017年|复杂信息时代,解决的是数据问题.标志性事件是08年安卓系统诞生,标志由PC时代向移动时代转变,云计算,网络安全,大数据.|Python得到发展.css,html,js前端页面.R/matlab/Go语言-适合特定领域|
|2016年-|人工智能时代,解决人类自身问题,计算机替代人类从事基础性工作||

- Python语言的特点:
	- 语法简洁.
	- 开源,使用第三方库,生态高产.
	- c规c,python规python.c更适合底层问题,接口问题.

# 4 Python函数
## 4.1 turtle
一个蟒蛇绘制程序 

	#draw a python
	import turtle
	turtle.setup(650,350,200.200)
	turtle.penup()
	turtle.fd(-250)
	turtle.pendown()
	turtle.pensize(25)
	turtle.pencolor("purple")
	turtle.seth(-40)
	for i in range(4):
	    turtle.circle(40,80)
	    turtle.circle(-40,80)
	turtle.circle(40,80/2)
	turtle.fd(40)
	turtle.fd(40*2/3)
	turtle.done()

	import turtle 
		引入turtle
	setup(width,height,startx,starty) 
		设置窗口
	seth(angle)	
		改变海龟方向,angle角度单位°
	fd(len)	
		行进len
	penup()	pu()
		抬起画笔
	pencolor("str"),pencolor(r,g,b)
		设置颜色


import 引入库的三种形式:  

	import 库名
	库名.函数(参数)

	from 库名 import *
	函数(参数)

	import 库名 as 别名
	别名.函数(参数)

	
	

中国大学mooc,北京理工大学  
https://www.icourse163.org/learn/BIT-268001?tid=1450323450#/learn/announce

