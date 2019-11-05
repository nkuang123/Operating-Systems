//
// Norman Kuang
// 

#include "thread.h"

struct station {
	// A subway station consists of passengers and trains, which each need to
	// be tracked. Specifically, a passenger can be in one of two states:
	//		1) Waiting for a train.
	//		2) Boarded a train and seated.
	// Likewise, a train can be in one of two states:
	//		1) Waiting for passengers to board (seats available)
	//		2) Satisfactorily loaded (no seats available/no more waiting pass.)
	int waiting_passengers;
	int boarded_passengers;
	int available_seats;

	// Multithreaded synchronization structures.
	struct lock lock;
	struct condition seats_available;
	struct condition train_full;
};

void
station_init(struct station *station)
{
	// Initializing station states.
	station->waiting_passengers = 0;
	station->boarded_passengers = 0;
	station->available_seats = 0;

	// Initializing station synch structures.
	lock_init(&(station->lock));
	cond_init(&(station->seats_available));
	cond_init(&(station->train_full));
}

void
station_load_train(struct station *station, int count)
{
	// Train arrived, first acquire lock, then change available seats to 
	// count and reset the number of boarded passengers.
	lock_acquire(&(station->lock));
	station->available_seats = count;
	station->boarded_passengers = 0;

	// Wait until satisfactorily loaded, either:
	//		1) No more waiting passengers.
	//		2) Train is fully boarded.
	while (!((station->waiting_passengers == 0) || 
		(station->boarded_passengers == count))) {
		// Signal to passengers that seats are still available.
		cond_signal(&(station->seats_available), &(station->lock));
		// Wait
		cond_wait(&(station->train_full), &(station->lock));
	}

	// Satisfactorily loaded.

	// First check the edge case in which the train 
	// arrives but there's no one waiting, we must reset available_seats.
	// Consider a scenario is which a train arrives, but there are no 
	// waiting passengers, thus leaving. Immediately after, a passenger will
	// arrive and try to board since there seems to be available seats. 
	// Because the next train hasn't arrived, available_seats has not been
	// changed. Boarding a non-existant train!
	station->available_seats = 0;

	// Release lock and return.
	lock_release(&(station->lock));
	return;
}

void
station_wait_for_train(struct station *station)
{
	// Passenger arrived, first acquire lock, then increment
	// waiting_passengers.

	lock_acquire(&(station->lock));
	station->waiting_passengers++;

	// Wait until train arrives AND there are enough available seats.
	while (!station->available_seats) {
		cond_wait(&(station->seats_available), &(station->lock));
	}

	// At least 1 seat available, passenger boards and gets seated, release
	// lock and return (station_on_board() called immediately after as per
	// specifications).
	lock_release(&(station->lock));
	return;
}

void
station_on_board(struct station *station)
{
	// Passenger has boarded and seated, first acquire lock, then increment
	// boarded_passengers while decrementing waiting_passengers and 
	// available_seats (do these atomically).
	lock_acquire(&(station->lock));

	station->available_seats--;
	station->waiting_passengers--;
	station->boarded_passengers++;

	// Now we want to signal the train_full condition in the case 
	// that the train is satisfactorily loaded.
	cond_signal(&(station->train_full), &(station->lock));

	// Release lock and return.
	lock_release(&(station->lock));
	return;
}
