// Vector.h -- specification of the Vector example/project for EE380L.5 EPL

// NOTE: you will amost certainly have to modify this file to complete the project (I did).

#pragma once

#ifndef _Vector_h
#define _Vector_h 1

#include<cstring>
#include<stdexcept>
#include<iterator>

template <typename T>
struct Data
{
	T** arr;
	unsigned int ref_count;
	unsigned int length;
	unsigned int capacity;
};

template <typename T>
class Vector {
public:
	static const size_t minimum_space = 8;
	size_t size(void) const;         // size returns the number of objects
		                            // currently in the Vector.  size can be
			                        // zero (in fact Vector::Vector(void) creates
				                    // an empty Vector where size == 0)
					                // size() should have the same value as
						            //   end() - begin()
	size_t capacity(void) const;     // capacity returns the total amount of
		                            // available objects in the vector.
			                        // capacity is always more than zero
				                    // and obviously always at least as
					                // large as size
	void reserve(size_t sp);		// resizes the array (if necessary) so that
						            // capacity() is made at least 'sp'

	class iterator;               // you must have an iterator type
  // or typedef ??? iterator      // it can be a nested class, or a typedef
		 						  // whichever you think is more appropriate
								  // NOTE: part of your grade will be based upon
                                  // the efficiency and simplicity of
                                  // your solution.  Don't make the iterator
                                  // class more complicated than it needs to
                                  // be. 
	typedef T value_type;

	class const_iterator;           // you must have a constant iterator too
  // or typedef ??? const_iterator; 

	Vector(void);					// create an empty Vector with 
									// capacity() == minimum_space and size() == 0

	explicit Vector(size_t sz);    // create a Vector with initial size 'sz'
		                           // the vector should contain sz objects
			                       // each object should be initialized
				                   // using the default T::T(void) constructor

							       //     minimum_space - sz
								   // or  0
								   // whichever is greater.

	Vector(const Vector<T>&);     // copy constructor.  You must copy lazily
		                          // i.e. use copy-on-write semantics
	template <typename AltType>
	Vector(const Vector<AltType>&);  // copy constructor for dissimilar type
		                             // you must copy immediately (not copy-on-write)
									 // when this function is called.

	void swap ( Vector < T > & );  // must execute in constant time.

	Vector<T>& operator=(const Vector<T>&); // your assignment operator
		                            // must also work lazily (copy-on-write).  We 
			                        // will be evaluating the speed of your code 
				                    // and the number of times you allocate memory

	template <typename AltType>
	Vector<T>& operator=(const Vector<AltType>&); // assignment operator for
		                            // vectors of alternate type.  This assignment
			                        // operator must perform an immediately copy
				                    // (no copy on write)

	~Vector(void);		            // The destructor must clean up, *no* memory
		                            // leaks!!!

	iterator begin(void);			// conventional begin() function
	const_iterator begin(void) const;// conventional begin for constants
	iterator end(void);           // conventional end
	const_iterator end(void) const; // ...etc.

  
	T& operator[](size_t);        // this function should be a guarded
		                          // dereference.  if the index is equal
			                      // to or greater than size() + range_start(), or less
				                  // than range_start(), then "throw std::out_of_range();"
	const T& operator[](size_t) const;// same as [] above, but for const vectors

	void push_back(const T&);		// increase the size of the vector by 1.
									// place the new object at the end of
									// all of the other objects.
									// do not allocate more memory unless necessary
	                                // if allocating more space is necessary
		                            // use "amortized doubling" 
	void pop_back(void);			// reduce the size of the vector by 1.
									// elminiate the object at the end of
									// the array.  This object must eventually
									// have T::~T invoked, although it is not
									// necessary to invoke the destructor
									// immediately (you can wait until the
									// whole vector is destroyed, or resized
									// or any other time that you think is
									// convenient --- as long as you
									// eventually invoke the destructor exactly
									// once.  You may invoke the destructor 
									// explicitly with syntax similar to p->~T();
									// however, you should be *very* *very*
									// careful if you do this
                                // pop_back is *not* required
                                // to reduce the available space (vectors
                                // never have to shrink)
                                // finally, if pop_back is invoked
                                // when the vector is empty, then you should
                                // "throw std::length_error();" 
	T& back(void);                // return the last object in the vector
		                           // note that this is the object *before*
                                // end() (end points after the last object)
                                // if the vector is empty, 
                                // "throw std::length_error();" 
	const T& back(void) const;    // same as back() but for constant vectors

	T& front(void);                // return the first object in the vector (same as op[](0))
                                // if the vector is empty, 
                                // "throw std::length_error();" 
	const T& front(void) const;    // same as front() but for constant vectors

	void insert(const_iterator p, const T&x); // I prefer you write this version of insert, with const_iterator as the argument
	//void insert(iterator p, const T& x);  // insert the new object 'x'
                                // into the vector immediately before the
                                // object pointed to by p.
                                // if p == end(), this is exactly the same as
                                // push_back(x)
                                // if p == begin(), this is exactly the same as
                                // push_front(x)
                                // for other values of p, make room for x in
                                // the middle of the vector

	void remove(const_iterator p); // I prefer this version of remove, 'cause it reminds you to make iterator => const_iterator type conversions
	//void remove(iterator p);  // remove the  object
                                // pointed to by p in vector.
                                // if p == begin(), this is exactly the same as
                                // pop_front(x)
                                // (if p == end(), then this is an error)
  
private:
	//void copy(const Vector<T>& that);
//	void destroy(void);
	template <typename AltType>
	void copyAltContent(const Vector<AltType>&);
	void allocateDefault();
	void copyContent(Data<T>* const);
	void deleteContent(Data<T>* );
	void setDataPtr(unsigned int ref, unsigned int len, unsigned int cap);
	void setDataPtr2(unsigned int ref, unsigned int len, unsigned int cap);
	void updateContents();
	Data<T>* m_data;
	Data<T>* getDataPtr() { return m_data; }
	void copyContentAfterInsert(Data<T>*,int,const T&);
	void copyContentAfterRemove(Data<T>*, int);
};

template <typename T>
class Vector<T>::const_iterator {
	protected:
		const Vector<T>* m_vector;
		unsigned int m_index;
		friend class Vector<T>;
	public:
		typedef T value_type;
		typedef std::random_access_iterator_tag iterator_type;
		const_iterator();
		const T& operator*(void) const;
		bool operator==(const const_iterator&) const;
		bool operator!=(const const_iterator&) const;
		int operator-(const const_iterator&) const;
		const_iterator operator+(const int);
		const T& operator [](const int) const;
		const_iterator operator-(const int);
		bool operator<(const const_iterator&) const;
		bool operator<=(const const_iterator&) const;
		bool operator>=(const const_iterator&) const;
		bool operator>(const const_iterator&)const;
		const_iterator& operator++(); //prefix
		const_iterator operator++(int); // postfix
		const_iterator& operator--(); //prefix
		const_iterator operator--(int); //postfix
private:
		const_iterator(const Vector<T>*,int);
};


template <typename T>
class Vector<T>::iterator{
protected:
    Vector<T>* m_vector;
    unsigned int m_index;
	friend class Vector<T>;
public:
    iterator();
    T& operator*(void);
	typedef T value_type;
	typedef std::random_access_iterator_tag iterator_type;
	const T& operator*(void) const;
    bool operator==(const iterator&) const;
    bool operator!=(const iterator&) const;
    int operator-(const iterator&) const;
    iterator operator+(const int);
    T& operator [](const int);
	const T& operator[](const int) const;
    iterator operator-(const int);
    bool operator<(const iterator&) const;
    bool operator<=(const iterator&) const;
    bool operator>=(const iterator&) const;
    bool operator>(const iterator&)const;
	iterator& operator++(); // prefix
	iterator operator++(int); // postfix
	iterator& operator--(); // prefix
	iterator operator--(int); // postfix
    operator const_iterator() const { return const_iterator(m_vector,m_index); }
private:
	iterator(Vector<T>*,int);
};


#include "Vector_impl.h"
#endif /* _Vector_h */