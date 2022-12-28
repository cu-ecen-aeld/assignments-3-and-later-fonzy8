#include <stdio.h>
#include <syslog.h>

int main(int argc, char**argv)
{
    FILE *fptr;
    
    if ( argc != 3 ) {
    	// not enough parameters - log an error and show usage...
    	openlog(NULL,0,LOG_ERR);
    	syslog(LOG_ERR,"Invalid number of arguments %d\n", argc);
    	printf("Usage writer <<directory>> <<text>>\n");
    	return 1;
    } else {
    	openlog(NULL,0,LOG_DEBUG);
        syslog(LOG_DEBUG,"Writing %s to %s\n",argv[2],argv[1]);
        
        fptr = fopen(argv[1],"w");
        if ( fptr == NULL ) {
    	    // file couldnt be opened - log an error...
    	    openlog(NULL,0,LOG_ERR);
    	    syslog(LOG_ERR,"Couldnt open file %s\n", argv[1]);
    	    printf("Couldnt open file for writing\n");
    	    return 1;
        } else {
            // file open - write the text...
            fprintf (fptr,"%s\n",argv[2]);
            fclose(fptr);       
            return 0;
        }
    }
}
