/*
 * File Name: case.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-19 20:24:55 week:3
*/
#include "common.h"
#include "list.h"
#include "stack.h"
#include "queue.h"
/*use stack to reverse list*/
void case1(){
	cout<<"case1.............................."<<endl<<"reverse a list by stack"<<endl;
	ArrayList<int> list;
	cout<<endl;
	list.insert_tail(1);
	list.insert_tail(2);
	list.insert_tail(3);
	list.insert_tail(4);
	list.print();
	cout<<endl;
	ArrayStack<int> stack;
	int *da;
	list.locate_element(1,da);
	stack.push(*da);
	list.locate_element(2,da);
	stack.push(*da);
	list.locate_element(3,da);
	stack.push(*da);
	list.locate_element(4,da);
	stack.push(*da);
	int dd;
	while(!stack.isEmpty()){
		stack.pop(&dd);
		cout<<dd<<" "<<endl;
	}
}
/* maze*/
enum {
	NO_RUN=0,
	RUN,
	WALL
};
class maze_node{
	public:
		int x;
		int y;
		int dir[4];
		int operator == (maze_node & da){
			if(x==da.x&&y==da.y)
				return 1;
			else
				return 0;
		}
		int operator > (maze_node & da){
			if(x>da.x&&y>da.y)
				return 1;
			else
				return 0;
		}
		int operator < (maze_node & da){
			if(x<da.x&&y<da.y)
				return 1;
			else
				return 0;
		}
		maze_node(){
			x=y=0;
			memset(&dir[0],0,sizeof(dir));
		}
		maze_node(int xx,int yy){
			x=xx;
			y=yy;
			memset(&dir[0],0,sizeof(dir));
		}
		maze_node(maze_node &da){
			x=da.x;
			y=da.y;
			memcpy(&dir[0],&da.dir[0],sizeof(da.dir));
		}
};	
ostream & operator << (ostream &os ,maze_node &da){
	return os<<"("<<da.x<<","<<da.y<<")"<<endl;//<<"left: "<<da.dir[0]<<",down:"<<da.dir[1]<<",right:"<<da.dir[2]<<",up:"<<da.dir[3]<<endl;
}
enum {
	LEFT=0,
	DOWN,
	RIGHT,
	UP
};
char dir_str[4][100]={
	"left",
	"down",
	"right",
	"up"
};
#define MaxCol 4
#define MaxRow 4
int cur_dir=LEFT;
int get_next_node(maze_node &in,maze_node &out,int* map){
	int ret;
	ret=-1;
	maze_node next;
	int i;
	for(i=0;i<4;i++){
		next.x=in.x;
		next.y=in.y;
		if(i==2){
			cur_dir=(cur_dir+1)%4;
			continue;
		}
		if(in.dir[cur_dir]==NO_RUN){
			if(cur_dir==LEFT)
				next.x-=1;
			else if(cur_dir==DOWN)
				next.y+=1;
			else if(cur_dir==RIGHT)
				next.x+=1;
			else if(cur_dir==UP)
				next.y-=1;
			cout<<"cur "<<in;
			cout<<"cur dir:"<<dir_str[cur_dir]<<endl;
			cout<<"next:("<<next.x<<","<<next.y<<"),data:"<<map[next.y*4+next.x];
			if(next.x<0||next.x>MaxRow-1||next.y<0||next.y>MaxCol-1||map[next.y*MaxRow+next.x]==1){
				cout<<",err "<<endl;
				in.dir[cur_dir]=WALL;
				cur_dir=(cur_dir+1)%4;
			}else{
				cout<<",suc "<<endl;
				in.dir[cur_dir]=RUN;
				out=next;
				ret=0;
				break;
			}
		}else{
			cur_dir=(cur_dir+1)%4;	
		}
	}
	return ret;
}
/*use stack for a maze solution*/
void case2(){
	cout<<"case2:................."<<endl<<"maze"<<endl;
	int maze_map[MaxCol*MaxRow]={
		0,0,0,1,
		1,0,1,1,
		0,0,0,1,
		1,1,0,0
	};
	int ret;
	maze_node cur,next,start,end;
	start=maze_node(0,0);
	end=maze_node(MaxCol-1,MaxRow-1);
	ArrayStack<maze_node> stack;
	cur=start;
	cur_dir=RIGHT;
	while(1){
		ret=get_next_node(cur,next,maze_map);
		if(ret==0){
			stack.push(cur);
			cur=next;
		}else if(ret==-1){
			stack.pop(&cur);
		}
		if(cur==end){
			stack.push(cur);
			break;
		}
		if(cur==start)
			break;
	}
	cout<<"finish................"<<endl;
	int i,j;
	cout<<"maze is:"<<endl;
	for(i=0;i<MaxCol;i++){
		for(j=0;j<MaxRow;j++){
			printf("%d\t",maze_map[i*MaxRow+j]);
		}
		printf("\n");
	}
	cout<<"Note:0 means can go,1 means wall. x is from left to right,y is from top to bottom."<<endl;
	cout<<"coordinate is (x,y).start at (0,0),end at(3,3)."<<endl;
	cout<<"solution is:"<<endl;
	stack.print();
}
/*use stack for a expression solution*/
void case3(){
	cout<<"case3:..........."<<endl;
	cout<<"use stack for expression  "<<endl;
}

// vim:tw=72 
