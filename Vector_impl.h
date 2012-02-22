#ifndef _Vector_impl_h
#define _Vector_impl_h

//template <typename T>
//struct Data
//{
//	T* arr;
//	unsigned int ref_count;
//	unsigned int length;
//	unsigned int capacity;
//};

/* Default constructor */
template <typename T>
Vector<T>::Vector(void)
{
	allocateDefault();
}

template <typename T>
Vector<T>::Vector(size_t sz)
{
	if(sz < minimum_space)
		setDataPtr2(1,sz,minimum_space);
	else
		setDataPtr2(1,sz,sz);
}

template <typename T>
void Vector<T>::allocateDefault()
{
	setDataPtr(1,0,minimum_space);
}

template<typename T>
void Vector<T>::setDataPtr2(unsigned int ref, unsigned int len, unsigned int cap)
{
	m_data = new Data<T>();
	m_data->ref_count = ref;
	m_data->length = len;
	m_data->capacity = cap;

	m_data->arr = (T**) ::operator new(m_data->capacity * sizeof(T*));

	for(int i=0;i<m_data->length;i++)
	{
		m_data->arr[i] =   new T;
	}
}

template <typename T>
void Vector<T>::setDataPtr(unsigned int ref, unsigned int len, unsigned int cap)
{
	m_data = new Data<T>();
	m_data->ref_count = ref;
	m_data->length = len;
	m_data->capacity = cap;

	m_data->arr = (T**) ::operator new(m_data->capacity * sizeof(T*));
}

template <typename T>
size_t Vector<T>::size(void) const
{
	return m_data->length;
}

template <typename T>
size_t Vector<T>::capacity(void) const
{
	return m_data->capacity;
}

/* Copy constructor , Lazy on Write , just increment reference count */

template <typename T>
Vector<T>::Vector(const Vector<T>& that)
{
	// Don't do anything just increment ref count
	m_data = that.m_data;
	m_data->ref_count++;
}

/* Alt Type copy constructor */
template<typename T>
template <typename AltType>
Vector<T>::Vector(const Vector<AltType>& that)
{
	copyAltContent(that);
}

template<typename T>
template<typename AltType>
void Vector<T>::copyAltContent(const Vector<AltType>& that)
{
	setDataPtr(1,that.size(),that.capacity());	

	for(int i=0;i<(int)that.size();i++)
	{
		m_data->arr[i] = new T(that[i]);
	}
}

template<typename T>
template <typename AltType>
Vector<T>& Vector<T>::operator=(const Vector<AltType>& that)
{
	if(m_data->ref_count == 1)
	{
		deleteContent(m_data);
	}
	else
		m_data->ref_count--;
	
	copyAltContent(that);
	return *this;
}

/* Assignment Operator */
template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& that)
{
	if(this != &that)
	{
		m_data->ref_count--; // decrement the existing count
		if(m_data->ref_count == 0)
			deleteContent(m_data);
		m_data = that.m_data;
		m_data->ref_count++;
	}

	return *this;
}

/* push_back. Mutating operation */

template <typename T>
void Vector<T>::push_back(const T& that)
{
	// first check if the ref count is = 1 , this means you are the only one making changes to data
	// in this case just check if you have to reallocate and then do delete the previous array !
	int newCapacity = m_data->capacity;
	if(m_data->ref_count == 1)
	{
		if(m_data->length + 1 > m_data->capacity)
		{
			 // we need to reallocate , and then delete the previous m_data array
			newCapacity = m_data->capacity*2;
			Data<T>* prevData = m_data;
			setDataPtr(1,prevData->length,newCapacity);
			copyContent(prevData);
			deleteContent(prevData);
		}
	}
	else
	{
		// we need to make our own data array and then copy everything
		newCapacity = m_data->length + 1 > m_data->capacity ? m_data->capacity*2 : m_data->capacity;
		Data<T>* prevData = m_data;
		setDataPtr(1,prevData->length,newCapacity);

		// decrement reference count
		prevData->ref_count--;
		copyContent(prevData);
	}
	m_data->arr[m_data->length++] = new T(that);
}

template<typename T>
void Vector<T>::insert(const_iterator p, const T&x)
{
	Data<T>* prevData = m_data;
	if( p== this->end() )
	{
		this->push_back(x);
		return;
	}
	else
	{
		m_data->ref_count--;
		int newCapacity = m_data->length + 1 > m_data->capacity ? m_data->capacity*2 : m_data->capacity;
		setDataPtr(1,prevData->length+1,newCapacity);
	}
	int index = p - this->begin();
	copyContentAfterInsert(prevData,index,x);

	// Delete 
	if(prevData -> ref_count == 0)
		deleteContent(prevData);
}

template<typename T>
void Vector<T>::remove(const_iterator p)
{

	int index = p - this->begin();

	Data<T>* prevData = m_data;

	m_data->ref_count--;
	setDataPtr(1,prevData->length-1,prevData->capacity);

	copyContentAfterRemove(prevData,index);

	// Delete
	if(prevData -> ref_count == 0)
		deleteContent(prevData);
}

template<typename T>
void Vector<T>::copyContentAfterRemove(Data<T>* prevData, int index)
{
	for(int i=0;i<index;i++)
	{
		m_data->arr[i] = new T(*prevData->arr[i]);
	}

	for(int i=index;i<m_data->length;i++)
	{
		m_data->arr[i] = new T(*prevData->arr[i+1]);
	}
}

template<typename T>
void Vector<T>::copyContentAfterInsert(Data<T>* prevData,int index,const T& x)
{
	for(int i=0;i<index;i++)
	{
		m_data->arr[i] = new T(*prevData->arr[i]);
	}
	m_data->arr[index] = new T(x);
	for(int i=index;i<prevData->length;i++)
	{
		m_data->arr[i+1] = new T(*prevData->arr[i]);
	}
}

template<typename T>
void Vector<T>::swap ( Vector <T> & that)
{
	if(m_data->ref_count == 1)
	{
		deleteContent(m_data);
	}
	else
		m_data->ref_count--;
	
	m_data = that.m_data;
	m_data->ref_count++;
}

template<typename T>
void Vector<T>::pop_back(void)
{
	if(m_data->length==0)
	{
		throw std::length_error("Length Error Occured");
		return;
	}
	
	if(m_data->ref_count > 1)
	{
		// okay, we need to make a new copy !
		updateContents();
	}

	m_data->length--;
	//m_data->arr[m_data->length]->~T();
	delete m_data->arr[m_data->length];
}

template <typename T>
void Vector<T>::copyContent(Data<T>* const prevData)
{
	for(int i=0;i<prevData->length;i++)
	{
		m_data->arr[i] = new T(*prevData->arr[i]);
	}
}

template<typename T>
T& Vector<T>::operator[](size_t t)
{
	if(t >= m_data->length)
		throw std::out_of_range("Index out of Bounds");

	if(m_data->ref_count > 1)
	{
		// Ref count > 1 , copy first and then return
		updateContents();
	}
	return *m_data->arr[t];
}

template<typename T>
void Vector<T>::updateContents()
{
	Data<T>* prevData = m_data;
	setDataPtr(1,prevData->length,prevData->capacity);
	copyContent(prevData);
	prevData->ref_count--;
}

template<typename T>
const T& Vector<T>::operator[](size_t t) const
{
	if( t >= m_data->length)
		throw std::out_of_range("Index out of Bounds"); 
	// A read only function, just return
	return *m_data->arr[t];
}

template<typename T>
const T& Vector<T>::back(void) const
{

	return *m_data->arr[m_data->length-1];
}

template<typename T>
const T& Vector<T>::front(void) const
{
	if(m_data->length == 0)
		throw std::length_error("Length Error Occured");
	return *m_data->arr[0];
}

template<typename T>
T& Vector<T>::front(void)
{
	if(m_data->length==0)
		throw std::length_error("Length Error Occured");
	

	if(m_data->ref_count>1)
	{
		// decrement reference count and update pointers
		updateContents();
	}

	return *m_data->arr[0];
}

template<typename T>
T& Vector<T>::back(void)
{
	if(m_data->length==0)
	{
		throw std::length_error("Length Error Occured");
	}

	if(m_data->ref_count>1)
	{
		updateContents();
	}

	return *m_data->arr[m_data->length-1];
}

template<typename T>
void Vector<T>::reserve(size_t sp)
{
	if(sp <= m_data->capacity)
		return;
	
	Data<T>* prevData = m_data;
	

	int newCapacity = 2*sp;
	setDataPtr(1,prevData->length,newCapacity);

	copyContent(prevData);
	prevData->ref_count--;

	if(prevData->ref_count == 0)
		deleteContent(prevData);
}

template<typename T>
void Vector<T>::deleteContent(Data<T>* prevData)
{
	// call destructors for each of the items in the array
	for(int i=0;i<prevData->length;i++)
	{
		//prevData->arr[i]->~T();
		delete prevData->arr[i];
	}

	::operator delete(prevData->arr);
	delete prevData;
}

template<typename T>
Vector<T>::~Vector(void)
{
	m_data->ref_count--;
	if(m_data->ref_count == 0)
		deleteContent(m_data);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::begin()
{
	return iterator(this,0);
}

template<typename T>
typename Vector<T>::const_iterator Vector<T>::begin() const
{
	return const_iterator(this,0);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::end()
{
	return iterator(this,m_data->length);
}

template<typename T>
typename Vector<T>::const_iterator Vector<T>::end() const
{
	return const_iterator(this,m_data->length);
}

/* Iterator class definitions go here */

template <typename T>
Vector<T>::const_iterator::const_iterator()
{
	m_index = 0;
	m_vector = NULL;
}

template <typename T>
Vector<T>::const_iterator::const_iterator(const Vector<T>* data, int index)
{
	m_vector = data;
	m_index = index;
}



template<typename T>
const T& Vector<T>::const_iterator::operator*(void) const
{

	return this->m_vector->operator[](m_index);
}

template<typename T>
bool Vector<T>::const_iterator::operator==(const const_iterator& it2) const
{
	return (this->m_index == it2.m_index);
}

template<typename T>
bool Vector<T>::const_iterator::operator!=(const const_iterator& it) const
{
	return !(this->operator==(it));
}

template<typename T>
int Vector<T>::const_iterator::operator-(const const_iterator& it2) const
{
	return (this->m_index - it2.m_index);
}

template<typename T>
typename Vector<T>::const_iterator Vector<T>::const_iterator::operator+(const int offset)
{
	return const_iterator(this->m_vector,this->m_index + offset);
}

template<typename T>
typename Vector<T>::const_iterator Vector<T>::const_iterator::operator-(const int offset)
{
	return const_iterator(this->m_vector,this->m_index - offset);
}

template<typename T>
const T& Vector<T>::const_iterator::operator[](const int offset) const
{
	return this->m_vector->operator[](m_index + offset);
}

template<typename T>
bool Vector<T>::const_iterator::operator<(const const_iterator& that) const
{
	return (this->m_index < that.m_index);
}

template<typename T>
bool Vector<T>::const_iterator::operator<=(const const_iterator& that) const
{
	return ( this->operator<(that)  || this->operator==(that) );
}

template<typename T>
bool Vector<T>::const_iterator::operator>=(const const_iterator& that) const
{
	return !( this->operator<(that));
}

template<typename T>
bool Vector<T>::const_iterator::operator>(const const_iterator& that) const
{
	return !this->operator<=(that);
}

template<typename T>
typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator++()
{
	m_index++;
	return *this;
}

template<typename T>
typename Vector<T>::const_iterator Vector<T>::const_iterator::operator++(int)
{
	const_iterator it(this->m_vector,this->m_index);
	m_index++;
	return it;
}

template<typename T>
typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator--()
{
	m_index--;
	return *this;
}

template<typename T>
typename Vector<T>::const_iterator Vector<T>::const_iterator::operator--(int)
{
	const_iterator it(this->m_vector,this->m_index);
	m_index--;
	return it;
}

/* Iterator definitions to here */

template <typename T>
Vector<T>::iterator::iterator()
{
 	this->m_index = 0;
	this->m_vector = NULL;   
}

template <typename T>
Vector<T>::iterator::iterator(Vector<T>* data, int index)
{
    this->m_vector = data;
	this->m_index = index;
}



template<typename T>
T& Vector<T>::iterator::operator*(void)
{
	return this->m_vector->operator[](this->m_index);
}

template<typename T>
const T& Vector<T>::iterator::operator[](const int t) const
{
	return this->m_vector->operator[](m_index + t);
}

template<typename T>
const T& Vector<T>::iterator::operator*(void) const
{
	return this->m_vector->operator[](this->m_index);
}

template<typename T>
bool Vector<T>::iterator::operator==(const iterator& it2) const
{
	return (this->m_index == it2.m_index);
}

template<typename T>
bool Vector<T>::iterator::operator!=(const iterator& it) const
{
	return !(this->operator==(it));
}

template<typename T>
int Vector<T>::iterator::operator-(const iterator& it2) const
{
	return (this->m_index - it2.m_index);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator+(const int offset)
{
	return iterator(this->m_vector,this->m_index + offset);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator-(const int offset)
{
	return iterator(this->m_vector,this->m_index - offset);
}

template<typename T>
T& Vector<T>::iterator::operator[](const int offset)
{
	return this->m_vector->operator[](offset + this->m_index);
}

template<typename T>
bool Vector<T>::iterator::operator<(const iterator& that) const
{
	return (this->m_index < that.m_index);
}

template<typename T>
bool Vector<T>::iterator::operator<=(const iterator& that) const
{
	return ( this->operator<(that)  || this->operator==(that) );
}

template<typename T>
bool Vector<T>::iterator::operator>=(const iterator& that) const
{
	return !( this->operator<(that));
}

template<typename T>
bool Vector<T>::iterator::operator>(const iterator& that) const
{
	return !this->operator<=(that);
}

template<typename T>
typename Vector<T>::iterator& Vector<T>::iterator::operator++()
{
	m_index++;
	return *this;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator++(int)
{
	iterator it(this->m_vector,this->m_index);
	m_index++;
	return it;
}

template<typename T>
typename Vector<T>::iterator& Vector<T>::iterator::operator--()
{
	m_index--;
	return *this;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator--(int)
{
	iterator it(this->m_vector,this->m_index);
	m_index--;
	return it;
}


#endif /* _Vector_impl_h */