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

	- Range[xmin,xmax]   
		产生xmin到xmax之间的数，间隔为1。

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
		xmax-xmin+1行，ymax-ymin+1列，例如：
		
			In[34]:= Table[x + y, {x, 2, 4}, {y, 1, 2}]  
			Out[34]= {{3, 4}, {4, 5}, {5, 6}}
			
#### 1.3规则与模式

- 规则	

	- **ReplaceAll (/.) 用法**

		expr/.{rule} 用规则替换前面的表达式，例如
		
			In[1]:= {x, x^2, y, z} /. x -> a
			Out[1]= {a, a^2, y, z}

### 2 数学和算法
### 2.5 级数,极限

- **Sum求和**

	- Sum[expr,{n,nmin,nmax}]  
	求expr当n从nmin到nmax的和  
	求1+2+3

			In[26]:= Sum[i, {i, 1, 3}]
			Out[26]= 6
	求a+aq+aq^2+...等比数列极限

			In[19]:= Sum[a*q^(n - 1), {n, 1, Infinity}]
			Out[19]= -(a/(-1 + q))

- **Product连乘**	
	- Product[expr,{n,nmin,nmax}]

			In[24]:= Product[x^i/i, {i, 1, 5}]
			Out[24]= x^15/120	

- **Limit求极限**
	- Limit[expr,var->n]
	
			In[3]:= Limit[x^2, x -> 1]
			Out[3]= 1

#### 2.7微积分

- **D 求导**

	- D[f,x]  
		给出f对x的偏导数
			
			In[1]:= D[x^n, x]
			Out[1]= n x^(-1 + n)
	
	- D[f,{x,n}]  
		给出f对x的n阶偏导数
		
			In[1]:= D[Sin[x]^10, {x, 4}]
			Out[1]= 5040 Cos[x]^4 Sin[x]^6 - 4680 Cos[x]^2 Sin[x]^8 +  280 Sin[x]^10
		
- **Dt求全导数**

	- Dt[f]		
		全微分
		
			In[1]:= Dt[x y]
			Out[1]= y Dt[x] + x Dt[y]
	
	- Dt[f,x]  
		对f求x的全导数
			
			In[1]:= Dt[a x + b, x]
			Out[1]= a + x Dt[a, x] + Dt[b, x]
	
- **Integrate积分**

	- Integrate[f,x] 不定积分

			In[22]:= Integrate[x, x]
			Out[22]= x^2/2
	- Integrate[f,{x,xmin,xmax}] 定积分
	
			In[20]:= Integrate[x, {x, 0, 2}]
			Out[20]= 2	
	
	- Integrate[f,{x,xmin,xmax},{y,ymin,ymax}...] 多重积分
			
			z[x_, y_] = x^2 + y^2;
			dzx = D[z[x, y], x];
			dzy = D[z[x, y], y];
			ds = z[x, y]*Sqrt[1 + dzx^2 + dzy^2] /. {x -> r*Cos[t], y -> r*Sin[t]};
			Integrate[ds*r, {t, 0, 2 Pi}, {r, 0, 1/2}]

- **Solve解方程** 

	- Solve[expr,var]
	
			In[1]:= Solve[x^2 + a x + 1 == 0, x]
			Out[1]= {{x -> 1/2 (-a - Sqrt[-4 + a^2])}, {x -> 1/2 (-a + Sqrt[-4 + a^2])}}
			
		可用来解包括多个方程的线性方程组

			In[18]:= Solve[{y + z (2*x - 2) == 0, 
			  x + z*2*y == 0, (x - 1)^2 + y^2 - 1 == 0}, {x, y, z}]

			Out[18]= {{x -> 0, y -> 0, z -> 0}, {x -> 3/2, y -> -(Sqrt[3]/2), 
			  z -> Sqrt[3]/2}, {x -> 3/2, y -> Sqrt[3]/2, z -> -(Sqrt[3]/2)}}			

- **DSolve解微分方程** 

	- DSolve[expr,y,x]
	
			In[1]:= Solve[{y'[x]==2x},y[x], x]
			Out[1]= {{y[x] -> x^2 + C[1]}}
			
			
		可用来解包括多个方程的方程组
	- DSolve[{exp1,exp2..},{y1[x],y2[x]..},x]


			
	

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




		

