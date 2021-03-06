#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int removeLineEndings(char * str)
{
    int end=strlen(str);
    if (end>0)
    {
        if ( (str[end]==10) || (str[end]==13) )
        {
            str[end]=0;
        }
    }
    if (end>1)
    {
        if ( (str[end-1]==10) || (str[end-1]==13) )
        {
            str[end-1]=0;
        }
    }
    if (end>2)
    {
        if ( (str[end-2]==10) || (str[end-2]==13) )
        {
            str[end-2]=0;
        }
    }
    return 1;
}

int parseFile(const char * filename)
{
//printf("Parsing rostopic %s !\n",filename);
//fprintf(stderr,"Running COCO 2D skeleton (%s)\n",filename);
    FILE * fp = fopen(filename,"r");
    if (fp!=0)
    {
        int csvHeaderFields=0;
        int csvHeaderEmmited=0;
        char csvHeader[4096]= {0};

        int csvContentFields=0;
        char csvLastContent[4096]= {0};
        char type[512]= {0};
        char subtype[512]= {0};
        char content[512]= {0};
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        unsigned int frameNumber =0;


        while ((read = getline(&line, &len, fp)) != -1)
        {
            //fprintf(stderr,"Retrieved line of length %zu :\n", read);
            //fprintf(stderr,"%s", line);
            if (strstr(line,"---")!=0)
            {
                if (!csvHeaderEmmited)
                {
                    fprintf(stdout,"%s\n",csvHeader);
                    csvHeaderEmmited=1;
                }
                fprintf(stdout,"%s\n",csvLastContent);

                type[0]=0;
                subtype[0]=0;
                content[0]=0;
                csvContentFields=0;
                ++frameNumber;
                //fprintf(stderr,"NEXT MESSAGE , do flush here\n");
            }
            else
            {
                char * foundColonNL=strstr(line,": \n");
                if (foundColonNL!=0)
                {
                    *foundColonNL=0;
                    snprintf(type,512,"%s",line);
                    //fprintf(stderr,"PREVIOUSLINETYPE(%s)\n",type);
                }
                else
                {
                    char * foundContentAfterColon=strstr(line,": ");
                    if (
                        (foundContentAfterColon!=0)
                    )
                    {

                        *foundContentAfterColon=0;
                        snprintf(subtype,512,"%s",line);
                        snprintf(content,512,"%s",foundContentAfterColon+2);
                        //fprintf(stderr,"SUBTYPE(%s)\n",subtype+2);
                        removeLineEndings(content);
                        //fprintf(stderr,"CONTENT(%s)\n",content);
                        char * subtypeClean=subtype;
                        if (
                            (subtype[0]==' ') &&
                            (subtype[1]==' ')
                        )
                        {
                            subtypeClean=subtype+2;
                        }

                        char tmp[4096]= {0};

                        if (type[0]==0)
                        {
                            if (csvHeaderFields>0)
                            {
                                snprintf(tmp,4096,"%s,%s",csvHeader,subtypeClean);
                            }
                            else
                            {
                                snprintf(tmp,4096,"%s",subtypeClean);
                            }
                        }
                        else
                        {
                            if (csvHeaderFields>0)
                            {
                                snprintf(tmp,4096,"%s,%s_%s",csvHeader,type,subtypeClean);
                            }
                            else
                            {
                                snprintf(tmp,4096,"%s_%s",type,subtypeClean);
                            }
                        }

                        snprintf(csvHeader,4096,"%s",tmp);


                        if (csvContentFields>0)
                        {
                            snprintf(tmp,4096,"%s,%s",csvLastContent,content);
                        }
                        else
                        {
                            snprintf(tmp,4096,"%s",content);
                        }
                        snprintf(csvLastContent,4096,"%s",tmp);


                        ++csvContentFields;
                        ++csvHeaderFields;
                    }
                }
            }
        }
        if (line)
        {
            free(line);
        }

        fclose(fp);
        return 1;
    }
    fprintf(stderr,"Unable to open (%s)\n",filename);
    return 0;
}



int parseSTDIN()
{
	int ch;
	/* read character by character from stdin */
	do {
		ch = fgetc(stdin);
		putchar(ch);
	} while (ch != EOF);
  return 1;
}



int main(int argc, char** argv)
{
    if (argc<=1)
    {
      return ( parseSTDIN() == 1 );
    } else
    {
      return ( parseFile(argv[1]) == 1 );
    }
    return 0;
}
