#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

typedef char integer[4];
typedef char floating[6];

char* record_delim = "#";
char* delim = "|"; 

struct employee {
    integer  empno;
    char name[10];
};

class Buffer
{
    int nextpos;
    public :
    char buff[100];
    int size;
    Buffer()
    {
        size = 40;
        nextpos=0;
        strcpy(buff, "");
    }

    void init()
    {
    	fstream f("file3.txt", ios::out);
        for(int i=0; i<size; i++)
            f<<" "<<record_delim[0];
        f.close();
    }

    int hash(char *empno)
    {
        return stoi(empno) % size;
    }

    void add(employee em)
    {
        int position = hash(em.empno), count = 0;
        while(cout < size -1)
        {
        	int done = 0;
            while(done == 0 && position < size)
            {
                done = pack(em, position);
				position++;
                count++;
            }
            if(done == 1)
            	break;
            if(position == size - 1 && count != size -1)
                position = 0;
        }
        if(count == size - 1)
            cout<<"Overflow";
    }

	int pos(int n)
	{
		char c;
		int i = 0, j = 0;
		fstream f("file3.txt", ios::in);
		while(i < n - 1)
        {
            f.get(c);
            if(c == record_delim[0])
                i++;
            j++;
        }
        f.close();
        return j;
	}
	
    int pack(employee em, int position)
    {   
    	fstream f("file3.txt", ios::out | ios::in);
        strcpy(buff, "");
        strcat(buff, "\r\r #");
        strcat(buff, em.empno);
        strcat(buff, delim);
        strcat(buff, em.name);
        nextpos += 14;
        f.seekp(pos(position), ios::beg);
        f.seekg(pos(position), ios::beg);
        char c, str[14 * 40];
        int i = 0;
		f.get(c);
        if(c != ' ')
            return 0;
        fstream f2("file3.txt", ios::in);
        f2.seekg(pos(position + 1) - 1, ios::beg);
        while(!f2.eof())
        	f2.get(str[i++]);
        f2.close();
        f.seekg(pos(position) - 1, ios::beg);
        f<<buff<<str;
        f.close();
        return 1;
    }

    employee unpack(int index)
    {
    	fstream f("file3.txt", ios::in);
        char c, old, *co = &c;
        employee s;
		f.seekg(pos(index), ios::beg);
        strcat(s.empno, "");
        strcat(s.name, "");
        for(int count=1; count <= 2; count++)
        {
            while(c != delim[0])
            {
                f.get(c);
                if(c >= '0' && c <= '9' && old == '#')
                    break;
                old = c;
            }
            do
            {
                if(c != delim[0] && c != record_delim[0])
                {
                   if(count == 1)
                        strcat(s.empno, co);
                    else if(count == 2)
                        strcat(s.name, co);
                } else 
                    break;
                f.get(c);
            } while(c != delim[0] && c != record_delim[0]);
        }
        f.close();
        return s;
    }
};

void read(Buffer b)
{
    fstream f("file3.txt", ios::in);
    char a[100];
    while(!f.eof()) {
        f.getline(a, 100, '#');
        if(strlen(a) >= 3)
            cout<<a<<endl;
    }
    f.close();
}

int search(char* empno, Buffer b)
{
    fstream f("file3.txt", ios::in);
    char a[100], emp[4];
    int count = b.hash(empno);
    while(!f.eof())
    {
        strcpy(emp, "");
    	f.seekg(count);
        f.getline(a, 100, '#');
        for(int i=0; a[i] != delim[0] && i<4; i++)
            emp[i] = a[i];
        if(strcmp(empno, emp) == 0)
        {
            f.close();
            return count;
        } else 	
        	count++;
    }
    f.close();
    return -1;
}   

int main()
{
    int op;
    employee em;
    Buffer b;
    b.init();
    do {
        cout<<"\n1. Enter Record\n2. Search Records\n3. View Records\n0. Quit\nEnter Option : ";
        cin>>op;
        switch(op) 
        {
            case 1:
                cout<<"Employee Number : ";
                cin>>em.empno;
                cout<<"Name : ";
                cin>>em.name;
                b.add(em);
                break;
            case 2:
            {
                char empno[4];
                cout<<"Empno to search : ";
                cin>>empno;
                int a=search(empno, b);
                if(a != -1)
                    cout<<"Found";
                else cout<<"Not found";
                break;
            }
            case 3: 
                read(b);
                break;
            case 0:
                cout<<"Quitting";
                break;
            default :
                cout<<"Invalid Option";
        }
    } while(op != 0);
    return 0;
}
