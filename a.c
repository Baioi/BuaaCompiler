const int F_MAX = 5, A_MAX = 5;
const char quit = 'q';
const char continue = 'c';
int length, width;
char p;
int area(int mode){
	const int pi1 = +3;
	int s;
	if(mode)
		s = length*width;
	else{
		if(length != width)
			error();
		s = pi1*length*width;
	}
	return (s);
}
int square_c(){
	return ((length+width)*2);
}
void out(int mode){
	if(mode == 0){
		printf("circle area: ",area(length, width, 0));
		return;
	}
	if(mode == 1) {
		printf("no circle circumference data!");
		return;
	}
	if(mode == 2) {
		printf("square area: ",area(length,width,1));
		return;
	}
	if(mode == 3) {
		printf("square circumference:", square_c());
		return;
	}
	else
		printf(q);
}
void error(){
	;
}
int test(int t){
	t = t + -2;
	if(t < 0)
		return (-1);
	test(t);
	return (t);
}
void main(){
	int w[5],l[5],i;
	int mode;
	char sign;
	for(i=0;i<A_MAX;i=i+1){
		printf("width:");
		scanf(w[i]);
	}
	for(i=A_MAX;i>0;i=i-1){
		printf("length:");
		scanf(l[A_MAX-i]);
	}
	do{
		scanf(mode,i,sign);
		width=w[i];
		length=l[i];
		out(mode);
	}
	while(sign!='c')
	width = 3;
	printf(test(width));
}
