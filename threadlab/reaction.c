//
// Norman Kuang
//

#include "thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	// The system only needs to keep track of the total number of hydrogen 
	// atoms, since most of the heavy lifting will be delegated to the oxygen
	// atom.
	int hydrogen_atoms;

	// Multithreaded synchronization structures.
	struct lock lock;
	struct condition h_added;
	struct condition water_made;
};

void
reaction_init(struct reaction *reaction)
{
	// Initializing system states.
	reaction->hydrogen_atoms = 0;

	// Initializing reaction synch structures.
	lock_init(&(reaction->lock));
	cond_init(&(reaction->h_added));
	cond_init(&(reaction->water_made));
}

void
reaction_h(struct reaction *reaction)
{
	// Hydrogren atom created, acquire lock and increment the hydrogen count.
	lock_acquire(&(reaction->lock));
	reaction->hydrogen_atoms++;

	// Signal to the system that hydrogen has been created.
	cond_signal(&(reaction->h_added), &(reaction->lock));

	// Wait until water is made, then release the lock and return.
	cond_wait(&(reaction->water_made), &(reaction->lock));

	lock_release(&(reaction->lock));
	return;

}

void
reaction_o(struct reaction *reaction)
{
	// The algorithm here is to delegate most of the reaction logic to the 
	// oxygen, since it only needs to check 1 state (# of hydrogens) to 
	// determine if water can be made.

	// Acquire lock.
	lock_acquire(&(reaction->lock));

	while (reaction->hydrogen_atoms < 2) {
		// Not enough hydrogens.
		cond_wait(&(reaction->h_added), &(reaction->lock));
	}

	// Enough hydrogens - make water.
	make_water();
	reaction->hydrogen_atoms -= 2;

	// Signal twice, once for each hydrogen in the reaction.
	cond_signal(&(reaction->water_made), &(reaction->lock));
	cond_signal(&(reaction->water_made), &(reaction->lock));

	lock_release(&(reaction->lock));
	return;
}
