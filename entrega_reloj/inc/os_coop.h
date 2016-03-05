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

#define OS_COOP_TASKS_LEN			5	// change this value to the amount of tasks the system has.

#define OS_COOP_TASK_PRIORITY_LOW	0
#define OS_COOP_TASK_PRIORITY_HIGH	1


/**
	os_coop_init : Initializes the operation system
*/
void os_coop_init(void);


/**
	os_coop_start : Starts the operating system. No tasks can be added while it is running.
*/
void os_coop_start(void);

/**
	os_coop_stop : Stops the operating system.
*/
void os_coop_stop(void);

/**
	os_coop_addTask		: Adds a task to the scheduler. This function must be called after os_coop_init and before os_coop_start

	@param functionPtr	: Pointer to task function.
	@param priority 	: Task Priority. Posble values are:
							- OS_COOP_TASK_PRIORITY_HIGH : Task will be loaded at the top of the scheduler queue
							- OS_COOP_TASK_PRIORITY_LOW : Task will be loaded at the bottom of the scheduler queue
	@param period		: Task period in miliseconds. If period is 0 the task will be executed each time os_coop_dispacher is called.

	@return				: 1 if the task was added. 0 in case of error.

	NOTE: Tasks cannot block code execution. Each task must be a state machine.
*/
uint32_t os_coop_addTask(void (*functionPtr)(void),uint32_t priority,uint32_t period);


/**
	os_coop_scheduler : This function must be called by system tick each 1 milisecond
*/
void os_coop_scheduler(void);


/** 
	os_coop_dispacher : This function must be called in the main loop.
*/
void os_coop_dispacher(void);

/**
	os_coop_waitMs 		: Non blocking delay. 

	@param delayNumber	: Delay number. There is one delay per task defined.

	@param delayMs		: Delay un miliseconds.

	@return				: 1 if delay is not complete. 0 if delay is finished.
*/
uint32_t os_coop_waitMs(uint32_t delayNumber,uint32_t delayMs);


/**
	os_coop_resetWait	: Resets a delay execution. This function must be called before os_coop_waitMs if the delay is not finished yet.

	@param delayNumber	: Delay number. There is one delay per task defined.
*/
void os_coop_resetWait(uint32_t delayNumber);


