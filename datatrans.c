#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	MAXBUF	65535

int main(int argc, char **argv)
{

    if (argc == 2)
    {
	int num;

	char buf[MAXBUF], outbuf[MAXBUF], *strp, *first, c, sc, *spanp, outfilename[1000];
	char delim[] = {' ', ',', ';'};

	FILE *fp, *fpout;

	int i = 0;                                                         

	if ((fp = fopen(argv[1],"r")) == NULL)
	{
	    printf("can not find %s\n", argv[1]);
	    return 1;
	}
	sprintf(outfilename,"%s.out",argv[1]);
	fpout = fopen(outfilename,"w");  

	while(fgets(buf, sizeof(buf)-1, fp)) {                           

	    num = 1;
	    outbuf[0] = '\0';

	    if(buf[0] == '#') continue;      


	    first = strp = buf;

	    while(strp != NULL)
	    {
		for (;;)
		{
		    c = *strp++;
			spanp = (char *)delim;
			do
			{
			    if ((sc = *spanp++) == c)
			    {
				if (c == 0)
				{
				    *(strp-1) = '\0';
					strp = NULL;
				    goto out;
				}
				else
				{
				    char *w = strp - 1;
					*w = '\0';
				    if (atof(first)!=0)
					sprintf(outbuf,"%s %d:%g",outbuf,num,atof(first));
				    num++;
				}
				first = strp;
			    }
			}
			while (sc != 0);
		}
out:
;
	    }                                  

	    fprintf(fpout,"%d%s\n",atoi(first),outbuf);
	    ++i;                                                               
	}             

	fclose(fp);                                 
	fclose(fpout);                   

	printf("converted %d data from %s to %s.out\n", i,argv[1],argv[1]);


    }
    else
	printf("Please Use: Command [input dataset] \n");

    return 0;
}




