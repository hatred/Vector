#include <iostream>
#include <iterator>
#include<vector>
#include "Vector.h"

using namespace std;
class Foo1 {
public:
	Foo1(void) { cout << "default Foo1 constructor\n"; }
	Foo1(const Foo1&) { cout << "Foo1 copy constructor\n"; }
	Foo1& operator=(const Foo1&) { cout << "Foo1 assignment\n"; return *this; }
	~Foo1(void) { cout << "Foo1 destructor\n"; }
};

class Foo2 : public Foo1 {
public:
	int data;
	Foo2(int x) { data = x; }
	Foo2(void) { data = 0; }
	~Foo2(void) { cout << data; }
	
};

ostream &operator<<(ostream& out, const Foo2& f) {
	out << f.data;
	return out;
}

int main(void) {

    Vector<Foo2> ivector1;
	    Vector<Foo2> ivector2(ivector1);

		    cout <<"\t Should Call Deep Copy\n";
			    Vector<Foo2>::const_iterator it = ivector1.begin();
				    cout << "\t Vector Size "<<ivector1.size() << " Capacity " << ivector1.capacity() << endl;
					    for (int i = 0; i < 10; i++){
						        Foo2 f(i);
								        ivector1.insert(it, f);
										        ++it;
												    }
Foo1 protoFoo;
	Vector<double>T1(10);
	Vector<int>T2(10);

	Vector< Vector<int> >be;

	be.push_back(T2);
	be[0] = T2;
	be.push_back(T1);

	for(int i=0;i<be[0].size();i++)
		cout<<be[0][i]<<endl;
	Vector<const Foo1>AF;
	Vector<Foo1> x; // Ideally, the Foo1::Foo1(void) construtor should NOT be called
	x.push_back(protoFoo); // should use the copy constructor for Foo1 (not assignment)
	cout << "making a copy of x\n";
	Vector<Foo1> y(x); // should be a shallow copy (no Foo1 constructors)
	cout << "preparing to pop from x\n";
	x.pop_back(); // should perform a deep copy (ideally with just one Foo1 copy construtor)
    
	cout << "beginning iterator tests, will push_back 10 Foo2 instances\n";
    
	Vector<Foo2> v1;
	for (int k = 0; k < 10; k += 1) { // Ideally, we should just create 10 objects (we might also copy them)
		v1.push_back(k);
	}
	cout<<"iterator tests over"<<endl;

	cout << "making a copy of the Foo2 Vector\n";
	Vector<Foo2> v2(v1); // shallow copy
    
	cout << "using begin and end with anand constant iterators to iterate over the Vector\n";
	for (Vector<Foo2>::const_iterator p = v1.begin(); p != v1.end(); ++p) { // lots going on here
		/* (1) I'd like Vector<T>::iterator to be convertable to Vector<T>::const_iterator without a type cast
		 * (2) begin is not a mutating function and does not require a deep copy
		 * (3) and since we're dereferencing a const_iterator, we should not see a deep copy
		 */
		cout << *p << endl;
	}
    
	Foo2 protoFoo2(42);
    
	cout << "using begin and end to iterate over the Vector\n";
	for (Vector<Foo2>::iterator p = v1.begin(); p != v1.end(); ++p) { 
		/* with or without the assignment to *p there's likely a deep copy here
		 * if commenting out the assignment to *p removes the deep copy for you, then congratulations!
		 * please let me know how you solved it, as I am not aware of a good solution */
		cout << *p << endl;
		*p = protoFoo2;
	}
    
	cout << "restoring v1 back to 0..9 via assignment\n";
	v1 = v2; // shallow copy
    
	cout << "removing element 5 from v1\n";
	v1.remove(v1.begin() + 5); // deep copy
   	cout<<" removal call over"<<endl; 
	cout << "using begin and end with constant iterators 2 to iterate over the Vector\n";
	for (Vector<Foo2>::const_iterator p = v1.begin(); p != v1.end(); ++p) { 
		cout << *p << endl;
	}
    
	cout << "and inserting 42 where the 5 used to be\n";
	v1.insert(v1.begin() + 5, 42);
	
	cout << "using begin and end with constant iterators 3 to iterate over the Vector\n";
	for (Vector<Foo2>::const_iterator p = v1.begin(); p != v1.end(); ++p) { 
		cout << *p << endl;
	}
    
	/* here's an iterator test */
	Vector<Foo2>::iterator t = v1.begin();
	v2 = v1;
	*t = 42;
	cout << v1[0] << " is fortytwo and " << v2[0] << " is zero\n";
    
	cout << "letting x, y, v1 and v2 go out of scope (and the two protoFoo objects)\n";

	const Vector<int>ti(10);
	Vector<int>::const_iterator it8 = ti.begin();

	for(it8 = ti.begin();it8!=ti.end();it8++)
		cout<<*it8<<endl;
	
}



