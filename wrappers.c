#include <stdio.h>
#include <string.h>
#include "slstatus.h"

#define BATTERY_EMPTY 		"\uf244"
#define BATTERY_FULL  		"\uf240"
#define BATTERY_HALF		"\uf242"
#define	BATTERY_QUARTER		"\uf243"
#define BATTERY_THREE_QUARTERS	"\uf241"
#define BATTERY_CHARGING	"\uf3ca"

#define VOLUME_MUTE		"\uf380"
#define VOLUME_UP		"\uf028"
#define VOLUME_DOWN		"\uf027"
#define VOLUME_OFF		"\uf026"

#define COLOR_GRAY		"\x02"
#define COLOR_WHITE		"\x04"
#define COLOR_BLUE		"\x06"
#define COLOR_RED		"\x07"
#define COLOR_GREEN		"\x08"
#define COLOR_MAGENTA		"\x09"

char buf[1024];
int string_to_int(const char *str)
{
	int result = 0;
	int n = strlen(str);
	for(int i = 0; i < n; i++) {
		if(str[i] == '%') 
			break;
		result = result * 10 + (str[i] - '0');
	}

	return result;
}

void
cpytobuf(char *first, char *second, char *third, char *delim)
{
	strcpy(buf, first);
	strcat(buf, second);
	strcat(buf, delim);
	strcat(buf, third);
}

const char*
get_battery_perc(const char *batt)
{
	const char *perc;
	char *delim = " ";
	const char *state;
	char volume[4];
	int charger;
	perc = battery_perc(batt);
	state = battery_state(batt);

	if(*state == '+')
		charger = 1;
	else if(*state == '?') 
		charger = 2;
	else
		charger = 0;

	strcpy(volume, perc);
	int ret = string_to_int(perc);

	buf[0] = '\0';
	if(ret > 75) 
		cpytobuf(COLOR_GREEN, BATTERY_FULL, volume, delim);

	else if(ret > 50 && ret < 75) 
		cpytobuf(COLOR_GREEN,BATTERY_THREE_QUARTERS, volume, delim);

	else if( ret > 25 && ret < 50)
		cpytobuf(COLOR_GREEN, BATTERY_HALF, volume, delim);

	else if( ret > 10 && ret < 25)
		cpytobuf(COLOR_GREEN, BATTERY_QUARTER, volume, delim);

	else if(ret >= 0 && ret < 10) 
		cpytobuf(COLOR_GREEN, BATTERY_EMPTY, volume, delim);

	if(charger > 0) {
	       	strcat(buf, " ");	
		strcat(buf, BATTERY_CHARGING); 	
	}
	return buf;
}


const char *
get_volume(void)
{
	const char *mute = "muted";
	const char *vol_stat;
	char status[256];
	vol_stat = run_command("pamixer --get-volume-human");
	strcpy(status, vol_stat);
	
	if(strcmp(status, mute)) {
		int vol = string_to_int(status);
		if(vol >= 50) {
			strcpy(buf, VOLUME_UP);
			strcat(buf, " ");
			strcat(buf, status);
		} else if(vol > 20/* && vol < 50*/){
			strcpy(buf, VOLUME_DOWN);
			strcat(buf, " ");
			strcat(buf, status);
		} else {
			strcpy(buf, VOLUME_OFF);
			strcat(buf, " ");
			strcat(buf, status);
		}

	} else {
		strcpy(buf, VOLUME_MUTE);
		strcat(buf, "      ");
	}
	return buf;	
}
/*	
int main()
{

	printf("%s \n", get_battery_perc("BAT0"));
	printf("%s \n", get_volume());
	return 0;
}
*/
