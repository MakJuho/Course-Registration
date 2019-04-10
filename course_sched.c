#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LENGTH 4096
struct CourseEntry {
  char name[MAX_LENGTH]; // 과목의 이름
  struct CourseEntry* prerequisites; // 선수 과목들
  int n_prerequisites; // 선수 과목의 수
  float difficulty; // 과목의 난이도
};

// 문자열 좌측 공백문자 삭제 함수
char* ltrim(char *s) {
  char* begin;
  begin = s;


  while (*begin != '\0') {
    if (isspace(*begin))
      begin++;
    else {
      s = begin;
      break;
    }
  }

  return s;
}

void Sort(char (*str)[MAX_LENGTH], int n)
{
	int min;
	char temp[MAX_LENGTH];

	for(int i=0; i<n-1; i++)
	{
		min = i;
		for(int k=i+1; k<n; k++)
		{
			if(strcmp(str[min], str[k]) >0)
			{
				min = k;
			}
		}
		strcpy(temp, str[i]);
		strcpy(str[i], str[min]);
		strcpy(str[min], temp);
	}
}
//int main()
int main(int argc, char** argv)
{
	/* Planning
	   Using Structure
	1. how to load Database.csv file
	2. how to load everytime0~2.csv file
	3. Following rule - topology sort
	4. Check results
	*/

	/*
	   Information Hints
	1. argc : 2
	2. argv[0] : ./course_sched
	   argv[1] : ./in/everytime0~2.csv
	   argv[2] : null
	*/
	
	// Definition Structure
	struct CourseEntry* Subject=(struct CourseEntry*)malloc(sizeof(struct CourseEntry)*6);


	for(int i=0; i<6; i++){
			Subject[i].prerequisites =(struct CourseEntry*)malloc(sizeof(struct CourseEntry)*3);
	}

	for(int i=0; i<6; i++){
		Subject[i].difficulty=5;
		for(int j=0; j<3; j++){
			Subject[i].prerequisites[j].difficulty=5;
		}

	}

	// read csv file.
	FILE* pFile = fopen("./in/database.csv", "r");

	if(pFile==NULL){
		//printf("pFile 파일 열기 실패!\n");
		return 0;
	}
	if(pFile!=NULL)
	{
		char strTemp[1000];
		char* pStr;
		int change_sub_cnt = 0;
		char* value;

		while(!feof(pFile))
		{

			pStr = fgets(strTemp, sizeof(strTemp), pFile);
			
			value = strtok(pStr,",");

			while(value!=NULL)
			{
	
				/*
					1. name
					2. presubject_count <- if
					3. presubject
				*/
				// input subject_name
				strncpy(Subject[change_sub_cnt].name,value,strlen(value));

				value = strtok(NULL,",");
				Subject[change_sub_cnt].n_prerequisites=atoi(value);
				
				value = strtok(NULL,",");
				
				int cnt=0;
				
				while(Subject[change_sub_cnt].n_prerequisites!=cnt){
					if(Subject[change_sub_cnt].n_prerequisites==cnt+1)
					{
						value[strlen(value)-1]='\0';
						strncpy(Subject[change_sub_cnt].prerequisites[cnt].name,ltrim(value),strlen(ltrim(value)));
					}
					else{
						strncpy(Subject[change_sub_cnt].prerequisites[cnt].name,ltrim(value),strlen(ltrim(value)));
					}
					value = strtok(NULL,",");
					cnt++; // presubject_ input 할때마다

				}
			
				change_sub_cnt++;
			}
			
		}
		fclose(pFile);
	}
	else
	{
		printf("Fail\n");

	}

	FILE* eFile = fopen(argv[1],"r");
    if(eFile==NULL){
		printf("eFile파일 열기 실패!\n");
		return 1;
	}

	if(eFile!=NULL)
      {
        char strTemp2[1000];
        char* pStr2;
        char* value2;
		int level_val=0;
		int pre_level_val=0;
 
         while(!feof(eFile))
         {

              pStr2 = fgets(strTemp2, sizeof(strTemp2), eFile);
              if(pStr2 == NULL)
              {
                  break;
              }
 
              if(pStr2[0]=='\n'){
                  break;
              }
 
              value2 = strtok(pStr2,",");
			
			  // insert 난이도 
			  for (int i=0; i<6; i++)
			  {
				if(strcmp(Subject[i].name,ltrim(value2))==0){
					level_val=i;
				}
				for(int j=0; j<3; j++){
					if(strcmp(Subject[i].prerequisites[j].name,ltrim(value2))==0){
						pre_level_val=j;
					}
				}
			  }
              value2 = strtok(NULL,", ");
			  Subject[level_val].difficulty = atof(value2);
			  Subject[level_val].prerequisites[pre_level_val].difficulty=atof(value2);

          }
          fclose(eFile);
      }

	/*
		0 : Computer Programming
		1 : Discrete Mathematics
		2 : Digital Systems
		3 : Data Structures
		4 : Computer Architecture
		5 : Operating Systems
	*/
	
	// insert ready_print
	
	/*
		1. 선수 과목 정렬
		2. 난이도 정렬
		3. 알파벳 순서
	*/

	int print_ary[6] = {0,0,0,0,0,0};

	int each_sub_condition[6][6] ={
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0}
	};

	int condition_finish=0;
	
	while( condition_finish != 6)
	{
	// make array
	for(int k=0; k<6; k++){
		char* tmp_subject = Subject[k].name;
		for(int i=0; i<6; i++){
			for(int j=0; j<3; j++){
				if(strcmp(tmp_subject,Subject[i].prerequisites[j].name)==0){
					each_sub_condition[i][k]=1;
				}
			}
		}
	}
	
	int satisfied_ary[6] = {1,1,1,1,1,1};
	// discriminate Satisfy
	for(int i=0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			if(print_ary[j]!=each_sub_condition[i][j] && print_ary[j]==0)
			{
				satisfied_ary[i]=0;
			}
		}
	}
	
	// 가능한 선수 과목들 중에서 2차원 배열로 순서 정하기
	float cmp_difficulty[6][2]={
		{0,0},
		{0,0},
		{0,0},
		{0,0},
		{0,0},
		{0,0}
	};

	int avail_cnt=0;
    for(int i=0; i<6; i++)
    {
       if(satisfied_ary[i]==1)
       {
	         cmp_difficulty[i][0]= i;
			 cmp_difficulty[i][1]= Subject[i].difficulty;
        }
    }
	
	for(int j=0; j<6; j++)
	{
		for(int i=0; i<6; i++)
		{
			if(cmp_difficulty[i][1] < cmp_difficulty[i+1][1])
			{
				float tmp = cmp_difficulty[i][1];
				float tmp2 = cmp_difficulty[i][0];

				cmp_difficulty[i][1] = cmp_difficulty[i+1][1];
				cmp_difficulty[i][0] = cmp_difficulty[i+1][0];

				cmp_difficulty[i+1][1] = tmp;
				cmp_difficulty[i+1][0] = tmp2;
			}
		}
	}
	
	int same_difficulty_cnt=0;
	// 난이도가 같지 않을 때 -> 가장 난이도 높은 부분만 출력
	if(cmp_difficulty[0][1]!=cmp_difficulty[1][1])
	{
		printf("%s\n",Subject[(int)cmp_difficulty[0][0]].name);
		print_ary[(int)cmp_difficulty[0][0]]=1;
		for(int i=0; i<6; i++)
		{
			each_sub_condition[(int)cmp_difficulty[0][0]][i]=2;
		}
	}

	// 난이도가 같을 때 -> 모아서 한 번에 알파벳 비교.
	else{
		for(int i=0; i<6; i++)
		{
			if(cmp_difficulty[0][1]==cmp_difficulty[i][1])
			{
				same_difficulty_cnt++;
			}
		}
		char s[same_difficulty_cnt][MAX_LENGTH];

		for(int i=0; i<same_difficulty_cnt+1; i++)
		{
			strcpy(s[i],Subject[(int)cmp_difficulty[i][0]].name);
		}
		
		Sort(s, same_difficulty_cnt);
		
		for(int i=0; i<6; i++)
		{
			if(strcmp(s[0],Subject[i].name)==0)
			{
				printf("%s\n", Subject[i].name);
				print_ary[i]=1;
				for(int j=0; j<6; j++)
				{
					each_sub_condition[i][j]=2;
				}
			}
		}
	}
	condition_finish=0;
	for(int i=0; i<6; i++)
	{
		if(print_ary[i]==1){
			condition_finish++;
		}
	}
	}
  return 0;
}
