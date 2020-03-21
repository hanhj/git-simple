#matlab study
## 1.通用
定义符号：syms x

pretty(fx)人性化显示公式

## 2.极限limit
	
	limit(expr,x,a)
	expr是表达式，可以直接用'xxx',也可用syms来定义。
	x表示自变量，a表示趋向。
例如：

	limit('(exp(x)+exp(-x))/sin(x)',x,0) or
	syms x y;
	y=(exp(x)+exp(-x))/sin(x);
	limit(y,x,0)
	结果为2
## 3.求导diff
	
	diff(expr)例如：f=sin(x);diff(f);
## 4.积分int
    
	int(fun,x)计算不定积分
    int(fun,x,a,b):计算定积分
例如：

	syms f x
	f=sinx;int(f)
	int(f,-pi,pi);
利用int可以计算重积分。

	int(int(fun,x,a,b),y,c,d)

## 5.数值积分：

- 梯形积分：trapz(x,y)
例如：

    x=-1:0.01:1
    y=tan(x);
    trapz(x,y);
- 辛普森积分：quad(fun,x,a,b)    
fun可以是匿名函数@(x)，或内联函数inline()

		比如y=cosx,可以表示成：y=@(x)cos(x),y=inline('cos(x)')
例如：

    y=@(x)tan(x)
    quad(y,-1,1)

## 6.幂函数方程solve

    solve('eq')
    solve('eq',var)
    solve('eq1','eq2',...)
    solve('eq1','eq2'...'eqn','var1','var2'...'varn')
## 7.微分方程dsolve 
	
	dsolve('eq1,eq2..','con1,con2','v')
	eq1,eq2..微分方程，
	  con1，con2..初始条件，
	  v	变量
	  Dy	表示导数y'

例子：求y'+3xy=4x的通解
	
	syms y x;
	dsolve('Dy+3*x*y=4*x','x')
	ezplot(y);
	ans =
		4/3+exp(-3/2*x^2)*C1
	pretty(ans)
            2
   4/3 + exp(- 3/2 x ) C1

