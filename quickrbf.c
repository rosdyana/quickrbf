#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define Sigma_Para 5
#define MYLAMDA 10e-12

typedef double Qfloat;

int num_center,num_train,num_test;
int num_real_att, num_real_class;

int *center_c,*train_c,*test_c;
Qfloat **center_data,**train_data,**test_data;

Qfloat *center_sigma;
Qfloat **center_weight;
Qfloat **center_mean;
Qfloat **k_matrix;

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

Qfloat **weightalloc(int n_class, int n_data)
{
    Qfloat **data;    
    int i;
    data = (Qfloat **) calloc(n_class+1, sizeof(Qfloat *));
    assert(data != NULL);

    for (i = 0; i <= n_class; i++) {
        data[i] = (Qfloat *) calloc(n_data, sizeof(Qfloat));
        assert(data[i] != NULL);
    }
    return data;
}

Qfloat **matrixalloc(int n_dim)
{
    Qfloat **data;    
    int i;
    data = (Qfloat **) calloc(n_dim+1, sizeof(Qfloat *));
    assert(data != NULL);

    for (i = 0; i <= n_dim; i++) {
        data[i] = (Qfloat *) calloc(n_dim+1, sizeof(Qfloat));
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


void read_center(const char *filename)
{
	int i;
	FILE *fp = fopen(filename,"r");
	char *p, *q, buf[65535];
	
	num_center = 0;
	while (fgets(buf, sizeof(buf), fp)) {
		if (buf[0] == '#') continue;
		num_center++;
	}

	rewind(fp);
	printf("#center_data=%d,#att=%d,#class=%d\n",num_center,num_real_att,num_real_class);
	center_c = idatavector(num_center);
	center_data = dataalloc(num_center, num_real_att);
	
	i = 0;
	while(fgets(buf, sizeof(buf), fp)) {
		if(buf[0] == '#') continue;
		center_c[i] = atoi(strtok(buf, " \t\r\n"));
        while((p = strtok(NULL, " \t\r\n"))) {
			*(q = strchr(p, ':')) = '\0';
			center_data[i][atoi(p)] = (double) atof(q + 1);
		}
		i++;
	}
	fclose(fp);
	printf("Read %d data in center data file.\n", num_center);
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


void read_test(const char *filename)
{
	int i;
	FILE *fp = fopen(filename,"r");
	char *p, *q, buf[65535];
	num_test = 0;
	while (fgets(buf, sizeof(buf), fp)) {
		if (buf[0] == '#') continue;
		num_test++;
	}
	rewind(fp);
	test_c = idatavector(num_test);
	test_data = dataalloc(num_test, num_real_att);
	i = 0;
	while(fgets(buf, sizeof(buf), fp)) {
		if(buf[0] == '#') continue;
		test_c[i] = atoi(strtok(buf, " \t\r\n"));
        while((p = strtok(NULL, " \t\r\n"))) {
			*(q = strchr(p, ':')) = '\0';
			test_data[i][atoi(p)] = (double) atof(q + 1);
		}
		i++;
	}
	fclose(fp);
	printf("Read %d data in testing data file.\n", num_test);
}

void determine_widths()
{
    int i;
/*    int     i,j,t;
    Qfloat n1,n2;*/
    
    center_sigma = datavector(num_center);
/*
    for(i=0; i<num_center; i++){
        n2=100000;
        for( j = 0 ; j < num_train ; j++ ){
           n1=0;
           for( t = 1 ; t <= num_real_att ; ++t ){
                n1 += (center_data[i][t] - train_data[j][t]) * (center_data[i][t] - train_data[j][t]);
                }
           n1 = sqrt(n1);
        if (n1<n2 && train_c[j]!=center_c[i]) n2=n1;
        }

        center_sigma[i]=n2 * Sigma_Para;
        //printf("%d:%f\n",i,center_sigma[i]);
    }
*/
    for(i=0; i<num_center; ++i) center_sigma[i]=5;
}

void determine_weights()
{
	int i,j,k;
	
	Qfloat sum;
	Qfloat *pivot;
    Qfloat *rbf_value;
    
    clock_t t1,t2=0;
    
   	t2=clock();
	        	
	center_weight = weightalloc(num_real_class,num_center);
	center_mean = weightalloc(num_real_class,num_center);
	rbf_value = datavector(num_center);
	pivot = datavector(num_center);
	
	k_matrix = matrixalloc(num_center);

	for (i=0;i<num_train;i++){
		for(j=0;j<num_center;j++) rbf_value[j]=0.0;
		for(j=0;j<num_center;j++){
			for(k=1;k<=num_real_att;k++){
				rbf_value[j] += (train_data[i][k]-center_data[j][k])*(train_data[i][k]-center_data[j][k]);
			}
            rbf_value[j] = sqrt(rbf_value[j]);
			rbf_value[j] = exp(-0.5*rbf_value[j]/(center_sigma[j]*center_sigma[j]));
			center_mean[train_c[i]][j] += rbf_value[j];
		}
		for(j=0;j<num_center;j++){
			for(k=j;k<num_center;k++){
				k_matrix[j][k]+=rbf_value[j]*rbf_value[k];	
			}	
		}
	}
	for(i=0;i<num_center;i++){
		for(j=0;j<num_center;j++){
			k_matrix[i][j]/=(Qfloat) num_train;
		}
		for(j=0;j<=num_real_class;j++) center_mean[j][i] /= (Qfloat) num_train;	
		k_matrix[i][i] += MYLAMDA;
	}	

	t1=clock();
	printf( "\n 2.1 Generating K matrix used %2.3f seconds\n", (Qfloat)(t1-t2) / CLOCKS_PER_SEC );
	//Starting Cholesky Decomposition
	for (i=0;i<num_center;i++) {
		for (j=i;j<num_center;j++) {
			for (sum=k_matrix[i][j],k=i-1;k>=0;k--) sum -= k_matrix[i][k]*k_matrix[j][k];
			if (i == j) {
				if (sum <= 0.0)	printf("Cholesky Decomposition Failed!\n");
				pivot[i]=sqrt(sum);
			} else k_matrix[j][i]=sum/pivot[i];
		}
	} 

	//Calculation of the weights vector
	for (i=0;i<num_center;i++) {
		for (j=0;j<=num_real_class;j++){
			center_weight[j][i] = center_mean[j][i];
			for (k=i-1;k>=0;k--){
				center_weight[j][i] -= k_matrix[i][k]*center_weight[j][k];
			}
		}
		for (j=0;j<=num_real_class;j++){		
				center_weight[j][i] /= pivot[i];
		}
	}
	
	for (i=num_center-1;i>=0;i--) {
		for (j=0;j<=num_real_class;j++){
		    for (k=i+1;k<num_center;k++) { 
				center_weight[j][i] -= k_matrix[k][i]* center_weight[j][k]; 
			}
	        center_weight[j][i] /= pivot[i];
		}
	}
	t2=clock();
	printf( " 2.2 Cholesky Decomposition used %2.3f seconds\n", (Qfloat)(t2-t1) / CLOCKS_PER_SEC );
}

int classify(int a)
{
    int i,j,k;
    int ans_c=0;
    
    Qfloat *rbf_value;
    Qfloat *ans;
    Qfloat ans_max;

	ans = (Qfloat *) calloc(num_real_class+1, sizeof(Qfloat));
    assert(ans != NULL);
    
   	rbf_value = datavector(num_center);

	for(j=0;j<num_center;j++){
		for(k=1;k<=num_real_att;k++){
			rbf_value[j] += (test_data[a][k]-center_data[j][k])*(test_data[a][k]-center_data[j][k]);
		}
		rbf_value[j] = sqrt(rbf_value[j]);
		rbf_value[j] = exp(-0.5*rbf_value[j]/(center_sigma[j]*center_sigma[j]));
    }

    for (i=0;i<=num_real_class;i++){
		for (j=0;j<num_center;j++){
    		ans[i] += (rbf_value[j]*center_weight[i][j]);
		}
    }

    ans_max = 0;
    for (i=0;i<=num_real_class;i++){
		if (ans[i]>ans_max) {
    		ans_max = ans[i];
   		 	ans_c = i;
		}
    }
	return ans_c;
}

int main(int argc, char **argv)
{
    int i;
    int acc=0;
    clock_t start1,end1,mid1,mid2,total1=0;


    switch(argc){
    case 4:
	read_train(argv[2]); // read train data file
	read_center(argv[1]); // read center data file
	read_test(argv[3]); // read test data file
	
	start1=clock();//Start timing
	determine_widths();
	mid1=clock();
	printf( "\n1. Calculation of bandwidths used %2.3f seconds\n", (Qfloat)(mid1-start1) / CLOCKS_PER_SEC );
	determine_weights();
	mid2=clock();
	printf( "2. Calculation of weights used %2.3f seconds\n\n", (Qfloat)(mid2-mid1) / CLOCKS_PER_SEC );
	
	for (i=0;i<num_test;i++){
	    if (classify(i)==test_c[i]) acc++;
	}
	mid1=clock();
	printf("3. Testing use %2.3f seconds\n", (Qfloat)(mid1-mid2) / CLOCKS_PER_SEC );
	printf("The accuracy is %f\n",(Qfloat)acc/num_test);

	end1=clock();total1=end1-start1;//End timing
	printf( "\nTotal use %2.3f seconds\n\n", (Qfloat)(total1) / CLOCKS_PER_SEC );

	break;
    default:
	printf("usage: quickrbf [center file] [training data file] [testing data file] \n");
	break;
    }
    return 0;
 
}
