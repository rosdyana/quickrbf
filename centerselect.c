#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

typedef double Qfloat;

int num_center,num_train;
int num_real_att, num_real_class;

int *data_select;

int *train_c;
Qfloat **train_data;


int *idatavector(int n_data)
{
	int *v;

	v=(int *) calloc(n_data, sizeof(int));
	assert(v != NULL);
	return v;
}

Qfloat *datavector(int n_data)
{
	Qfloat *v;

	v=(Qfloat *) calloc(n_data, sizeof(Qfloat));
	assert(v != NULL);
	return v;
}


Qfloat **dataalloc(int n_data, int n_att)
{
    Qfloat **data;    
    int i;
    data = (Qfloat **) calloc(n_data, sizeof(Qfloat *));
    assert(data != NULL);

    for (i = 0; i < n_data; i++) {
        data[i] = (Qfloat *) calloc(n_att+1, sizeof(Qfloat));
        assert(data[i] != NULL);
    }
    return data;
}

void datafree(Qfloat **data, int n_data, int n_att)
{
    int i;

    for (i = 0; i <= n_data; i++) {
        free(data[i]);
    }
    free(data);
}



void read_train(const char *filename)
{
	int i;
	int tmp_c;

	FILE *fp = fopen(filename,"r");
	char *p, *q, buf[65535];

	num_train = num_real_att = num_real_class = 0;

	while (fgets(buf, sizeof(buf), fp)) {
		if (buf[0] == '#') continue;
		num_train++;
		tmp_c = atoi(strtok(buf, " \t\r\n"));
		if(tmp_c>num_real_class) num_real_class=tmp_c;
		while((p = strtok(NULL, " \t\r\n"))) {
			*(q = strchr(p, ':')) = '\0';
			if(atoi(p)>num_real_att) num_real_att=atoi(p);
		}
	}
	rewind(fp);
	train_c = idatavector(num_train);
	train_data = dataalloc(num_train, num_real_att);
	
	i = 0;
	while(fgets(buf, sizeof(buf), fp)) {
		if(buf[0] == '#') continue;
		train_c[i] = atoi(strtok(buf, " \t\r\n"));
        while((p = strtok(NULL, " \t\r\n"))) {
			*(q = strchr(p, ':')) = '\0';
			train_data[i][atoi(p)] = (double) atof(q + 1);
		}
		i++;
	}
	fclose(fp);
	printf("Read %d data in training data file.\n", num_train);
}

void center_select()
{
     int num_select;
     int temp_select;
     
     data_select = idatavector(num_train);
     num_select = 0;
     srand((unsigned int)time((time_t *)NULL)*time((time_t *)NULL)%1000);
     
     while (num_select < num_center){
           temp_select = (int)((double)rand() / RAND_MAX * num_train);
           if (data_select[temp_select]!=1) {
              data_select[temp_select] = 1;
              num_select ++;
              
           }            
     }
     printf("number of center data: %d\n",num_center);
      
}


int main(int argc, char **argv)
{
    int i,j;
    clock_t start1,end1,total1=0;
    char outfile[255];
    FILE *output_file;

    switch(argc){
    case 3:
	read_train(argv[1]); // read train data file
	num_center = atoi(argv[2]); // read center data file
	sprintf(outfile,"%s.%d",argv[1],num_center);
	output_file = fopen(outfile,"w");
	
	start1=clock();//Start timing
	
	center_select();
	
	for (i=0;i<num_train;i++){
        if(data_select[i]==1) {
             fprintf(output_file,"%d ",train_c[i]);
                 for(j=1;j<=num_real_att;j++){
                     if (train_data[i][j]!=0) fprintf(output_file,"%d:%.8g ",j,train_data[i][j]);
                 }
                 fprintf(output_file,"\n");
        }    
    }
    
    fclose(output_file);
	

	end1=clock();total1=end1-start1;//End timing
	printf( "\nTotal use %2.3f seconds\n\n", (Qfloat)(total1) / CLOCKS_PER_SEC );

	break;
    default:
	printf("usage: centerselect [training data file] [number of center] \n");
	break;
    }
    return 0;
 
}
