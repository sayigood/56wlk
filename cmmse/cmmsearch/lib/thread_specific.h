/* $Id: thread_specific.h,v 1.1 2008/08/22 04:02:42 yanghy2 Exp $
 */ 
#ifndef _THREAD_SPECIFIC_H_
#define _THREAD_SPECIFIC_H_

#include <pthread.h>

template <typename TYPE>
class thread_specific
{
public:
	// Hook type for construction parameters.
	typedef void* (*new_hook_t)(void);

	// If do not set hook, use default constructor to new TYPE.
	thread_specific(new_hook_t hook = NULL);
	~thread_specific();

	// Use a "smart pointer" to get the thread-specific object
	// associated with the <key>.
	TYPE *operator-> () const {return this->ts_get ();}

	// Return or create and return the calling threads TYPE object.
	operator TYPE *(void) const {return this->ts_get ();}

	// Actually implements the code that retrieves the object from
	// thread-specific storage.
	TYPE *ts_get (void) const;

protected:
	// Factors out common code for initializing TSS.  This must NOT be
	// called with the lock held...
	int ts_init (void);

	// Make a new object.
	TYPE* make_tss_obj (void) const; 

	// Avoid race conditions during initialization.
	pthread_mutex_t keylock_;

	// "First time in" flag.
	int once_;

	// Key for the thread-specific data.
	pthread_key_t key_;

	// Hook for construction parameters.
	new_hook_t hook_;

	// <todo>"Destructor" that deletes internal TYPE * when thread exits.
	// static void cleanup (void *ptr);
};

template <class TYPE>
thread_specific<TYPE>::thread_specific (new_hook_t hook)
	: once_ (0), key_ (0), hook_ (hook)
{
	pthread_mutex_init(&this->keylock_, NULL);
}

template <class TYPE>
thread_specific<TYPE>::~thread_specific (void)
{
	if(once_) {
		pthread_key_delete(this->key_);
	}
	pthread_mutex_destroy(&this->keylock_);
}

template <class TYPE> TYPE*
thread_specific<TYPE>::make_tss_obj (void) const 
{
	if(hook_) { 
		return (TYPE*) hook_(); 
	} else {
		return new TYPE;
	}
}

template <class TYPE> int
thread_specific<TYPE>::ts_init (void)
{
	int ret = 0;

	pthread_mutex_lock(&this->keylock_);

	// Use the Double-Check pattern to make sure we only create the key once!
	if (this->once_ == 0) {
		if (pthread_key_create (&this->key_, NULL/*cleanup*/)) {
			ret = -1; // Major problems, this should *never* happen!
		} else {
			// This *must* come last to avoid race conditions!
			this->once_ = 1;
		}
	}

	pthread_mutex_unlock(&this->keylock_);
	return ret;
}

template <class TYPE> TYPE *
thread_specific<TYPE>::ts_get (void) const
{
	if (this->once_ == 0) {
		// Create and initialize thread-specific ts_obj.
		thread_specific<TYPE>* this_ = const_cast<thread_specific<TYPE>*>(this);
		if (this_->ts_init () == -1)
			return NULL;//seriously wrong..
	}

	TYPE *ts_obj = (TYPE*) pthread_getspecific(this->key_);

	// Check to see if this is the first time in for this thread.
	if (ts_obj == NULL) {
		// Allocate memory off the heap and store it in a pointer in
		// thread-specific storage (on the stack...).

		ts_obj = this->make_tss_obj ();

		if (ts_obj == 0) {
			return NULL;//no memory..
		}

		// Store the dynamically allocated pointer in thread-specific storage.
		if (pthread_setspecific (this->key_, (void *)ts_obj)) {
			delete ts_obj;
			return NULL; // Major problems, this should *never* happen!
		}
	}

	return ts_obj;
}

#endif

