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