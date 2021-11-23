void randomRegistration (char imma[]);
int randomInt (int min,int max); 



// char* randomRegistration (){
// 	char* mat=malloc(6);
//      *mat = (char) ('A' + randomInt(0,25));
//      *(mat+1) = 'A' + (randomInt(0,25));
//      mat[2] = '-';
//      mat[3] = 'A' + (random()%26);
//      mat[4] = 'A' + (random()%26);
//      mat[5] = 'A' + (random()%26);
//      mat[6] ='\0';    
// return mat;	
// }

// int randomInt (int min,int max){
// 	int result;
// 	static int A;
// 	srand( time( NULL )+A );
// 	A++;
// 	if(A>100)A=0;
// 	result = (rand() % (max - min)) + min;
// 	return result;
// 	}