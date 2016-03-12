/*
	Cooperative Operating System
    Copyright (C) <2016>  <Ernesto Gigliotti>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdint.h"
#include "os_coop.h"

struct S_taskData{
	uint32_t priority;
	void (*functionPtr)(void);
	uint32_t period;
	uint32_t periodValue;
	uint32_t offset;
	uint32_t flagExecute;
	uint32_t flagEmpty;
};
typedef struct S_taskData	OsCoopTaskData;

struct S_osCoopData{
	uint32_t flagRun;
};
typedef struct S_osCoopData	OsCoopData;

struct S_osCoopDelayData{
	uint32_t running;
	uint32_t delayValue;
};
typedef struct S_osCoopDelayData OsCoopDelayData;


static void recalculateTasksOffsets(void);

static volatile OsCoopTaskData tasksData[OS_COOP_TASKS_LEN];
static volatile OsCoopData osCoopData;
static volatile OsCoopDelayData osCoopDelays[OS_COOP_TASKS_LEN];

void os_coop_init(void)
{
	osCoopData.flagRun = 0;

	uint32_t i;
	for(i=0; i<OS_COOP_TASKS_LEN; i++)
		tasksData[i].flagEmpty=1;

}
void os_coop_start(void)
{
	recalculateTasksOffsets();

	osCoopData.flagRun = 1;
}
void os_coop_stop(void)
{
	osCoopData.flagRun = 0;
}

uint32_t os_coop_addTask(void (*functionPtr)(void),uint32_t priority,uint32_t period)
{
	uint32_t i;
	uint32_t freeIndex=-1;
	if(priority==OS_COOP_TASK_PRIORITY_HIGH)
	{
		for(i=0; i<OS_COOP_TASKS_LEN; i++)
		{
			if(tasksData[i].flagEmpty==1)
			{
				freeIndex=i;
				break;
			}
		}
	}
	else
	{
		for(i=OS_COOP_TASKS_LEN-1; i>0; i--)
		{
			if(tasksData[i].flagEmpty==1)
			{
				freeIndex=i;
				break;
			}
		}
	}
	if(freeIndex==-1)
		return 0; // error

	tasksData[freeIndex].priority = priority;
	tasksData[freeIndex].period = period;
	tasksData[freeIndex].functionPtr = functionPtr;
	tasksData[freeIndex].flagExecute = 0;
	tasksData[freeIndex].periodValue = period;
	tasksData[freeIndex].flagEmpty = 0;


	return 1;
}

void os_coop_scheduler(void) // each milisecond
{
	if(osCoopData.flagRun==1)
	{
		uint32_t i;
		for(i=0; i<OS_COOP_TASKS_LEN; i++)
		{
			//delays
			if(osCoopDelays[i].running==1)
			{
				if(osCoopDelays[i].delayValue>0)
					osCoopDelays[i].delayValue--;
			}

			// tasks
			if(tasksData[i].flagEmpty==0)
			{	
				if(tasksData[i].offset>0)
				{
					tasksData[i].offset--;
					continue;
				}
				else
				{
					if(tasksData[i].periodValue>0)
					{
						tasksData[i].periodValue--;
						if(tasksData[i].periodValue==0)
						{
							tasksData[i].flagExecute=1;
							tasksData[i].periodValue = tasksData[i].period;
						}
					}
					else if(tasksData[i].period==0)
						tasksData[i].flagExecute=1;
				}
			}

			
		}
	}
}


void os_coop_dispacher(void)
{
		uint32_t i;
		for(i=0; i<OS_COOP_TASKS_LEN; i++)
		{
			if(tasksData[i].flagEmpty==0)
			{
				if(tasksData[i].flagExecute==1)
				{
					tasksData[i].functionPtr();
					if(tasksData[i].period!=0)
						tasksData[i].flagExecute=0;
				}				
			}
		}	
}

uint32_t os_coop_waitMs(uint32_t delayNumber,uint32_t delayMs)
{
	if(osCoopDelays[delayNumber].running==0)
	{
		osCoopDelays[delayNumber].delayValue = delayMs;
		osCoopDelays[delayNumber].running=1;
	}
	else
	{
		if(osCoopDelays[delayNumber].delayValue==0)
		{
			osCoopDelays[delayNumber].running=0;
			return 0;
		}	
	}
	return 1;
}
void os_coop_resetWait(uint32_t delayNumber)
{
	osCoopDelays[delayNumber].running=0;
}



static int gcd(int x, int y)
{
    int r;
    if (x <= 0 || y <= 0)
        return(0);
    while ((r = x % y) != 0)
    {
        x = y;
        y = r;
    }
    return(y);
}
static uint32_t gcd_a(uint32_t n, uint32_t A[n]){
    uint32_t g = A[0];
	uint32_t i;
    for (i = 1; i < n; i++)
        g = gcd(g, A[i]);
	return g;
}

static void recalculateTasksOffsets(void)
{
	uint32_t periods[OS_COOP_TASKS_LEN];
	uint32_t i;
	uint32_t c;
	uint32_t gcd;

	// get common divisor from tasks array
	c=0;
	for(i=0; i<OS_COOP_TASKS_LEN; i++)
	{
		if(tasksData[i].flagEmpty==0)
		{
			periods[c]=tasksData[i].periodValue;
			c++;
		}
	}
	gcd = gcd_a(c, periods);
	//_____________________________________


	if(gcd>1 && c>1)
	{
		// exist common divisor, add offsets to tasks
		uint32_t offset=0;
		for(i=0; i<OS_COOP_TASKS_LEN; i++)
		{
			if(tasksData[i].flagEmpty==0)
			{
				tasksData[i].offset=offset;
				offset++;
			}
		}
		//____________________________________________
	}

}
