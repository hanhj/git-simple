#mathematics study

## 1 核心语言 
### 1.1 通用

内置函数首字母大写，以区分用户定义函数。比如Integrate。  
变量用[]括起，比如Sqrt[x]   
{}表示集合  
定义函数时，变量名用\_.比如fun[x\_,y\_]
执行语句用shift+enter
	
### 1.2 列表操作
#### 1.2.1 构造列表
- **Range用法**
		
	-	Range[xmax]  
		产生1,2.。xmax个数

	- Range[xmin,imax]   
		产生xmin到imax之间的数，间隔为1。

	- Range[xmin,xmax,di]  
		产生xmin到xmax之间的数，间隔为di

- **Table用法：**

	- Table[expr,{xmax}]  
		生成xmax个expr表达式集合,例如
	
			In[27]:= Table[i^3, {3}]
			Out[27]= {i^3, i^3, i^3}
		
	- Table[expr,{x,xmax}]  
		自变量为x，从1到xmax，间隔1
		
			In[28]:= Table[i^3, {i, 3}]
			Out[28]= {1, 8, 27}

	- Table[expr,{x,xmin,xmax}]  
		自变量为x，从min到max，间隔1
		
	- Table[expr,{x,xmin,xmax，di}]  
		自变量为x，从xmin到xmax间隔di
		
	- Table[expr,{x,{x1,x2...xn}}]  
		自变量为x，直接用x1，x2.。。
		
	- Table[expr,{x,xmin,xmax},{y,ymin,ymax}。。。]  
		自变量为x，是一个嵌套列表。如果是只有x，y就是二维表  
		xmax-xmin行，ymax-ymin列，例如：
		
			In[34]:= Table[x + y, {x, 2, 4}, {y, 1, 2}]  
			Out[34]= {{3, 4}, {4, 5}, {5, 6}}
			
#### 1.3规则与模式

- 规则	

	- **ReplaceAll (/.) 用法**

		expr/.{rule} 用规则替换前面的表达式，例如
		
			In[1]:= {x, x^2, y, z} /. x -> a
			Out[1]= {a, a^2, y, z}

### 2 数学和算法

### 3 可视化和图形
	
#### 3.1 数据可视化

- **ListPlot用法**

	- ListPlot[{y1,y2...}]

			x坐标取1,2，y坐标取y1，y2

	- LintPlot[{x1,y1},{x2,y2},{x3,y3}...]
	
			对于曲面上的点给出明确的 x, y, z 坐标：  
			ListPlot3D[{{0, 0, 1}, {1, 0, 0}, {0, 1, 0}}, Mesh -> All]
	- ListPlot[list1,list2,...]

			In[1]:= ListPlot[Prime[Range[25]], Filling -> Axis]
			In[1]:= ListPlot[Table[{Sin[n], Sin[2 n]}, {n, 50}]]
	
- **ListPlot3D用法**

	- ListPlot3D[array]
	
			data = Table[Sin[j^2 + i], {i, 0, Pi, Pi/5}, {j, 0, Pi, Pi/5}]  
			ListPlot3D[data, Mesh -> None, InterpolationOrder -> 3, ColorFunction -> "SouthwestColors"]
	- ListPlot3D[{data1,data2,...}]

	
#### 3.2函数可视化

- **Plot用法** 

	- Plot[f,{x,xmin,xmax}]
	
			Plot[Sin[x],{x,0,2Pi}]
	
	- Plot[{f1,f2},{x,xmin,xmax}] 
	
			Plot[{Sin[x],Sin[2x],Sin[3x]},{x,0,2Pi}]

- **Plot3D用法**

	- Plot3D[f,{x,xmin,xmax},{y,ymin,ymax}]

			Plot3D[Sin[x+y^2],{x,-3,3},{y,-3,3}]
		


### 4 数据操纵

### 5 可计算数据

### 6 动态交互

### 7 笔记本和文档

### 8 系统界面和部署

### 9 新功能

### 10 附加程序包




		

