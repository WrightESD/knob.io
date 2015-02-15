/*
 * arduino-serial
 * --------------
 *
 * A simple command-line example program showing how a computer can
 * communicate with an Arduino board. Works on any POSIX system (Mac/Unix/PC)
 *
 *
 * Compile with something like:
 *   gcc -o arduino-serial arduino-serial-lib.c arduino-serial.c
 * or use the included Makefile
 *
 * Mac: make sure you have Xcode installed
 * Windows: try MinGW to get GCC
 *
 *
 * Originally created 5 December 2006
 * 2006-2013, Tod E. Kurt, http://todbot.com/blog/
 *
 *
*/

#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // for usleep()
#include <getopt.h>

#include "arduino-serial-lib.h"

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>


void error(char* msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

int main()
{
    const int buf_max = 512;

    int fd = -1;
    char serialport[buf_max];
    int baudrate = 115200;  // default
    char quiet=0;
    char eolchar = '\n';
    int timeout = 1;
    char buf[buf_max];

    strncpy(serialport, "/dev/ttyACM0",sizeof(serialport));
    
    fd = serialport_init(serialport, baudrate);
    if( fd==-1 ) error("couldn't open port");
    if(!quiet) printf("opened port %s\n", serialport);
    //serialport_flush(fd);

    while(1) {
		
           
	    //fd = serialport_init(serialport, baudrate);
            //if( fd == -1 ) error("serial port not opened");
	    //if(!quiet) printf("opened port %s\n", serialport);

	    //serialport_flush(fd);

	   
		   long min, max;
		   long p;
		   long knobvolume;
	       long intervolume;
	       long volume;
		   
           memset(buf,0,buf_max);  //
	            
           serialport_read_until(fd, buf, eolchar, buf_max, timeout);
           if( !quiet ) printf("read string:");
           printf("%s\n", buf);
                
           //long volume = (-10000 + (atol(buf)*120));
	  
	   		uint8_t sendvolume;
                
           snd_mixer_t *handle;
           snd_mixer_selem_id_t *sid;
           const char *card = "default";
           //const char *selem_name = "Master";
 
           snd_mixer_open(&handle, 0);
           snd_mixer_attach(handle, card);
           snd_mixer_selem_register(handle, NULL, NULL);
           snd_mixer_load(handle);
 
           snd_mixer_selem_id_alloca(&sid);
           snd_mixer_selem_id_set_index(sid, 0);
           snd_mixer_selem_id_set_name(sid, "PCM");
    
           snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);
    
           snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    
    
 		 if(atol(buf)!=0){
	   		long x = atol(buf);
	   		printf("%ld\n",x);
	  		volume = (-10239 + (atol(buf)*41.72));
		
           printf("%ld\n",volume);   
           
	
           //printf("%ld\n",min);
           //printf("%ld\n",max);
		
           
           snd_mixer_selem_set_playback_volume_all(elem, volume);
           p=volume;
           
	       //p=volume;
	       
	       printf("%ld\n",p); 
	       printf("First if statement executed\n");
	    
	   
	   
	   }
		    
    
	   else if(atol(buf) == 0){

	   
	   if(p!=volume){
	   intervolume=(p+10239)/41.72;
	   sendvolume=intervolume;
	   serialport_writebyte(fd,sendvolume);
	   //serialport_flush(fd);
	   
	   printf("%ld\n",volume);
	   //printf("%ld\n",p);
	   printf("%i\n",sendvolume);  
	   printf("Second if statement executed\n");
	   volume=p;
	   
                
		}
		
		 
       else
    { 
       printf("No change\n");    
       printf("%ld\n",volume);
    
	   
       //oldvolume=p;     	
    }
    }
    
    snd_mixer_selem_get_playback_volume(elem,0,&p);
    snd_mixer_close(handle);
	}
   

} // end main

