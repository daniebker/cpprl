#ifndef PERSISTENT_H_
#define PERSISTENT_H_

class Persistent {
public :
    virtual void load(TCODZip &zip) = 0;
    virtual void save(TCODZip &zip) = 0;
};

#endif // PERSISTENT_H_
