#ifndef __REVQUEUE_INL_H__
#define __REVQUEUE_INL_H__

template<typename T>
revQueue<T>::revQueue()
: first(0)
, last(0)
, capacity(0)
{
}

template<typename T>
revQueue<T>::revQueue(uint32 _capacity) 
: first(0)
, last(0)
, capacity(_capacity)
{
	queue = revArray<T>(capacity);
}

template<typename T>
bool revQueue<T>::Enqueue(T& obj)
{
	uint32 newLast = (last +1 ) % capacity;
	if( (last + 1) % capacity == first ){
		return false;
	}

	queue[last] = obj;
	last = newLast;
	return true;
}

template<typename T>
T& revQueue<T>::Dequeue()
{
	// is queue empty? 
	if(first == last){
		// TODO : Assert
	}

	T& ret = queue[first];
	first = (first + 1) % capacity;
	return ret;
}

template<typename T>
T& revQueue<T>::Peek() const
{
	// is queue empty? 
	if(first == last){
		// TODO : Assert
	}

	return queue[first];
}

template<typename T>
uint32 revQueue<T>::Count() const
{
	if( last >= first ){
		return last - first;
	}

	return capacity - first + last;
}

template<typename T>
void revQueue<T>::SetCapacity(uint32 _capacity)
{
	queue.SetCapacity(_capacity);
	capacity = _capacity;
}

#endif
