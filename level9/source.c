#include <iostream>
#include <cstring>
#include <cstdlib>

class N {
	private:
	    int value;

	public:
	    N(int val) : value(val) {}

	    void setAnnotation(const char* annotation) {
	        size_t len = strlen(annotation);
	        memcpy((char*)this + 4, annotation, len);
	    }

	    int operator+(const N& other) {
	        return this->value + other.value;
	    }

	    int operator-(const N& other) {
	        return this->value - other.value;
	    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        exit(1);
    }

    N* obj1 = new N(5);
    N* obj2 = new N(6);

    obj1->setAnnotation(argv[1]);

	return (obj1->operator+(*obj2));
}
