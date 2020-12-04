#ifndef _INDEXARRAY_H
#define	_INDEXARRAY_H

#define CHUNK   (4096)

template<class T>
class IndexArray
{
public:
    IndexArray()
	{
        size = CHUNK;
        data = new T[size];
        count = 0;
    }

    ~IndexArray() {

        if(data)
		{
            delete []data;
            data = 0;
        }
    }

    void add(T* src)
	{
        if(count >= size)
		{
            //resize
            int s = size;
            size += CHUNK;
            T* n = new T[size];
            for(int i = 0; i < s; i++)
			{
                n[i] = data[i];
            }
            delete []data;
            data = n;
        }

        T* dst = data + count;
		*dst = *src;
        count++;
    }

    operator T*(void) const { return data; }
    const T* operator[](int i) const { return data + i; }

    int current() const { return count; }

private:
	int size;
	int count;
	T* data;
	
};

#endif	/* _INDEXARRAY_H */

